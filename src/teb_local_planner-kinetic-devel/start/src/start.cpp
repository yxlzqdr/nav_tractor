#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <stdio.h>

//FILE *fp_nav;
//FILE *fp_work;
void  nav_mode_select_callback(const std_msgs::String &msg)
{   
    ROS_INFO("NAV MODE!!!");
    // if(fp_work != nullptr)
    // {
    //     pclose(fp_work);
    // }
    
    FILE *fp_nav;
    fp_nav = popen("bash  /home/green/run.sh -nav","w");
    pclose(fp_nav);
}

void  work_mode_select_callback(const std_msgs::String &msg)
{
    ROS_INFO("WORK MODE!!!");
    // if (fp_nav != nullptr)
    // {
    //     pclose(fp_nav);
    // }
    
    FILE *fp_work;
    fp_work = popen("bash  /home/green/run.sh -work","w");
    pclose(fp_work);
}

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "mode");
  ros::NodeHandle node;
  ros::Subscriber nav_mode = node.subscribe("/satellite_map/nav_mode", 1, nav_mode_select_callback);
 // ros::Subscriber nav_mode = node.subscribe("/satellite_map/mode_select_nav", 1, nav_mode_select_callback);
  ros::Subscriber work_mode = node.subscribe("/satellite_map/work_mode", 1, work_mode_select_callback);
  ros::spin();
}