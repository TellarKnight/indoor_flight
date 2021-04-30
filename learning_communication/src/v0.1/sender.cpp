//
// Created by k on 2021/2/1.
//

//
// Created by zhangcheng on 2020/8/25.
//

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <learning_communication/OffboardState.h>
#include"std_msgs/Float64MultiArray.h"

using namespace std;

int main(int argc,char **argv)
{
    int signal;
    int index_0;
    int index_1;
    int index_2;
    int index_3;
    int index_4;
    int index_5;
    int index_6;
    int index_7;
    int index_8;
    int index_9;

    std_msgs::Float64MultiArray track_1;

//    std_msgs::Float64MultiArray track_uav;
//    std_msgs::Float64MultiArray track_u1;
//    std_msgs::Float64MultiArray track_u2;
//    std_msgs::Float64MultiArray track_u3;
//    std_msgs::Float64MultiArray track_u4;
//    std_msgs::Float64MultiArray track_u5;
//    std_msgs::Float64MultiArray track_u6;
//    std_msgs::Float64MultiArray track_u7;
//    std_msgs::Float64MultiArray track_u8;
//    std_msgs::Float64MultiArray track_u9;

    double track [10][2];
    track[0][0] = -6.5;track[0][1] = 3;
    track[1][0] = 6.5;track[1][1] = 3;
    track[2][0] = -6.5;track[2][1] = 8.5;
    track[3][0] = 6.5;track[3][1] = 8.5;
    track[4][0] = -6.5;track[4][1] = 14;
    track[5][0] = 6.5;track[5][1] = 14;
    track[6][0] = -6.5;track[6][1] = 18.5;
    track[7][0] = 6.5;track[7][1] = 18.5;
    track[8][0] = -6.5;track[8][1] = 24;
    track[9][0] = 6.5;track[9][1] = 24;

    double track_all [10][2];

    ros::init(argc,argv,"sender");
    ros::NodeHandle sender;

//    ros::Subscriber state_sub = sender.subscribe<mavros_msgs::State>("mavros/state",10,state_cb);
    ros::Publisher track_uav = sender.advertise<std_msgs::Float64MultiArray>("track",10);
//    ros::Publisher track_1 = sender.advertise<std_msgs::Float64MultiArray>("uav1/track",10);
//    ros::Publisher track_2 = sender.advertise<std_msgs::Float64MultiArray>("uav2/track",10);
//    ros::Publisher track_3 = sender.advertise<std_msgs::Float64MultiArray>("uav3/track",10);
//    ros::Publisher track_4 = sender.advertise<std_msgs::Float64MultiArray>("uav4/track",10);
//    ros::Publisher track_5 = sender.advertise<std_msgs::Float64MultiArray>("uav5/track",10);
//    ros::Publisher track_6 = sender.advertise<std_msgs::Float64MultiArray>("uav6/track",10);
//    ros::Publisher track_7 = sender.advertise<std_msgs::Float64MultiArray>("uav7/track",10);
//    ros::Publisher track_8 = sender.advertise<std_msgs::Float64MultiArray>("uav8/track",10);
//    ros::Publisher track_9 = sender.advertise<std_msgs::Float64MultiArray>("uav9/track",10);

    ros::Rate rate(2.0);
    ROS_INFO("init");

    cout << "0 uav target";
    cin >> index_0;
//    track_all[0][0] = track[index_0][0];
//    track_all[0][1] = track[index_0][1];
//    track_u0.data.push_back(track[index_0][0]);
//    track_u0.data.push_back(track[index_0][1]);
//    track_0.publish(track_u0);

    cout << "1 uav target";
    cin >> index_1;
//    track_all[1][0] = track[index_1][0];
//    track_all[1][1] = track[index_1][1];
//    track_u1.data.push_back(track[index_1][0]);
//    track_u1.data.push_back(track[index_1][1]);
//    track_1.publish(track_u1);

    cout << "2 uav target";
    cin >> index_2;
//    track_all[2][0] = track[index_2][0];
//    track_all[2][1] = track[index_2][1];
//    track_u2.data.push_back(track[index_2][0]);
//    track_u0.data.push_back(track[index_2][1]);
//    track_2.publish(track_u2);

    cout << "3 uav target";
    cin >> index_3;
//    track_all[3][0] = track[index_3][0];
//    track_all[3][1] = track[index_3][1];
//    track_u3.data.push_back(track[index_3][0]);
//    track_u3.data.push_back(track[index_3][1]);
//    track_3.publish(track_u3);

    cout << "4 uav target";
    cin >> index_4;
//    track_all[4][0] = track[index_4][0];
//    track_all[4][1] = track[index_4][1];
//    track_u4.data.push_back(track[index_4][0]);
//    track_u4.data.push_back(track[index_4][1]);
//    track_4.publish(track_u4);

    cout << "5 uav target";
    cin >> index_5;
//    track_all[5][0] = track[index_5][0];
//    track_all[5][1] = track[index_5][1];
//    track_u5.data.push_back(track[index_5][0]);
//    track_u5.data.push_back(track[index_5][1]);
//    track_5.publish(track_u5);

    cout << "6 uav target";
    cin >> index_6;
//    track_all[6][0] = track[index_6][0];
//    track_all[6][1] = track[index_6][1];
//    track_u6.data.push_back(track[index_6][0]);
//    track_u6.data.push_back(track[index_6][1]);
//    track_6.publish(track_u0);

    cout << "7 uav target";
    cin >> index_7;
//    track_all[7][0] = track[index_7][0];
//    track_all[7][1] = track[index_7][1];
//    track_u7.data.push_back(track[index_7][0]);
//    track_u7.data.push_back(track[index_7][1]);
//    track_7.publish(track_u7);

    cout << "8 uav target";
    cin >> index_8;
//    track_all[8][0] = track[index_8][0];
//    track_all[8][1] = track[index_8][1];
//    track_u8.data.push_back(track[index_8][0]);
//    track_u8.data.push_back(track[index_8][1]);
//    track_8.publish(track_u8);

    cout << "9 uav target";
    cin >> index_9;
//    track_all[9][0] = track[index_9][0];
//    track_all[9][1] = track[index_9][1];
//    track_u9.data.push_back(track[index_9][0]);
//    track_u9.data.push_back(track[index_9][1]);
//    track_9.publish(track_u9);
//    track_1.publish(track[index_1]);
//    track_2.publish(track[index_2]);
//    track_3.publish(track[index_3]);
//    track_4.publish(track[index_4]);
//    track_5.publish(track[index_5]);
//    track_6.publish(track[index_6]);
//    track_7.publish(track[index_7]);
//    track_8.publish(track[index_8]);
//    track_9.publish(track[index_9]);

    track_1.data.push_back(index_0);
    track_1.data.push_back(index_1);
    track_1.data.push_back(index_2);
    track_1.data.push_back(index_3);
    track_1.data.push_back(index_4);
    track_1.data.push_back(index_5);
    track_1.data.push_back(index_6);
    track_1.data.push_back(index_7);
    track_1.data.push_back(index_8);
    track_1.data.push_back(index_9);


    while (ros::ok()){
        track_uav.publish(track_1);
//        track_1.publish(track_u1);
//        track_2.publish(track_u2);
//        track_3.publish(track_u3);
//        track_4.publish(track_u4);
//        track_5.publish(track_u5);
//        track_6.publish(track_u6);
//        track_7.publish(track_u7);
//        track_8.publish(track_u8);
//        track_9.publish(track_u9);

        ros::spinOnce();
        rate.sleep();
    }


    ros::spinOnce();
    rate.sleep();

    return 0;
}
