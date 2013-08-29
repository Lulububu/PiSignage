#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QtQuick/QtQuick>
#include <QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QFile>





class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);

    void downloadFile(QUrl, QString);
    bool downloadOver();

protected:
    QNetworkAccessManager m_WebCtrl;
    QUrl url;
    int length;
    bool download_over;
    QFile* file;

    void downloadWholeFile();
    void downloadChunk(QNetworkRequest req, int startByte, int endByte);
    
signals:
    void finished();
    void chunkSaved();

    void percentageDownloaded(int);
    
public slots:
    void setFileSize();
    void saveChunk();
    
};

#endif // DOWNLOADMANAGER_H
