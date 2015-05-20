<<<<<<< HEAD
1. Install Gazebo and necessary dependencies
	> libgazebo_ros_control plugin
	> ros controller-manager
2. `cd remote_ws`
3. `catkin_make`
4. `source devel/setup.bash`
5. `rospack profile`
=======
1. Install Gazebo and necessary dependencies	//To install models for gazebo
2. To install models use: hg clone https://bitbucket.org/osrf/gazebo_models
3. Then copy these models to ~/.gazebo/models
4. cd remote_ws
5. catkin_make
6. source devel/setup.bash
7. rospack profile
>>>>>>> 2b80ca27e3341722cc3eae535ef3d21eb5070869

Launch the empty world and control the robot
---------------------------------------------

1. roslaunch sangi_gazebo sangi_world.launch
2. rosrun turtlesim turtle_teleop_key /turtle1/cmd_vel:=/sangi/cmd_vel
