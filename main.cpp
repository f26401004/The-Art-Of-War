#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // create the window.
    MainWindow win;
    // show the window.
    win.show();   
    return app.exec();
}
