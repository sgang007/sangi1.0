1. Install Gazebo and necessary dependencies
	> libgazebo_ros_control plugin
	> ros controller-manager
2. `cd remote_ws`
3. `catkin_make`
4. `source devel/setup.bash`
5. `rospack profile`

Launch the empty world and control the robot
---------------------------------------------

1. roslaunch sangi_gazebo sangi_world.launch
2. rosrun turtlesim turtle_teleop_key /turtle1/cmd_vel:=/sangi/cmd_vel
