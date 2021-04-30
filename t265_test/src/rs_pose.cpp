//
// Created by zhangcheng on 2020/9/14.
//

#include <ros/ros.h>
#include <librealsense2/rs.hpp>
#include <geometry_msgs/PoseStamped.h>
#include <eigen3/Eigen/Eigen>

using namespace std;

geometry_msgs::PoseStamped pose_t;

ros::Publisher t_pub;

void printf_info(){
    cout <<">>>>>>>>>>>>>>>>>>>>>>>>PX4_T265_ESTIMATOR<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" <<endl;
    //固定的浮点显示
    cout.setf(ios::fixed);
    //setprecision(n) 设显示小数精度为n位
    cout<<setprecision(2);
    //左对齐
    cout.setf(ios::left);
    // 强制显示小数点
    cout.setf(ios::showpoint);
    // 强制显示符号
    cout.setf(ios::showpos);

    cout << "Pos_t [X Y Z]" << pose_t.pose.position.x << " [m] " << pose_t.pose.position.y << " [m] " << pose_t.pose.position.z << " [m] " << endl;

}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"rs_pose");
    ros::NodeHandle n;

    rs2::context ctx;

    std::cout << "hello from librealsense - " << RS2_API_VERSION_STR << std::endl;
    std::cout << "You have " << ctx.query_devices().size() << " RealSense devices connected" << std::endl;

    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_POSE,RS2_FORMAT_6DOF);
    pipe.start();

    t_pub = n.advertise<geometry_msgs::PoseStamped>("tPose",1000);

    while (true)
    {
        auto frames = pipe.wait_for_frames();
        auto f = frames.first_or_default(RS2_STREAM_POSE);
        auto pose_data = f.as<rs2::pose_frame>().get_pose_data();
        pose_t.pose.position.x = -pose_data.translation.z;
        pose_t.pose.position.y = -pose_data.translation.x;
        pose_t.pose.position.z = pose_data.translation.y;

        t_pub.publish(pose_t);
        printf_info();
        usleep(1000);
    }
}
