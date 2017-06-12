#!/usr/bin/env bash
set -euo pipefail

#Check if script is being run as root
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

if [ ! $? = 0 ]; then
   exit 1
else
  #Installs packages which might be missing
   apt-get install -y git whiptail

   # enable I2C
   raspi-config nonint do_i2c 0

   git clone https://github.com/PiSupply/Flick.git
   cd Flick/flick
   python setup.py install
   whiptail --msgbox "The system will now reboot" 8 40
   reboot
fi
