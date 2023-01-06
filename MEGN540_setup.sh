# Get System Updates and Upgrades
sudo apt update
sudo apt upgrade -y

# INSTALL PROGRAMMING APPS & LIBS: GIT (version control), MELD (file differences), code (Visual Studio Code), AND AVRDUDE (avr programming)
sudo apt install -y build-essential git meld gcc-avr avr-libc avrdude code cmake clang-format-10 doxygen

# INSTALL VSCODE Extensions
code --install-extension ms-vscode.cpptools 
code --install-extension ms-vscode.cpptools-extension-pack 
code --install-extension jeff-hykin.better-cpp-syntax 
code --install-extension ms-vscode.cpptools-themes 
code --install-extension xaver.clang-format 
code --install-extension twxs.cmake 
code --install-extension ms-vscode.cmake-tools 
code --install-extension cschlosser.doxdocgen



# INSTALL SCREEN CAPTURE APPS: - This allows you to record the desktop with audio for assignents
sudo apt install -y ffmpeg vokoscreen

# CLONE CLASS REPOS AND SETUP FILE SYSTEM
cd ~/Desktop
git clone https://github.com/M3R-CSM/MEGN540.git
cd MEGN540
git submodule update --init --recursive
mkdir BIN
bash SerialMonitor/install_dep.sh
cd ..

# Remove Mode Manager Which Causes Issues with programming
sudo apt-get purge -y modemmanager
sudo apt-get autoremove -y

# SETUP UDEV RULES FOR ZUMO
sudo sh -c 'echo "ACTION==\"add\", SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"1ffb\", ATTRS{idProduct}==\"2300\", SYMLINK+=\"ttyZumoCar\"
ACTION==\"add\", SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"1ffb\", ATTRS{idProduct}==\"0101\", SYMLINK+=\"ttyZumoCarAVR\"
SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"1ffb\", GROUP=\"dialout\", MODE=\"0666\" " > /etc/udev/rules.d/99-zumo.rules'

sudo udevadm trigger
