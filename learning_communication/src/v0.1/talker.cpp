//
// Created by zhangcheng on 2020/8/19.
//
#include <ros/ros.h>
#include <sstream>
#include <std_msgs/String.h>


int main(int argc,char **argv)
{
    ros::init(argc,argv,"talker");
    ros::NodeHandle nh;

    ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter",1000);
    ros::Rate loop_rate(10);

    int count = 0;
    while (ros::ok())
    {
        std_msgs::String msg;
        std::stringstream ss;
        ss << "hello world" << count;
        msg.data = ss.str();

        ROS_INFO("%s",msg.data.c_str());
        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
}