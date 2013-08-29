#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{
    this->download_over = false;
}

void DownloadManager::downloadFile(QUrl url, QString local_file_path){
    this->url = url;
    qDebug() << "url " <<url.toString();

    file = new QFile(local_file_path);
    if(file->exists()){
        qDebug() << "file exists";
        emit finished();
        download_over = true;
    }else{
        QNetworkRequest rqst(url);
        QNetworkReply *rply = this->m_WebCtrl.head(rqst);
        connect(rply, SIGNAL(finished()),this,
                SLOT(setFileSize()));
    }



}



void DownloadManager::setFileSize(){
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    this->length = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
    qDebug() << "length "  << this->length;
    this->downloadWholeFile();
}

void DownloadManager::downloadWholeFile(){
    int range = 2000000;
    int numberChunk = this->length / range;

    QNetworkRequest req(this->url);
    req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

    for(int i = 0; i < numberChunk; i++){
//        qDebug() << range * (i + 1);
        this->downloadChunk(req, range * i, range * (i + 1));

        QEventLoop loop;
        QObject::connect(this, SIGNAL(chunkSaved()), &loop, SLOT(quit()));
        loop.exec();
    }
    if(this->length % range != 0){
        this->downloadChunk(req, range * numberChunk, this->length);
        QEventLoop loop;
        QObject::connect(this, SIGNAL(chunkSaved()), &loop, SLOT(quit()));
        loop.exec();
    }

    qDebug() << "dl over";
    emit finished();
    download_over = true;
    file->close();
}

void DownloadManager::downloadChunk(QNetworkRequest req, int startByte, int endByte){
    QString bytes = "bytes="+
            QString::number( startByte ) + "-" +
            QString::number( endByte );
    req.setRawHeader("Range", bytes.toUtf8());
    QNetworkReply *rply = this->m_WebCtrl.get(req);
    connect(rply, SIGNAL(finished()),this,
            SLOT(saveChunk()));
    double percent = (double)endByte / (double)this->length * (double)100;
    qDebug() << endByte << "/" << this->length << "*" << 100;
    emit percentageDownloaded(percent);
}

void DownloadManager::saveChunk(){
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray m_DownloadedData = pReply->readAll();
    int size = m_DownloadedData.size();
//       qDebug() << "chunk size " << size;
    m_DownloadedData.remove(size - 1, 1);
//       qDebug() << "chunk size after " << m_DownloadedData.size();
    file->open(QIODevice::WriteOnly| QIODevice::Append);
    file->write(m_DownloadedData);


    pReply->deleteLater();

    emit chunkSaved();
}

bool DownloadManager::downloadOver(){
    return this->download_over;
}








