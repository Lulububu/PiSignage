TEMPLATE = app

target.path = /home/pi/pisignage
TARGET = pisignage
INSTALLS += target

DEPENDPATH += .
INCLUDEPATH += .

QT += quick

SOURCES += main.cpp \
    json.cpp \
    downloadmanager.cpp \
    connectionmanager.cpp \
    ftpmanager.cpp \
    qftp.cpp \
    qurlinfo.cpp \
    filemanager.cpp
INCLUDEPATH += /mnt/rasp-pi-rootfs/usr/local/qt5pi/include/
qml_folder.source = qml
res_folder.source = res
DEPLOYMENTFOLDERS = qml_folder
DEPLOYMENTFOLDERS += res_folder

OTHER_FILES += \
    qml/* \
    res/* \
    qml/scene.qml \
    conf.ini

for(deploymentfolder, DEPLOYMENTFOLDERS) {
    item = item$${deploymentfolder}
    itemsources = $${item}.sources
    $$itemsources = $$eval($${deploymentfolder}.source)
    itempath = $${item}.path
    $$itempath= $$eval($${deploymentfolder}.target)
    export($$itemsources)
    export($$itempath)
    DEPLOYMENT += $$item
}

installPrefix = /home/pi/$${TARGET}

for(deploymentfolder, DEPLOYMENTFOLDERS) {
    item = item$${deploymentfolder}
    itemfiles = $${item}.files
    $$itemfiles = $$eval($${deploymentfolder}.source)
    itempath = $${item}.path
    $$itempath = $${installPrefix}/$$eval($${deploymentfolder}.target)
    export($$itemfiles)
    export($$itempath)
    INSTALLS += $$item
}

HEADERS += \
    json.h \
    downloadmanager.h \
    connectionmanager.h \
    ftpmanager.h \
    qftp.h \
    qurlinfo.h \
    filemanager.h
