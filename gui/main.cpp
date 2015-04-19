#include "Config.h"
#include "mainwindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	Config config;
    MainWindow w(&config);
    w.show();
    return a.exec();
}
