#include <QCoreApplication>
#include "mainworker.h"

#define BSP_VERSION "0.0.1"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Starting BSP v" << QString(BSP_VERSION);
    MainWorker myworker;
    if(myworker.startUp())
        return a.exec();
    else return -1;
}
