//
// Created by k on 2021/2/2.
//

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <learning_communication/OffboardState.h>
#include"std_msgs/Float64MultiArray.h"
#include <cstdlib>



using namespace std;

geometry_msgs::PoseStamped pose;
geometry_msgs::TwistStamped vel;
learning_communication::OffboardState current_state;
float target_x;
float target_y;
std_msgs::Float64MultiArray list_1;

void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state.mode = msg->mode;
    current_state.armed = msg->armed;
    current_state.connected = msg->connected;
}

void position_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    current_state.position[0] = msg->pose.position.x;
    current_state.position[1] = msg->pose.position.y;
    current_state.position[2] = msg->pose.position.z;
//    ROS_INFO("current height order: [%f] [m]",current_state.position[2]);
}

void target(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
    list_1.data[0] = msg->data[0];
    list_1.data[1] = msg->data[1];
    list_1.data[2] = msg->data[2];
    list_1.data[3] = msg->data[3];
    list_1.data[4] = msg->data[4];
    list_1.data[5] = msg->data[5];
    list_1.data[6] = msg->data[6];
    list_1.data[7] = msg->data[7];
    list_1.data[8] = msg->data[8];
    list_1.data[9] = msg->data[9];
}

//判断航点是否到达
int judg_dist()
{
    float delta_x;
    float delta_y;
    float delta_z;
    float dist;
    bool signal;

    delta_x = current_state.position[0]-pose.pose.position.x;
    delta_y = current_state.position[1]-pose.pose.position.y;
    delta_z = current_state.position[2]-pose.pose.position.z;
    dist = sqrt(delta_x*delta_x+delta_y*delta_y+delta_z*delta_z);

    if (dist < 0.1)
    {
        signal = 1;
    }
    else
    {
        signal = 0;
    }

    return signal;
}

//打印信息
void printf_param()
{
    cout <<">>>>>>>>>>>>>>>>>>>>>>information>>>>>>>>>>>>>>>>>>>>>>>>" <<endl;
    cout <<"position x:"<< pose.pose.position <<"[m]" <<endl;

}

int main(int argc,char **argv)
{
    ROS_INFO("begin");

    ros::init(argc,argv,"offboard");
    ros::NodeHandle nh;

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

    bool signal;
    int timing = 0;


    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state",10,state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local",10);
    ros::Publisher local_vel_pub = nh.advertise<geometry_msgs::TwistStamped>("mavros/setpoint_velocity/cmd_vel",10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
    ros::Subscriber position_sub = nh.subscribe<geometry_msgs::PoseStamped>("mavros/local_position/pose",10,position_cb);
    ros::Subscriber target_pos = nh.subscribe<std_msgs::Float64MultiArray>("/track", 10, target);

//    int index = atoll(argv[1]);
    int index_1 = 0;
//    int index_1 = list_1.data[index];
    target_x = track[index_1][0];
    target_y = track[index_1][1];

    pose.pose.position.x = current_state.position[0];
    pose.pose.position.y = current_state.position[1];
    pose.pose.position.z = current_state.position[2];

    ros::Rate rate(1.0);
    ROS_INFO("init");

    while (ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("ros_ok");

//    for(int i=100;ros::ok() && i>0; --i){
//        local_pos_pub.publish(pose);
//        ros::spinOnce();
//        rate.sleep();
//    }

    ROS_INFO("begin");
//    ROS_INFO("command pos [%f],[%f],[%d],[%s]", target_x,target_y,index,argv[1]);
    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD"; //设定模式

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value=true;

    ros::Time last_request = ros::Time::now();

    while (ros::ok()){
        if (current_state.mode != "OFFBOARD" && (ros::Time::now()-last_request>ros::Duration(5.0))){
            if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent){
                ROS_INFO("offboard enabled");
            }
            last_request = ros::Time::now();
        } else{
            if (!current_state.armed && (ros::Time::now()-last_request>ros::Duration(5.0))){
                if (arming_client.call(arm_cmd) && arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }

        signal = judg_dist();

        if (signal == 1)
        {
            if (timing == 0){
                pose.pose.position.x = 0;
                pose.pose.position.y = 0;
                pose.pose.position.z = 5;
                local_pos_pub.publish(pose);
            }

            if (timing == 1){
                pose.pose.position.x = 0.5;
                pose.pose.position.y = target_y;
                pose.pose.position.z = 1.5;
                local_pos_pub.publish(pose);
            }

            if (timing == 2){
                pose.pose.position.x = target_x;
                pose.pose.position.y = target_y;
                pose.pose.position.z = 1.5;
                local_pos_pub.publish(pose);
            }
            timing ++;
//            ROS_INFO("---commandddddd pos [%f],[%f],[%d]", target_x,target_y,timing);
        }
        ROS_INFO("pos_now x:[%f]y:[%f]z:[%f]",current_state.position[0],current_state.position[1],current_state.position[2]);
        ROS_INFO("pos_mission x:[%f]y:[%f]z:[%f] timing:[%d]", pose.pose.position.x,pose.pose.position.y,pose.pose.position.z,timing);
//        ROS_INFO("index:[%d]", index);


        local_pos_pub.publish(pose);
//        sleep(2);

//        ROS_INFO("command height: [%f] [m]",pose.pose.position.z);

//        local_vel_pub.publish(vel);

        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
