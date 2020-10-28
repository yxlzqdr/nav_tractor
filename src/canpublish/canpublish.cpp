#include "ros/ros.h"
#include "std_msgs/String.h"
#include "carmontioncontrol.h"
#include "geometry_msgs/Twist.h"
#include "math.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "canpublish",ros::init_options::NoSigintHandler);
//    ros::NodeHandle nh;


//    ros::Rate loop_rate(100);
    CarMontionControl car;

    while (ros::ok())
    {
        geometry_msgs::Twist msg;

        msg.linear.x = (car.pdata->actionGet.speed -1000)/(10*3.6);
        msg.angular.z = (car.pdata->actionGet.steeringAngle -900)*180/M_PI;
//        usleep(30000);

    }

    return 0;
}
