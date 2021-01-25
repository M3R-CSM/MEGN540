# INSTALL ROS STUFF

#sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
#sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654

#sudo apt-get update
#sudo apt-get upgrade

#sudo apt install -y python-rosdep python-rosinstall-generator python-wstool python-rosinstall build-essential cmake

#sudo rosdep init
#rosdep update

#mkdir -p ~/ros_catkin_ws
#cd ~/ros_catkin_ws

#rosinstall_generator ros_comm --rosdistro melodic --deps --wet-only --tar > melodic-ros_comm-wet.rosinstall
#wstool init src melodic-ros_comm-wet.rosinstall
#rosinstall_generator desktop --rosdistro melodic --deps --wet-only --tar > melodic-desktop-wet.rosinstall
#wstool init src melodic-desktop-wet.rosinstall
#cd ~/ros_catkin_ws
#rosdep install -y --from-paths src --ignore-src --rosdistro melodic -r --os=debian:buster
#sudo ./src/catkin/bin/catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/melodic

#cd ~/ros_catkin_ws
#rosinstall_generator ros_comm ros_control joystick_drivers --rosdistro melodic --deps --wet-only --tar > melodic-custom_ros.rosinstall
#wstool merge -t src melodic-custom_ros.rosinstall
#wstool update -t src
#rosdep install --from-paths src --ignore-src --rosdistro melodic -y -r --os=debian:buster
#sudo ./src/catkin/bin/catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/melodic

#source /opt/ros/melodic/setup.bash
#echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc