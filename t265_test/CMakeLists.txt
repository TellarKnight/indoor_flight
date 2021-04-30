cmake_minimum_required(VERSION 3.0.2)
project(t265_test)

find_package(Eigen3 REQUIRED)
find_package(realsense2 REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
        tf
)

catkin_package(
        INCLUDE_DIRS include
        CATKIN_DEPENDS  message_runtime
)



include_directories(
  /home/x/vision_ws1/src/t265_test/include
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
)

#######################t265_tracker###########################
add_executable(rs_pose src/rs_pose.cpp)
target_link_libraries(rs_pose ${catkin_LIBRARIES} ${realsense2_LIBRARY})

add_executable(t265_px4_pos_send src/t265_px4_pos_send.cpp)
target_link_libraries(t265_px4_pos_send ${catkin_LIBRARIES})

