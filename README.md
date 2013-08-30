PiSignage
=========

Digital signage on a raspberry Pi with QT5 and accelerated hardware use for video.


#Introduction
This project is more about discovering the possibilites of media managenement on the raspberry Pi than creating a complete digital signage solution. Hopefully, it will eventually become a complete software with nice management features for the user.

Right now the program is working but contains too much issues to be used in production. Most of the issues is about the video player on the raspberry Pi but there is some really talented people currently working on the raspberry Pi, I am sure this will be resolved soon.

#Strucuture
The solution is divided in two parts, the client part and the server part.

##Client
The client part is designed to run on a raspberry Pi but can be used on any device you can compiled on with Qt. The program is a full screen display which will request its *scene* from the server, download the required content and then display it. This is a Qt 5 program using qml and qtvideo, qtwebkit module.

##Server
The server is where you can decide what each of your client device should display. This a web application written in PHP. After installation you can create some *scene* where you can choose the video you want to play and a web page you want to display in the background. You must provide an HTTP or FTP link for the video, this program doesn't handle the upload. You have a list of every registered device and you can assign a different scene to each of them. 

#How to build
##Client
If you want to build the client part on the raspberry Pi, you have to compile Qt on the raspberry Pi. One of the issues is that the qtvideo module doesn't use the hardware acceleration on the PI. Lucas Carlon find a way to use the hardware acceleration, you can see his work [here](http://thebugfreeblog.blogspot.co.uk/2013/08/raspberry-pi-wheezy-image-with-openmax.html). You must follow his instructions then you will be able to compile the client part thanks Qtcreator. Just open the .pro file contained in the qt folder and you will be able to compile in on your pi.

##Server
The server is a simple website with a mysql database. Make sure the www folder is in your apache folder and create the required database thanks to the SQL file. Then just configure the name and the access of the database in the www/app/config.php file. 

#how to use
##Client
Once your program is compiled, you can just customize the conf.ini file on your computer to gives your raspberry Pi a name. (ongoing: you will soon be able to change the server IP address in this file, right now you have to change it in the code). You can now run this program on your PI and it will connect to the server side.

##Server
Once your website is running, just connect to it with a simple webbrowser and create scenes. If you launch the client program on the PI, you will be able to see the device on the devices list with its name, IP and MAC address.

This is it, when your program will receive the new scene it will refresh and display what you decided.

#Bugs
Right now the qtvideo module is not fully operational and suffers many bug. This is why the video loop is not working well and the client refreshing will ended in a crash. You would better want to set up the scene for a device and restart the raspberry PI after this.
