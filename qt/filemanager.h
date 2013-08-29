#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

#ifdef WIN32
    #include <Windows.h>
#elif defined (linux)
    #include <sys/statfs.h>
#endif

class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = 0);
    static int getFreeDiskSpace();
    static void clearCache();
    
signals:
    
public slots:
    
};

#endif // FILEMANAGER_H
