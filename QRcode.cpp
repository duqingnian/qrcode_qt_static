#include "QRcode.h"
#include <QPushButton>
#include <QMenu>
#include <QClipboard>
#include <QFileDialog>
#include <common/Toast.h>
#include <QSettings>

QRcode::QRcode(QWidget *parent): QDialog(parent), ui(new Ui::QRcodeClass())
{
    ui->setupUi(this);
    this->setLayout(ui->LayoutMain);
    connect(ui->btn_generate,&QPushButton::clicked, this, &QRcode::generate_qrcode);

    QSettings settings("DUQN", "AdoTools");
    lastSavePath = settings.value("lastSavePath", QCoreApplication::applicationDirPath()).toString();

    renderColors();
}

void QRcode::generate_qrcode()
{
    QString text = ui->edit_content->text();

    if (text.trimmed().isEmpty())
    {
        Toast::err("没有输入任何内容");
    }
    else
    {
        QImage unscaledImage = m_generator.generateQr(text, ui->label_render->width(), 1, qrcodegen::QrCode::Ecc::HIGH, selectedColorButton->property("color").toString());
        QImage image = unscaledImage.scaled(ui->label_render->size());

        ui->label_render->setPixmap(QPixmap::fromImage(image));
        ui->label_render->setToolTip(text);
    }
}

QRcode::~QRcode()
{
    QSettings settings("DUQN", "AdoTools");
    settings.setValue("lastSavePath", lastSavePath);

    delete ui;
}

void QRcode::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        QMenu* menu = new QMenu(this);
        QAction* copyAction = new QAction("复制", this);
        QAction* saveAction = new QAction("另存为", this);

        connect(copyAction, &QAction::triggered, this, [this]() {
            QPixmap image = ui->label_render->pixmap().copy();
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setPixmap(image);
            Toast::info("图片已复制到剪贴板");
            });

        connect(saveAction, &QAction::triggered, this, &QRcode::save_as_image);

        menu->addAction(copyAction);
        menu->addAction(saveAction);
        menu->exec(event->globalPosition().toPoint());
    }
}

void QRcode::renderColors()
{
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(5,5,5,5);

    QLabel* label = new QLabel("颜色：", this);
    label->setFixedWidth(30);

    hLayout->addWidget(label);

    for (const QString& colorCode : colors) {
        // 创建按钮
        QPushButton* button = new QPushButton;
        button->setFixedSize(20, 20);
        button->setProperty("color", colorCode);
        button->setStyleSheet("background-color: " + colorCode + "; border: 5px solid #fff;");  // 设置按钮颜色
        hLayout->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, button, colorCode]() {
            // 按钮点击时的处理逻辑
            if (selectedColorButton == button) {
                return;  // 如果已经选中当前按钮，忽略点击
            }

            // 取消当前选中按钮的边框
            if (selectedColorButton) {
                selectedColorButton->setStyleSheet(selectedColorButton->styleSheet().replace("border: 5px solid green;", "border: 5px solid #fff;"));
            }

            // 设置当前按钮的边框为绿色
            button->setStyleSheet("background-color: " + colorCode + "; border: 5px solid green;");
            selectedColorButton = button;
        });
        hLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        ui->widget_colors->setLayout(hLayout);
        colorButtons.append(button);
    }

    colorButtons.at(0)->click();
}

void QRcode::save_as_image()
{
    QString defaultFileName = ui->edit_content->text().trimmed();
    if (defaultFileName.isEmpty()) 
    {
        defaultFileName = "二维码图像";
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", lastSavePath + "/" + defaultFileName + ".png", "PNG Files (*.png)");

    if (!fileName.isEmpty()) 
    {
        QPixmap image = ui->label_render->pixmap().copy();
        if (image.save(fileName)) 
        {
            Toast::info("图片已保存为 " + fileName);
            lastSavePath = QFileInfo(fileName).absolutePath();
        }
        else {
            Toast::err("保存图片失败");
        }
    }
}


