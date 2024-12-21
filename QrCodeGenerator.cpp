#include "QrCodeGenerator.h"
#include <QPainter>
#include <QColor>
#include <QImage>
#include <QSvgRenderer>
#include <QTextStream>

QrCodeGenerator::QrCodeGenerator(QObject* parent)
    : QObject(parent)
{
}

QImage QrCodeGenerator::generateQr(const QString& data, const quint16 size, const quint16 borderSize,
    qrcodegen::QrCode::Ecc errorCorrection, const QString& color)
{
    auto b = data.toUtf8();
    const auto qrCode = qrcodegen::QrCode::encodeText(b.constData(), errorCorrection);
    return qrCodeToImage(qrCode, borderSize, size, color);
}

QString QrCodeGenerator::generateSvgQr(const QString& data, const quint16 borderSize,
    qrcodegen::QrCode::Ecc errorCorrection, const QString& color)
{
    auto b = data.toUtf8();
    const auto qrCode = qrcodegen::QrCode::encodeText(b.constData(), errorCorrection);
    return toSvgString(qrCode, borderSize, color);
}

QString QrCodeGenerator::toSvgString(const qrcodegen::QrCode& qr, quint16 border, const QString& color) const
{
    QString str;
    QTextStream sb(&str);

    sb << R"(<?xml version="1.0" encoding="UTF-8"?>)"
        << R"(<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">)"
        << R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1" viewBox="0 0 )"
        << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2)
        << R"(" stroke="none"><rect width="100%" height="100%" fill="#FFFFFF"/><path d=")";

    QColor qrColor(color);
    QString hexColor = qrColor.name(); // get hex code for color

    for (int y = 0; y < qr.getSize(); y++)
    {
        for (int x = 0; x < qr.getSize(); x++)
        {
            if (qr.getModule(x, y))
            {
                sb << (x == 0 && y == 0 ? "" : " ") << "M" << (x + border) << "," << (y + border)
                    << "h1v1h-1z";
            }
        }
    }

    sb << R"(" fill=")" << hexColor << R"("/></svg>)";
    return str;
}

QImage QrCodeGenerator::qrCodeToImage(const qrcodegen::QrCode& qrCode, quint16 border,
    const quint16 size, const QString& color) const
{
    QString svg = toSvgString(qrCode, border, color);
    QSvgRenderer render(svg.toUtf8());
    QImage image(size, size, QImage::Format_RGB32);  // Using RGB32 to support color
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    render.render(&painter);
    return image;
}
