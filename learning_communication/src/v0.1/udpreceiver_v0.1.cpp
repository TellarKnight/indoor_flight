//
// Created by k on 2021/3/15.
//

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

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
learning_communication::OffboardState current_state;

//
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

int main(int argc,char **argv)
{
    ROS_INFO("begin");

    ros::init(argc,argv,"offboard");
    ros::NodeHandle nh;

//    int index = *argv[1] - 48;

    //读取参数表中的参数
//    nh.param<int>("uav_name", index, 0);

    bool signal;
    int timing = 0;
    float target_height = 0.5;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state",10,state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local",10);
    ros::Publisher local_vel_pub = nh.advertise<geometry_msgs::TwistStamped>("mavros/setpoint_velocity/cmd_vel",10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
//    ros::ServiceClient set_mode_client1 = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
    ros::Subscriber position_sub = nh.subscribe<geometry_msgs::PoseStamped>("mavros/local_position/pose",10,position_cb);

    ros::Rate rate(20.0);
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

    mavros_msgs::SetMode takeoff_set_mode;
    takeoff_set_mode.request.custom_mode = "AUTO.TAKEOFF"; //设定模式

    mavros_msgs::SetMode land_set_mode;
    land_set_mode.request.custom_mode = "AUTO.LAND"; //设定模式

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value=true;

    ros::Time last_request = ros::Time::now();

// udp para
    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);

    // 广播地址
    struct sockaddr_in from;
    memset(&from,0, sizeof(struct sockaddr_in));
    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_ANY);
    from.sin_port = htons(9999);

    int sock = -1;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        cout<<"socket error"<<endl;
        return false;
    }

    const int opt = 1;
    //设置该套接字为广播类型，
    int nb = 0;
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if(nb == -1)
    {
        cout<<"set socket error..."<<endl;
        return false;
    }

    if(bind(sock,(struct sockaddr *)&(from), sizeof(struct sockaddr_in)) == -1)
    {
        cout<<"bind error..."<<endl;
        return false;
    }

    int len = sizeof(sockaddr_in);

    FILE *fp;
    fd_set fds;
    int maxfdp;
    struct timeval timeout={0,0};

    while (ros::ok()){
        //udp
        //从广播地址接受消息
        int ret;
        double smsg[4];
        smsg[3] = 5;

        FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化
        FD_SET(sock,&fds); //添加描述符
        FD_SET(sock,&fds); //添加描述符

        maxfdp = sock+1;

        switch(select(maxfdp,&fds,NULL,NULL,&timeout)) //select使用
        {
            case -1: exit(-1);break; //select错误，退出程序
            case 0:
                printf("recvfrom fail\n");
                break; //再次轮询
            default:
                if(FD_ISSET(sock,&fds)) //测试sock是否可读，即是否网络上有数据
                {
                    recvfrom(sock, smsg, sizeof(smsg), 0, (struct sockaddr*)&from,(socklen_t*)&len);//接受网络数据
                    printf("receive a broadCast messages:%f%f%f%f\n", smsg[0],smsg[1],smsg[2],smsg[3]);
                }// end if break;
        }// end switch

//        if((ret=recvfrom(sock, smsg, sizeof(smsg), 0, (struct sockaddr*)&from,(socklen_t*)&len)) != -1){
//            std::cout<<"recvbuf:::"<<smsg[0]<<std::endl;
//            printf("receive a broadCast messages:%f%f%f%f\n", smsg[0],smsg[1],smsg[2],smsg[3]);
//        } else{
//            printf("recvfrom fail\n");
//        }
//        && current_state.mode != "AU.LOITER"

        if (current_state.mode!="OFFBOARD" && (smsg[3]==2)){
            if(set_mode_client.call(offb_set_mode) ) {
                ROS_INFO("offboard enable");
            }
            last_request = ros::Time::now();
        }else if (!current_state.armed && (smsg[3]==1 || smsg[3]==2 ||smsg[3]==3)){
            if (arming_client.call(arm_cmd) && arm_cmd.response.success){
                ROS_INFO("Vehicle armed");
            }
        }
        if (current_state.mode!="AUTO.LAND" && (smsg[3]==3)){
            if(set_mode_client.call(land_set_mode) ) {
                ROS_INFO("land enable");
            }
        }

        cout<< current_state.mode<<endl;
        std::cout <<smsg[3]<< "kkkkkkkkkkkkkkkkkkkkkkk" << std::endl;

        arm_cmd.request.value= true;
        if(smsg[3]==0){
            arm_cmd.request.value= false;
            arming_client.call(arm_cmd);
        }

        pose.pose.position.x = smsg[0];
        pose.pose.position.y = smsg[1];
        pose.pose.position.z = smsg[2];

        local_pos_pub.publish(pose);
//        ROS_INFO("pos_now x:[%f]y:[%f]z:[%f]",current_state.position[0],current_state.position[1],current_state.position[2]);
//        ROS_INFO("pos_mission x:[%f]y:[%f]z:[%f] timing:[%d]", pose.pose.position.x,pose.pose.position.y,pose.pose.position.z,timing);


//        sleep(2);

//        ROS_INFO("command height: [%f] [m]",pose.pose.position.z);

//        local_vel_pub.publish(vel);

        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
