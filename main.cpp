#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("tgtsml"));
//    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
//    qputenv("QT_VIRTUALKEYBOARD_STYLE", QByteArray("retro"));
    QApplication a(argc, argv);

    MainWidget w;
//    UserListForm w;
    w.show();
    return a.exec();
}
