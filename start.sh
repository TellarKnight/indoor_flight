
gnome-terminal --window -e 'bash -c "source /opt/ros/melodic/setup.bash; roscore; exec bash"' \
--tab -e 'bash -c "sleep 10; source /home/x/catkin_ws/devel/setup.bash; roslaunch mavros px4.launch; ecec bash"' \
--tab -e 'bash -c "sleep 10; source /opt/ros/melodic/setup.bash; roslaunch realsense2_camera rs_t265.launch; exec bash"' \
--tab -e 'bash -c "sleep 10; source /home/x/vision_ws1/devel/setup.bash; rosrun t265_test t265_px4_pos_send; exec bash"' \
--tab -e 'bash -c "sleep 10; source /home/x/vision_ws1/devel/setup.bash; rosrun local_pos_send send_pos5; exec bash"' \
--tab -e 'bash -c "sleep 10; source /home/x/vision_ws1/devel/setup.bash; rosrun learning_communication udpreceiver; exec bash"' 
#roslaunch mavros px4.launch fcu_url:="/dev/ttyTHS1:921600"

# start x11vnc
gnome-terminal --window -e 'bash -c "x11vnc -forever -shared -rfbauth ~/.vnc/passwd; exec bash"' \
