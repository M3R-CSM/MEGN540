# Download CLion Tarball
wget -O CLion-2020.3.1.tar.gz https://download.jetbrains.com/cpp/CLion-2020.3.1.tar.gz

# Unzip taball 
tar -xf CLion-2020.3.1.tar.gz 

# Move to  opt folder
sudo mv clion-2020.3.1/ /opt/

# cleanup download 
rm CLion-2020.3.1.tar.gz 

# update .bashrc so it can launch from any folder path without headach
echo 'export PATH=/opt/clion-2020.3.1/bin:$PATH' >> ~/.bashrc
. ~/.bashrc  # reloads bashrc file

# update to add to launcher
sudo sh -c 'echo "[Desktop Entry]
Type=Application
Name=CLion
Comment=CLion Integrated Development Environment
Icon=/opt/clion-2020.3.1/bin/clion.png
Exec=/opt/clion-2020.3.1/bin/clion.sh
Terminal=false
Categories=Development;IDE;" > /usr/share/applications/clion.desktop'


#remove eclipse
sudo apt remove -y eclipse
sudo rm /usr/share/applications/eclipse.desktop
