#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <matrix.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //makes application
    MainWindow w;               //creates window
    w.show();                   //shows window
    return a.exec();            //executes application

}
