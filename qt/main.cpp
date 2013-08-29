#include <QtGui/QtGui>
#include <QtQuick/QtQuick>
#include <QDebug>
#include "connectionmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    ConnectionManager manager;


    return app.exec();
}
