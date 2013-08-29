import QtQuick 2.0
import QtMultimedia 5.0
import QtWebKit 3.0

Item {
    id: screen

    Component.onCompleted: {

        web.url = manager.getUrl();
        videoContainer.x = manager.getVideoX();
        videoContainer.y = manager.getVideoY()
        videoContainer.width = manager.getVideoWidth();
        videoContainer.height = manager.getVideoHeight();


        if(manager.isVideoAvailable()){
            video.source= manager.getVideoPath();
            video.play()
            pendingVideo.destroy()
        }


    }
    Connections {
        target: manager
        onPercentageDownloadedChanged:{
            pendingVideo.percent = percent + "%";
        }

        onVideoLoaded: {
            if(success){
                console.log("DATA loaded")
                video.source= manager.getVideoPath();

                video.play()
            }else{
                textError.text = "Couldn't download the video!"
                console.log("DATA NOT LOADED")
            }

            pendingVideo.destroy()
        }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        property int current: 0
        z: 33
        onClicked: {
            if(current === 0){
                current = 1;
                video.source = "";
                video.play();
            }else{
                current = 0;
                video.source = "../videos/4.mov";
                video.play();
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"






        WebView{
            id:web
            x: 0
            y: 0
            width: parent.width
            height: parent.height

            onLoadingChanged: {
                    if (loadRequest.status === WebView.LoadSucceededStatus)
                        pendingWebView.destroy()
                }

            Pending{
                id: pendingWebView
                z:10
            }
        }

        Rectangle{
            id:videoContainer
            color: "#333333"
            Video{
                z:3
                id:video
                anchors.fill: parent
                autoLoad: false

                onStopped: {
                    video.source = "";
                    video.source = manager.getVideoPath();
                    video.play();
                }

                Pending{
                    id: pendingVideo
                    z:21
                }
                Text {
                    anchors.centerIn: parent
                    id: textError
                    text: ""
                    font.pointSize: 24
                    color: "gray"
                }
            }
        }
    }



}
