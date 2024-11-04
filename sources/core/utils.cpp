
#include "utils.h"

#include <openssl/bio.h>
#include <openssl/evp.h>

#include <algorithm>
#include <cstdlib>
#include <map>
#include <sstream>

#include "types.h"

void allocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void freeWriteReqCb(uv_write_t* req, int status) {
    if (status < 0) {
        fprintf(stderr, "Write error %s\n", uv_err_name(status));
    }

    WriteReq* wr = (WriteReq*)req;
    if (wr && wr->buff.len) {
        delete wr->buff.base;
    }
    free(wr);
}

void freePipeCb(uv_handle_t* handle) {
    uv_pipe_t* pipe = (uv_pipe_t*)handle;
    free(pipe);
}

size_t randomId() { return std::rand(); }

std::vector<std::string> strSplit(const std::string& str, char delimiter) {
    std::vector<std::string> rs;
    std::istringstream stream(str);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        rs.push_back(token);
    }

    return rs;
}

std::vector<std::string> strSplitFirts(const std::string& str, char delimiter) {
    std::vector<std::string> rs;
    auto pos = str.find(delimiter);
    if (pos == std::string::npos) {
        rs.push_back(str);
    } else {
        auto p1 = std::string(str.begin(), str.begin() + pos);
        rs.push_back(p1);

        if (pos + 1 < str.size()) {
            auto p2 = std::string(str.begin() + pos + 1, str.end());
            rs.push_back(p2);
        }
    }

    return rs;
}

std::string base32Decode(const std::string& input) {
    const std::map<char, uint32_t> DecodingTable{
        {'A', 0},  {'B', 1},  {'C', 2},  {'D', 3},  {'E', 4},  {'F', 5},  {'G', 6},  {'H', 7},
        {'I', 8},  {'J', 9},  {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
        {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
        {'Y', 24}, {'Z', 25}, {'2', 26}, {'3', 27}, {'4', 28}, {'5', 29}, {'6', 30}, {'7', 31},
    };
    std::ostringstream output;
    uint32_t buffer = 0;
    size_t bits = 0;
    for (auto datum : input) {
        const auto entry = DecodingTable.find(datum);
        uint32_t group = 0;
        if (entry != DecodingTable.end()) {
            group = entry->second;
        }
        buffer <<= 5;
        bits += 5;
        buffer += group;
        if (bits >= 8) {
            if (datum != '=') {
                output << (char)(buffer >> (bits - 8));
            }
            buffer &= ~(0xff << (bits - 8));
            bits -= 8;
        }
    }
    return output.str();
}

std::string toLower(const std::string& input) {
    std::string output = input;  // Create a copy of the input string
    std::transform(output.begin(), output.end(), output.begin(), [](unsigned char c) { return std::tolower(c); });
    return output;
}
