//
// Created by zhangcheng on 2020/8/26.
//

#include <ros/ros.h>
#include <iostream>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <learning_communication/OffboardState.h>

using namespace std;

int Num_StateMechine;
int order;
float flight_point[4];
learning_communication::OffboardState order_state;
learning_communication::OffboardState current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state.connected = msg->connected;
    current_state.armed = msg->armed;
    current_state.mode = msg->mode;
}

void position_cb(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    current_state.position[0] = msg->pose.position.x;
    current_state.position[1] = msg->pose.position.y;
    current_state.position[2] = msg->pose.position.z;
//    cout << "Pos_fcu [X Y Z] : " << current_state.position[0] << " [ m ] "<< current_state.position[1] <<" [ m ] "<< current_state.position[2] <<" [ m ] "<<endl;

}



int main(int argc,char **argv)
{
    ros::init(argc,argv,"pos_controller");
    ros::NodeHandle nh;

//    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state",10,state_cb);
//    ros::Subscriber position_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose",10,position_cb);
    ros::Publisher control_pub = nh.advertise<learning_communication::OffboardState>("order_position",10);

    Num_StateMechine = 0;

    while (ros::ok())
    {
        switch (Num_StateMechine) {
            case 0:
                cout << "Input your command,1 means continue to input position" << endl;
                //TODO:创建命令节点
                cin >> order;

                if (order==0){
                    Num_StateMechine = 0;
                    break;
                }

                if (order==1){
                    Num_StateMechine = 1;
                }

                cout << "setpoint_t[0] --- x [m] : " << endl;
                cin >> flight_point[0];
                cout << "setpoint_t[0] --- y [m] : " << endl;
                cin >> flight_point[1];
                cout << "setpoint_t[0] --- z [m] : " << endl;
                cin >> flight_point[2];



            case 1:
                current_state.position[0] = flight_point[0];
                current_state.position[1] = flight_point[1];
                current_state.position[2] = flight_point[2];
                control_pub.publish(current_state);
                cout << "return idle" <<endl;
                cin >> flight_point[3];
                if (flight_point[3] == 0){
                    Num_StateMechine = 0;
                }
//                Num_StateMechine = 0;
                break;

        }
    }



}