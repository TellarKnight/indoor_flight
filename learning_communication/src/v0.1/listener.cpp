//
// Created by zhangcheng on 2020/8/19.
//
#include <ros/ros.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/AttitudeTarget.h>
#include <mavros_msgs/PositionTarget.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>

using namespace std;

mavros_msgs::State current_state;
geometry_msgs::PoseStamped current_position;

void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state = *msg;
//    cout << "Armed:" << current_state.armed << " Mode:" << current_state.mode << endl;
}

void position_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    current_position = *msg;
    cout << "Pos_fcu [X Y Z] : " << current_position.pose.position.x << " [ m ] "<< current_position.pose.position.y <<" [ m ] "<< current_position.pose.position.z <<" [ m ] "<<endl;
}

void printf_info()
{
    cout.setf(ios::fixed);
    //setprecision(n) 设显示小数精度为n位
    cout<<setprecision(2);
    //左对齐
    cout.setf(ios::left);
    // 强制显示小数点
    cout.setf(ios::showpoint);
    // 强制显示符号
    cout.setf(ios::showpos);



}

int main(int argc,char **argv)
{
    ros::init(argc,argv,"listener");
    ros::NodeHandle nh;
    printf_info();

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state",10,state_cb);
    ros::Subscriber position_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose",10,position_cb);



    ros::spin();
    return 0;
}


