#include <QApplication>
#include "mainwindow.h"

#include <iostream>

int main(int argc, char **argv) {
	std::cout << "arftracksat by arf20" << std::endl;

    // start Qt main window
    QApplication app(argc, argv);
    MainWindow mainwindow;

    mainwindow.show();
    return app.exec();
}
