#!/bin/bash
sudo apt-get install apache2
sudo sh PiCam_server/RPi_Cam_Web_Interface_Installer.sh
dialog --inputbox "Enter path to PiCam Server:\n(eg:- http://localhost/piviewer)" 8 40 2>temp
ip=$(cat temp)

sed '93 c \          piImage.src = "http://localhost/'$ip'/cam_pic.php?time=" + new Date().getTime();' ./VR_server/index.html

