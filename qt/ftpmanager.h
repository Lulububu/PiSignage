#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QtNetwork>
#include <QUrl>
#include <QFile>
#include <QMutex>
#include <iostream>
#include "qftp.h"

using namespace std;

class FtpManager:public QFtp
{
    Q_OBJECT
public:
    FtpManager();

    void downloadFile(QUrl, QString);

signals:
    void finished(bool);
    void percentageDownloaded(int);


private slots:
    void connectionStateChanged(int state);
    void viewProgress(qint64 done ,qint64 total);
    void listInfo(QUrlInfo info);
    void commandFinished(int id,bool error);
    void commandStarted(int);
    void readData();
    void saveData();
    void checkFileSize(int ftp_file_size);

private:
    map<int,QString> m_MStateMap;
    QFile* file;
    int get_command;
    QMutex mutex;
    QUrl url;
    int getsize_command;

    void beginDownload();

};

#endif // FTPMANAGER_H





