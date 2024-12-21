#pragma once

#include <QImage>
#include <QObject>
#include <QString>

#include "qrcodegen/qrcodegen.h"

class QrCodeGenerator : public QObject
{
public:
    explicit QrCodeGenerator(QObject* parent = nullptr);

    /**
     * @brief Generates a QR code from the given data and error correction level.
     * @param data The QString containing the data to encode in the QR code.
     * @param size The desired width/height of the generated image (default: 1000).
     * @param borderSize The desired border width of the generated image (default: 1).
     * @param errorCorrection The desired error correction level (default:
     * qrcodegen::QrCode::Ecc::MEDIUM).
     * @param color The color of the QR code modules in hex format (e.g., "#ff0000" for red).
     *
     * @return QImage containing the generated QR code.
     */
    QImage generateQr(const QString& data, const quint16 size = 1000, const quint16 borderSize = 1,
        qrcodegen::QrCode::Ecc errorCorrection = qrcodegen::QrCode::Ecc::MEDIUM,
        const QString& color = "#000000");

    /**
     * @brief Generates a QR code from the given data and error correction level.
     * @param data The QString containing the data to encode in the QR code.
     * @param borderSize The desired border width of the generated image (default: 1).
     * @param errorCorrection The desired error correction level (default:
     * qrcodegen::QrCode::Ecc::MEDIUM).
     * @param color The color of the QR code modules in hex format (e.g., "#ff0000" for red).
     *
     * @return QString string containing the generated QR code in SVG format.
     */
    QString generateSvgQr(const QString& data, const quint16 borderSize = 1,
        qrcodegen::QrCode::Ecc errorCorrection = qrcodegen::QrCode::Ecc::MEDIUM,
        const QString& color = "#000000");

private:
    /**
     * @brief Converts a qrcodegen::QrCode object to a SVG image.
     * @param qrCode The qrcodegen::QrCode object to convert.
     * @param border The desired border width of the generated image (default: 1).
     * @param color The color of the QR code modules in hex format (e.g., "#ff0000" for red).
     *
     * @return SVG containing the QR code.
     */
    QString toSvgString(const qrcodegen::QrCode& qr, quint16 border, const QString& color) const;

    /**
     * @brief Converts a qrcodegen::QrCode object to a QImage.
     * @param qrCode The qrcodegen::QrCode object to convert.
     * @param size The desired width/height of the generated image.
     * @param borderSize The desired border width of the generated image.
     * @param color The color of the QR code modules in hex format (e.g., "#ff0000" for red).
     *
     * @return QImage containing the QR code.
     */
    QImage qrCodeToImage(const qrcodegen::QrCode& qrCode, quint16 border, const quint16 size, const QString& color) const;
};
