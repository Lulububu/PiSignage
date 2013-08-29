#include "filemanager.h"

FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
}

int FileManager::getFreeDiskSpace(){
    int64_t available;

    QString tmp = QCoreApplication::applicationDirPath();
    #ifdef WIN32
        GetDiskFreeSpaceEx(tmp.toStdString().c_str(),(PULARGE_INTEGER)&available,NULL,NULL);
    #elif defined (linux)
        struct statfs sf;
        statfs(tmp.toStdString().c_str(), &sf);
        //f_bavail contient le nombre de blocks disponibles, f_bsize contient la taille d'un block en octet
        available=sf.f_bavail*sf.f_bsize;
    #endif
    return available;
}

void FileManager::clearCache(){
    qDebug() << "clearing cache";
    QString path = QCoreApplication::applicationDirPath() + "/videos";
    QDir dir(path);
//    dir.setNameFilters(QDir::Files);
    dir.setFilter(QDir::Files);

    foreach (QString entry, dir.entryList()) {
        qDebug() << "delete " << entry;
        dir.remove(entry);
    }
}
