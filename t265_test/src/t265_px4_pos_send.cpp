/***************************************************************************************************************************
 * px4_pos_estimator.cpp
 *
 * Author: Qyp
 *
 * Update Time: 2019.3.10
 *
 * 说明: mavros位置估计程序
 *      1. 订阅激光SLAM (cartorgrapher_ros节点) 发布的位置信息,从laser坐标系转换至NED坐标系
 *      2. 订阅t265设备 (vision_to_mavros节点) 发布的位置信息
 *      3. 订阅飞控发布的位置、速度及欧拉角信息，作对比用
 *      4. 存储飞行数据，实验分析及作图使用
 *      5. 选择激光SLAM或者t265设备作为位置来源，发布位置及偏航角(xyz+yaw)给飞控
 *
***************************************************************************************************************************/
/***************************************************************************************************************************
* px4_pos_controller.cpp
*
* Author: Qyp
*
* Update Time: 2019.3.16
*
* Introduction:  PX4 Position Estimator using external positioning equipment
*         1. Subscribe position and yaw information from Lidar SLAM node(cartorgrapher_ros节点), transfrom from laser frame to ENU frame
*         2. Subscribe position and yaw information from Vicon node(vrpn-client-ros节点), transfrom from vicon frame to ENU frame
*         3. Send the position and yaw information to FCU using Mavros package (/mavros/mocap/pose or /mavros/vision_estimate/pose)
*         4. Subscribe position and yaw information from FCU, used for compare
***************************************************************************************************************************/


//头文件
#include <ros/ros.h>

#include <Eigen/Eigen>

#include <tf2_msgs/TFMessage.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/TransformStamped.h>
using namespace std;

//---------------------------------------vision vio定位相关------------------------------------------
Eigen::Vector3d pos_drone_vio;                          //无人机当前位置 (vision)
Eigen::Quaterniond q_vio;
//---------------------------------------laser定位相关------------------------------------------

geometry_msgs::TransformStamped laser;                          //当前时刻cartorgrapher发布的数据

//---------------------------------------发布相关变量--------------------------------------------
ros::Publisher vision_pub;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>函数声明<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void printf_info();                                                                       //打印函数
void send_to_fcu();
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>回调函数<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void vision_cb(const tf2_msgs::TFMessage msg_tf)
{
    pos_drone_vio[0] = msg_tf.transforms[0].transform.translation.x;
    pos_drone_vio[1] = msg_tf.transforms[0].transform.translation.y;
    pos_drone_vio[2] = msg_tf.transforms[0].transform.translation.z;

    q_vio.x() = msg_tf.transforms[0].transform.rotation.x;
    q_vio.y() = msg_tf.transforms[0].transform.rotation.y;
    q_vio.z() = msg_tf.transforms[0].transform.rotation.z;
    q_vio.w() = msg_tf.transforms[0].transform.rotation.w;

}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>主 函 数<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int main(int argc, char **argv)
{
    ros::init(argc, argv, "t265_px4_pos_send");
    ros::NodeHandle nh("~");

    // 【订阅】vision估计位置
    ros::Subscriber vision_sub = nh.subscribe<tf2_msgs::TFMessage>("/tf", 1000, vision_cb);

    // 【发布】无人机位置和偏航角 坐标系 ENU系
    //  本话题要发送飞控(通过mavros_extras/src/plugins/vision_pose_estimate.cpp发送), 对应Mavlink消息为VISION_POSITION_ESTIMATE(#??), 对应的飞控中的uORB消息为vehicle_vision_position.msg 及 vehicle_vision_attitude.msg
    vision_pub = nh.advertise<geometry_msgs::PoseStamped>("/mavros/vision_pose/pose", 1000);


    // 频率
    ros::Rate rate(10.0);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Main Loop<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    while(ros::ok())
    {
        //回调一次 更新传感器状态
        ros::spinOnce();

        // 将定位信息及偏航角信息发送至飞控，根据参数flag_use_laser_or_vicon选择定位信息来源
        send_to_fcu();

        // 打印
        rate.sleep();
    }

    return 0;

}


void send_to_fcu()
{
    geometry_msgs::PoseStamped vision;

    vision.pose.position.x = pos_drone_vio[0] ;
    vision.pose.position.y = pos_drone_vio[1] ;
    vision.pose.position.z = pos_drone_vio[2] ;

    vision.pose.orientation.x = q_vio.x();
    vision.pose.orientation.y = q_vio.y();
    vision.pose.orientation.z = q_vio.z();
    vision.pose.orientation.w = q_vio.w();

    vision.header.stamp = ros::Time::now();
    vision_pub.publish(vision);
}
