#include "QRcode.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRcode w;
    w.show();
    return a.exec();
}
