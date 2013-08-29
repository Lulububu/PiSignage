#ifndef QMLMANAGER_H
#define QMLMANAGER_H

#include <QObject>
#include <QtQuick/QtQuick>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QTimer>

#include "json.h"
#include "downloadmanager.h"
#include "ftpmanager.h"
#include "qftp.h"
#include "filemanager.h"

class ConnectionManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(StateCode)
public:
    enum StateCode {REFRESH, PASS,ERROR,EMPTY_SCENE};

    explicit ConnectionManager(QObject *parent = 0);

    Q_INVOKABLE StateCode requestData();

    Q_INVOKABLE QString getUrl();

    Q_INVOKABLE QString getVideoPath();


    Q_INVOKABLE int getVideoX();
    Q_INVOKABLE int getVideoY();
    Q_INVOKABLE int getVideoWidth();
    Q_INVOKABLE int getVideoHeight();

    Q_INVOKABLE void loadVideo();
    Q_INVOKABLE bool isVideoAvailable();



protected:

    QtJson::JsonObject result;
    DownloadManager downloadManager;
    FtpManager ftpmanager;
    int scene_version;
    int id_scene;
    bool video_available;
    bool wipeout_cache;
    QQuickView view;
    QString local_file_path;
    QString name;


    QString getMacAddress();
    void setNameFromIni();

    
signals:
    void videoLoaded(bool success);
    void percentageDownloadedChanged(int percent);

    
public slots:
    void videoDownloaded(bool);
    void percentageDownloaded(int);
    void initQml();
};

#endif // QMLMANAGER_H
