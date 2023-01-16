#include "mainwindow.h"

#include <QApplication>
#include "ThreadPool.h"

int main(int argc, char *argv[])
{
    ThreadPool* pool = ThreadPool::getInstance();
    pool->start();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
