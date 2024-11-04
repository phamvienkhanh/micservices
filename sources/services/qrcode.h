#ifndef _QRCODE_H_
#define _QRCODE_H_

#include <ZXing/Barcode.h>
#include <ZXing/BarcodeFormat.h>
#include <ZXing/ReadBarcode.h>

#include <string>

#include "core/connection.h"
#include "core/utils.h"
#include "stb_image.h"

class QRcode {
  public:
    QRcode() = default;

    void handle(const std::string& data, const Connection& con) {
        auto inputs = strSplit(data, ' ');
        std::string rs;
        for (const auto& code : inputs) {
            auto codes = read(code);
            for (const auto& code : codes) {
                rs += code;
                rs += "\n==============================\n";
            }
        }

        if (rs.empty()) {
            con.send("invalid command");

        } else {
            con.send(rs);
        }
    }

    std::vector<std::string> read(const std::string& filePath) {
        ZXing::ReaderOptions options;
        options.setFormats(ZXing::BarcodeFormat::QRCode | ZXing::BarcodeFormat::MicroQRCode |
                           ZXing::BarcodeFormat::RMQRCode);
        options.setTextMode(ZXing::TextMode::HRI);
        options.setEanAddOnSymbol(ZXing::EanAddOnSymbol::Read);

        int width, height, channels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        auto ImageFormatFromChannels =
            std::array{ZXing::ImageFormat::None, ZXing::ImageFormat::Lum, ZXing::ImageFormat::LumA,
                       ZXing::ImageFormat::RGB, ZXing::ImageFormat::RGBA};
        ZXing::ImageView img{data, width, height, ImageFormatFromChannels.at(channels)};

        ZXing::Barcodes codes = ZXing::ReadBarcodes(img, options);

        std::vector<std::string> rs;
        for (const auto& it : codes) {
            rs.push_back(it.text());
        }

        return rs;
    }
};

#endif