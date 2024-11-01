#ifndef _TOTP_H_
#define _TOTP_H_

#include <openssl/hmac.h>
#include <openssl/sha.h>

#include <chrono>
#include <cmath>
#include <string>

#include "core/connection.h"
#include "core/utils.h"

class TOTP {
  public:
    TOTP() = default;

    void handle(const std::string& data, const Connection& con) {
        auto inputs = strSplit(data, ' ');
        std::string rs;
        for (const auto& code : inputs) {
            auto totp = generate(code);
            rs.append(totp);
            rs.append(" ");
        }

        con.send(rs);
    }

    std::string generate(const std::string& input, int timeStep = 30, int digits = 6) {
        std::string secret = base32Decode(input);

        // Convert counter to binary
        // Get the current time as an integer
        auto currentTime = std::chrono::system_clock::now();
        auto epoch = std::chrono::time_point_cast<std::chrono::seconds>(currentTime).time_since_epoch().count();
        uint64_t counter = epoch / timeStep;

        // Convert counter to binary
        unsigned char counterBin[8];
        for (int i = 7; i >= 0; --i) {
            counterBin[i] = counter & 0xff;
            counter >>= 8;
        }

        // Generate HMAC-SHA1
        unsigned char* hmac =
            HMAC(EVP_sha1(), secret.c_str(), secret.size(), counterBin, sizeof(counterBin), nullptr, nullptr);

        // Extract dynamic binary code
        int offset = hmac[19] & 0xf;
        int binaryCode = ((hmac[offset] & 0x7f) << 24) | ((hmac[offset + 1] & 0xff) << 16) |
                         ((hmac[offset + 2] & 0xff) << 8) | (hmac[offset + 3] & 0xff);

        // Get the OTP (using modulus)
        int otp = binaryCode % static_cast<int>(std::pow(10, digits));  // 6-digit OTP
        return std::to_string(otp);
    }
};

#endif