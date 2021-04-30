//
// Created by x on 3/1/21.
//

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <stdio.h>
#include <stdlib.h>
#include <eigen3/Eigen/Eigen>
#pragma comment(lib,"ws2_32.lib")
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <tf2_msgs/TFMessage.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/TransformStamped.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/time_synchronizer.h>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <geometry_msgs/PoseStamped.h>
#include <ros/ros.h>
#include <learning_communication/OffboardState.h>
using namespace std;

geometry_msgs::PoseStamped pose;
learning_communication::OffboardState current_state;
float f[7];
//判断航点是否到达
int judg_dist()
{
    float delta_x;
    float delta_y;
    float delta_z;
    float dist;
    bool signal;

    delta_x = f[0]-pose.pose.position.x;
    delta_y = f[1]-pose.pose.position.y;
    delta_z = f[2]-pose.pose.position.z;
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

void position_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    pose.pose.position.x = msg->pose.position.x;
    pose.pose.position.y = msg->pose.position.y;
    pose.pose.position.z = msg->pose.position.z;
//    ROS_INFO("current height order: [%f] [m]",current_state.position[2]);
}

void msgCallback(const geometry_msgs::PoseStamped::ConstPtr &P) {
    f[0] = P->pose.position.x;	
    f[1] = P->pose.position.y;
    f[2] = P->pose.position.z;
    //ROS_INFO("I recevied the topic: %f %f %f",f[0],f[1],f[2]);
}

void msgCallback2(const tf2_msgs::TFMessage msg_tf) {
    f[3] = msg_tf.transforms[0].transform.translation.x;
    f[4] = msg_tf.transforms[0].transform.translation.y;
    f[5] = msg_tf.transforms[0].transform.translation.z;
    //ROS_INFO("I recevied the topic: %f %f %f",f[3],f[4],f[5]);
}

void *udp_get( void *arg )
{
    int port = 7999;
    int sockfd;

    // 创建socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1==sockfd){
        puts("Failed to create socket");
    }

    // 设置地址与端口
    struct sockaddr_in addr;
    socklen_t  addr_len=sizeof(addr);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;       //use IPV4
    addr.sin_port   = htons(port);    
    addr.sin_addr.s_addr = inet_addr("192.168.10.102");
    //addr.sin_addr.s_addr = inet_addr("192.168.0.110");
    //addr.sin_addr.s_addr = inet_addr("10.109.6.105");
    //addr.sin_addr.s_addr = inet_addr("192.168.43.64");
    // Time out
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 2000000;  // 2000 ms
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

    int counter = 0;

    while(ros::ok())
    {
        int signal = judg_dist();
        if (signal == 1){
            f[6] = 1.0;
        }
        else{
            f[6] = 0.0;}
        /*for(int i=0;i<3;++i){
          pos[i]=htonl(f[i]);
        }*/
        //sendto(sockfd, "hello world", 11, 0, (sockaddr*)&addr, addr_len);
        sendto(sockfd, f, sizeof(f), 0, (sockaddr*)&addr, addr_len);
        //printf("%f",f[2]);
        printf("Sended %d:%f,%f,%f\n", ++counter,f[2],f[5],f[6]);
        usleep(50000);
    }
    close(sockfd);
}
void create_all_thread(void)
{
    pthread_t thread_udp_get;//声明线程ID
    if( (pthread_create( &thread_udp_get , NULL , udp_get, NULL )) != 0 )//创建线程
    {
        perror("Create the thread_joy fail");
        exit( 1 );
    }
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"send_pos5");
    ros::NodeHandle nh;
    ros::Subscriber pos_sub = nh.subscribe("mavros/local_position/pose", 1000, msgCallback);
    ros::Subscriber vision_sub = nh.subscribe("/tf", 1000, msgCallback2);
    ros::Subscriber local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>("mavros/setpoint_position/local",10,position_cb);
    create_all_thread();
    ros::spin();

    return 0;
}
