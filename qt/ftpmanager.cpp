#include "ftpmanager.h"



FtpManager::FtpManager()
{
    m_MStateMap.insert(pair<int,QString>(0,"Unconnected"));
    m_MStateMap.insert(pair<int,QString>(1,"Host Lookup"));
    m_MStateMap.insert(pair<int,QString>(2,"Connecting"));
    m_MStateMap.insert(pair<int,QString>(3,"Connected"));
    m_MStateMap.insert(pair<int,QString>(4,"Login"));
    m_MStateMap.insert(pair<int,QString>(5,"Closing"));

    connect(this,SIGNAL(stateChanged(int)),this,SLOT(connectionStateChanged(int)));
    connect(this,SIGNAL(listInfo(QUrlInfo)),this,SLOT(listInfo(QUrlInfo)));
    connect(this,SIGNAL(readyRead()),this,SLOT(saveData()));
    connect(this,SIGNAL(commandStarted(int)), this,SLOT(commandStarted(int)));
    connect(this,SIGNAL(commandFinished(int,bool)),this,SLOT(commandFinished(int,bool)));
    connect(this,SIGNAL(dataTransferProgress(qint64,qint64))
            ,this,SLOT(viewProgress(qint64,qint64)));

    get_command = -1;
    getsize_command = -1;
}

void FtpManager::downloadFile(QUrl url, QString local_file_path){
    this->url = url;
    qDebug() << url.port();
    if(url.port() != -1){
        connectToHost(url.host(), url.port());
    }else{
        connectToHost(url.host());
    }

    login(url.userName(), url.password());



    file = new QFile(local_file_path);
    if(file->exists()){
        cout << "file already there "<< local_file_path.toStdString() << endl;
       this->getsize_command = this->list(url.path());
    }else{
        beginDownload();
    }
}

void FtpManager::beginDownload(){
    if(file->open(QIODevice::WriteOnly| QIODevice::Append)){
        QString path = url.path();
        path.remove(0, 1);
        qDebug() << "ok : dl " << path;
        get_command = get(path);

    }else{
        cout << "impossible d ouvrir le fichier " << endl;
    }
}

void FtpManager::checkFileSize(int ftp_file_size){

    cout << "file " << file->size() <<endl ;
    if(ftp_file_size != file->size()){
        file->remove();
        beginDownload();
    }else{
        emit finished(true);
    }


}

void FtpManager::connectionStateChanged(int state)
{
    cout<<"Connection State Changed->"
       << state<<" Status: "
       <<m_MStateMap.find(state)->second.toStdString()
      <<endl;
}

void FtpManager::viewProgress(qint64 done ,qint64 total)
{
    int percent = (double)done / (double)total * (double)100;
//    cout << "percent " << percent << endl;
    emit percentageDownloaded(percent);
}

void FtpManager::listInfo(QUrlInfo info)
{
    if(this->getsize_command){
        cout<<"Listing URL Size:"<<info.size()<<endl;
        checkFileSize(info.size());
    }else{
        cout<<"Listing URL Info:" << info.name().toStdString() <<endl;
    }
}

void FtpManager::commandFinished(int id,bool error)
{
    cout<<"Command Finished! Id:"<<id<<"-Status:"<<error<<endl;
    if(id == get_command){
        cout << "dl over"<<endl;
        if(error){
            file->remove();
            cout << "problem during the dl"<<endl;
        }

        emit finished(!error);
        file->close();
    }
}
void FtpManager::commandStarted(int id)
{
    cout<<"Command Started! Id:"<<id<<"-Status:"<<endl;
    if(id == get_command){
        cout << "dl begin"<<endl;
    }
}

void FtpManager::readData()
{
    cout<<"Read File Content-->"<<endl
       <<QString(readAll()).toStdString()
      <<endl
     <<"Read File END "<<endl;
}


void FtpManager::saveData()
{
    mutex.lock();
    file->write(readAll());
    mutex.unlock();
}
