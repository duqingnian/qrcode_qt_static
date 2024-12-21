#pragma once

#include <QtWidgets/QDialog>
#include "ui_QRcode.h"
#include "QrCodeGenerator.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class QRcodeClass; };
QT_END_NAMESPACE

class QRcode : public QDialog
{
    Q_OBJECT

public:
    QRcode(QWidget *parent = nullptr);
    ~QRcode();
    void mousePressEvent(QMouseEvent* event) override;
    void renderColors();

private:
    Ui::QRcodeClass *ui;
    QrCodeGenerator m_generator;
    QString lastSavePath;

    QVector<QPushButton*> colorButtons;
    QStringList colors = {
        "#000000", "#a4373a", "#0078d4", "#217346", "#d83b01", "#077568", "#2b579a", "#008272",
        "#106ebe", "#b7472a"
    };
    QPushButton* selectedColorButton = nullptr; 

private slots:
    void generate_qrcode();
    void save_as_image();
};
