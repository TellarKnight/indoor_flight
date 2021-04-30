cmake_minimum_required(VERSION 3.0.2)
project(learning_communication)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
set(catkin_DIR /opt/ros/melodic/share/catkin/cmake/)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  geometry_msgs
)

add_message_files(
  FILES
  OffboardState.msg
#   Message2.msg
)


## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_package(
 # INCLUDE_DIRS include
#  LIBRARIES learning_communication
 CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
)
include_directories(
        /home/x/vision_ws1/src/learning_communication/include
        /home/x/catkin_ws/devel/include
        /home/k/code/rosproject/src/learning_communication/include
        /home/k/catkin_ws/devel/include

  ${catkin_INCLUDE_DIRS}
)


add_executable(talker src/v0.1/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})

add_executable(listener src/v0.1/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})

add_executable(pos_controller src/v0.1/pos_controller.cpp)
target_link_libraries(pos_controller ${catkin_LIBRARIES})

add_executable(sender src/v0.1/sender.cpp)
target_link_libraries(sender ${catkin_LIBRARIES})

add_executable(offboard src/v0.1/offboard.cpp)
target_link_libraries(offboard ${catkin_LIBRARIES})

add_executable(udpreceiver src/v0.1/udpreceiver_v0.1.cpp)
target_link_libraries(udpreceiver ${catkin_LIBRARIES})




