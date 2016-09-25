sudo chmod a+rw /dev/input/js0
roscore &
sleep 2
rosparam set joy_node/dev "/dev/input/js0"
rosrun joy joy_node &
sleep 2
rostopic echo joy
