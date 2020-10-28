#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <ros/ros.h>
#include "nav_msgs/Path.h"
#include <stdio.h>
ros::Publisher  path_pub_;
bool count = false; 
void  pub_Onlyonce_Callback(const nav_msgs::Path &msg)
{   
    if (count == true)
    {
        return;
    }
    path_pub_.publish(msg);
    count = true;
}


int main(int argc, char *argv[])
{
  ros::init(argc, argv, "pub_global_plan");
  ros::NodeHandle node;
  ros::Subscriber path_sub_ = node.subscribe("/move_base/TebLocalPlannerROS/global_plan", 1, pub_Onlyonce_Callback);
  path_pub_ = node.advertise<nav_msgs::Path>("/plan", 1);
  ros::spin();
}