#include "connectionmanager.h"



ConnectionManager::ConnectionManager(QObject *parent) :
    QObject(parent)
{
    this->video_available = false;
    this->scene_version = -1;
    this->id_scene = -1;
    this->wipeout_cache = false;

    setNameFromIni();


    view.rootContext()->setContextProperty("manager", this);
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QTimer* timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(initQml()));
    initQml();
    timer->start(1000*60);

}


ConnectionManager::StateCode ConnectionManager::requestData(){
    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the eventl-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("http://192.168.1.144/PiSignage/device_register") ) );

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray array;
    array.append("mac=" + getMacAddress());
    array.append("&name=" + this->name);
    array.append("&disk_available=" + QString::number(FileManager::getFreeDiskSpace() ));

    QNetworkReply *reply = mgr.post(req, array);
    //      QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        // Everything is ok => reply->readAll()
        QString str(reply->readAll());
        qDebug() << str;
        bool ok;
        // json is a QString containing the JSON data
        QtJson::JsonObject result = QtJson::parse(str, ok).toMap();

        if(!ok) {
            qFatal("An error occurred during parsing");
            delete reply;
            return ConnectionManager::ERROR;
        }else{
            qDebug() << "url:" << result["url"].toString();
            this->result = result;
            delete reply;
            if(this->result["id"].toInt() == -1){
                return ConnectionManager::EMPTY_SCENE;
            }else{
                this->wipeout_cache = this->result["wipeout_cache"].toBool();
                if(this->id_scene != this->result["id"].toInt()
                        || this->scene_version != this->result["version"].toInt()
                        || this->wipeout_cache)
                {
                    this->id_scene = this->result["id"].toInt();
                    this->scene_version = this->result["version"].toInt();


                    return ConnectionManager::REFRESH;
                }else{
                    return ConnectionManager::PASS;
                }
            }
        }
    }
    else {
        qDebug() << reply->errorString();
        delete reply;
        return ConnectionManager::ERROR;
    }
}

void ConnectionManager::loadVideo(){

    QUrl video_url(this->result["video"].toString());

    QFileInfo fi(video_url.path());
    local_file_path = QString(QCoreApplication::applicationDirPath() + "/videos/%1.%2")
            .arg(this->result["id_video"].toInt())
            .arg(fi.suffix());

    qDebug() << "url scheme " << video_url.scheme();
    if(video_url.scheme() == "http"){
        connect(&downloadManager, SIGNAL(finished()), this, SLOT(videoDownloaded()));
        connect(&downloadManager, SIGNAL(percentageDownloaded(int)), this, SLOT(percentageDownloaded(int)));
        downloadManager.downloadFile(video_url, local_file_path);
    }else{
        connect(&ftpmanager, SIGNAL(finished(bool)), this, SLOT(videoDownloaded(bool)));
        qDebug() << "ftp download ";
        connect(&ftpmanager, SIGNAL(percentageDownloaded(int)), this, SLOT(percentageDownloaded(int)));
        ftpmanager.downloadFile(video_url, local_file_path);
    }
}

void ConnectionManager::percentageDownloaded(int percent){
    if(!video_available){
        emit percentageDownloadedChanged(percent);
    }

}

void ConnectionManager::videoDownloaded(bool success){
    qDebug() << "FINI";
    this->video_available = success;
    emit videoLoaded(success);
}

QString ConnectionManager::getMacAddress()
{
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            return netInterface.hardwareAddress();
    }
    return QString();
}



QString ConnectionManager::getVideoPath(){
    return local_file_path;
}



QString ConnectionManager::getUrl(){
    return this->result["url"].toString();
}

int ConnectionManager::getVideoX(){
    return this->result["x"].toInt();
}
int ConnectionManager::getVideoY(){
    return this->result["y"].toInt();
}
int ConnectionManager::getVideoWidth(){
    return this->result["width"].toInt();
}
int ConnectionManager::getVideoHeight(){
    return this->result["height"].toInt();
}

bool ConnectionManager::isVideoAvailable(){
    return this->video_available;
}

void ConnectionManager::initQml(){
    ConnectionManager::StateCode status = requestData();
    switch (status) {
    case ConnectionManager::EMPTY_SCENE:
        view.setSource(QUrl(QCoreApplication::applicationDirPath() + "/qml/default.qml"));
        break;

    case ConnectionManager::ERROR:
        view.setSource(QUrl(QCoreApplication::applicationDirPath() + "/qml/error.qml"));
        break;

    case ConnectionManager::REFRESH:
        this->view.engine()->clearComponentCache();
        if(this->wipeout_cache){
            FileManager::clearCache();
        }
        loadVideo();
        view.setSource(QUrl(QCoreApplication::applicationDirPath() + "/qml/scene.qml"));
        break;
    case ConnectionManager::PASS:
        qDebug() << "nothing to do";

    default:
        break;
    }

    this->view.show();
}

void ConnectionManager::setNameFromIni(){
    QSettings settings("conf.ini", QSettings::IniFormat);
    this->name = settings.value("name", "default name").toString();
}
