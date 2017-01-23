#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setWindowFlags(w.windowFlags()&~Qt::WindowMaximizeButtonHint);
    //QDesktopWidget *dw = QApplication::desktop();
    //w.setFixedSize(dw->width()*0.5, dw->height()*0.5);
    w.show();
    w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);
    return a.exec();
}
