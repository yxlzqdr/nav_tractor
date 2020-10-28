#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/NavSatStatus.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Bool.h"
#include "move_base_msgs/MoveBaseActionGoal.h"
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>
#include "tf/transform_listener.h"
#include <Eigen/Core>
#include <visualization_msgs/Marker.h>
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <gps_common/conversions.h>
#include "document.h"
#include "stringbuffer.h"
#include "prettywriter.h"


using namespace gps_common;
using namespace std;
using namespace rapidjson;
ros::Publisher odom_pub;
ros::Publisher pathPub;
ros::Publisher rviz_target;
ros::Publisher home_path;
std::vector<geometry_msgs::Point> pointsBuf;
sensor_msgs::NavSatFix fixStart;
double offset_heading, offset_x, offset_y;
bool record_starting_point, get_starting_point;
std::vector<std::string> dst;
double linear_x, angular_z;


void Split(const std::string &src, const std::string &separator, std::vector<std::string> &dest); //字符串分割到数组

void convert(const sensor_msgs::NavSatFixConstPtr &fix, tf::Transform &t)
{
	double northing = 0.0;
	double easting = 0.0;
	std::string zone;
	if (fix->status.status == sensor_msgs::NavSatStatus::STATUS_NO_FIX)
	{
		ROS_DEBUG_THROTTLE(60, "No fix.");
		return;
	}
	LLtoUTM(fix->latitude, fix->longitude, northing, easting, zone);
	tf::Point p(easting, northing, 0);
	tf::Quaternion q = tf::createQuaternionFromRPY(0, 0, 3.1415926 / 2.0 + offset_heading / (180.0 / 3.141592) - fix->altitude / (180.0 / 3.141592));
	t.setRotation(q);
	t.setOrigin(p);
}
void pubMsg(tf::Transform t)
{
	nav_msgs::Odometry odom;
	odom.header.stamp = ros::Time::now();
	odom.header.frame_id = "aerial_0";
	odom.child_frame_id = "aerial_t";
	odom.pose.pose.position.x = t.getOrigin().getX() + offset_x;
	odom.pose.pose.position.y = t.getOrigin().getY() + offset_y;
	odom.pose.pose.position.z = t.getOrigin().getZ();
	pointsBuf.push_back(odom.pose.pose.position);
	odom.pose.pose.orientation.w = t.getRotation().getW();
	odom.pose.pose.orientation.x = t.getRotation().getX();
	odom.pose.pose.orientation.y = t.getRotation().getY();
	odom.pose.pose.orientation.z = t.getRotation().getZ();
	odom.twist.twist.linear.x = linear_x;
	odom.twist.twist.angular.z = angular_z;
	odom_pub.publish(odom);
}
void pubPath(tf::Transform t)
{
	visualization_msgs::Marker points;
	points.header.frame_id = "/map";
	points.header.stamp = ros::Time::now();
	points.ns = "points";
	points.action = visualization_msgs::Marker::ADD;
	// points.pose.orientation.w = 1.0;
	// %EndTag(MARKER_INIT)%

	points.id = 0;

	points.type = visualization_msgs::Marker::POINTS;

	points.scale.x = 0.2;
	points.scale.y = 0.2;
	points.scale.z = 0.2;

	points.color.g = 1.0f;
	points.color.a = 1.0;

	// Create the vertices for the points and lines
	for (uint32_t i = 0; i < pointsBuf.size(); i++)
	{

		geometry_msgs::Point p;
		p.x = pointsBuf[i].x;
		p.y = pointsBuf[i].y;
		p.z = pointsBuf[i].z;

		points.points.push_back(p);
	}
	pathPub.publish(points);
}
void pubgoal(tf::Transform t)
{
	move_base_msgs::MoveBaseActionGoal MapGoal;
	MapGoal.header.stamp = ros::Time::now();
	MapGoal.goal.target_pose.header.frame_id = "map";
	MapGoal.goal.target_pose.pose.position.x = t.getOrigin().getX() + offset_x;
	MapGoal.goal.target_pose.pose.position.y = t.getOrigin().getY() + offset_y;
	MapGoal.goal.target_pose.pose.orientation.w = 1;
	rviz_target.publish(MapGoal);
}
void pubhomepath(const nav_msgs::Path &msg)
{ 
	ROS_INFO("PUBLISH PLAN!!!");
    int pathLength = msg.poses.size();
    Document doc;
    Document::AllocatorType &allocator = doc.GetAllocator();
    doc.SetObject();
    Value array(kArrayType);
    for (int i = 0; i < pathLength; i++)
    {
        Value item(kObjectType);
        double utm_e = msg.poses[i].pose.position.x;
        double utm_n = msg.poses[i].pose.position.y;
        double gps_lat;
        double gps_lng;
        char *zone = "50S";
        gps_common::UTMtoLL(utm_n, utm_e, zone, gps_lat, gps_lng);
        item.AddMember("lat", gps_lat, allocator);
        item.AddMember("lng", gps_lng, allocator);
        if (i % 10 == 0)
        {
            array.PushBack(item, allocator);
        }
    }
    doc.AddMember("shapes", array, allocator);
    StringBuffer stringBuf;
    PrettyWriter<StringBuffer> writer(stringBuf);
    doc.Accept(writer);
    std_msgs::String gps;
    gps.data = stringBuf.GetString();
	home_path.publish(gps);
	ROS_INFO("PUBLISH PLAN!!!");
}
static tf::Transform utm_aerial_0;
void gps_msg_callback(const sensor_msgs::NavSatFixConstPtr &fix)
{
	//NOTE:static 必须为static
	static tf::TransformBroadcaster br;
	tf::Transform utm_aerial_t;
	tf::Transform aerial_0_map;
	aerial_0_map.setIdentity();

	convert(fix, utm_aerial_t);


	if (record_starting_point == true)
	{
		utm_aerial_0  = utm_aerial_t;

		//读取文件
        ofstream openfile("/home/green/catkin_ws/map_origin.txt",ios::out);
        if(!openfile)
        {
            cout << "文件不能打开" << endl;
        }
        else
        {
            openfile << fixed << utm_aerial_0.getOrigin().getX() << "," << utm_aerial_0.getOrigin().getY() << "," << utm_aerial_0.getOrigin().getZ() << "," 
					 << utm_aerial_0.getRotation().getX() << "," << utm_aerial_0.getRotation().getY() << "," << utm_aerial_0.getRotation().getZ() << "," << utm_aerial_0.getRotation().getW() << endl;

            openfile.close();
        }


		ROS_INFO("%f", utm_aerial_0.getOrigin().getX());
		ROS_INFO("%f", utm_aerial_0.getOrigin().getY());
		ROS_INFO("%f", utm_aerial_0.getOrigin().getZ());
		ROS_INFO("%f", utm_aerial_0.getRotation().getX());
		ROS_INFO("%f", utm_aerial_0.getRotation().getY());
		ROS_INFO("%f", utm_aerial_0.getRotation().getZ());
		ROS_INFO("%f", utm_aerial_0.getRotation().getW());

		record_starting_point = false;
	}
	if(get_starting_point == true)
	{
		ROS_INFO("获取初始点\n");
		tf::Point p(std::stod(dst[0]), std::stod(dst[1]), std::stod(dst[2]));
		tf::Quaternion q(std::stod(dst[3]), std::stod(dst[4]), std::stod(dst[5]), std::stod(dst[6]));
		utm_aerial_0.setOrigin(p);
		utm_aerial_0.setRotation(q);
		get_starting_point = false;
	}
	tf::Transform aerial_0_aerial_t = utm_aerial_0.inverse() * utm_aerial_t;
	br.sendTransform(tf::StampedTransform(utm_aerial_0, ros::Time::now(), "utm", "aerial_0"));
	br.sendTransform(tf::StampedTransform(aerial_0_map, ros::Time::now(), "aerial_0", "map"));
	br.sendTransform(tf::StampedTransform(aerial_0_aerial_t, ros::Time::now(), "aerial_0", "base_footprint"));
	br.sendTransform(tf::StampedTransform(aerial_0_aerial_t, ros::Time::now(), "aerial_0", "base_link"));
	//br.sendTransform(tf::StampedTransform(aerial_0_aerial_t, ros::Time::now(), "map", "base_link"));
	//br.sendTransform(tf::StampedTransform(aerial_0_aerial_t, ros::Time::now(), "map", "base_footprint"));
	pubMsg(aerial_0_aerial_t);
	pubPath(aerial_0_aerial_t);	
}

void Split(const std::string &src, const std::string &separator, std::vector<std::string> &dest) //字符串分割到数组
{

    //参数1：要分割的字符串；参数2：作为分隔符的字符；参数3：存放分割后的字符串的vector向量

    string str = src;
    string substring;
    string::size_type start = 0, index;
    dest.clear();
    index = str.find_first_of(separator, start);
    do
    {
        if (index != string::npos)
        {
            substring = str.substr(start, index - start);
            dest.push_back(substring);
            start = index + separator.size();
            index = str.find(separator, start);
            if (start == string::npos)
                break;
        }
    } while (index != string::npos);

    //the last part
    substring = str.substr(start);
    dest.push_back(substring);
}

void car_odom_callback(const nav_msgs::Odometry& car_odom)
{
    linear_x = car_odom.twist.twist.linear.x;
    angular_z = car_odom.twist.twist.angular.z;
}

void gpsTargetMsgCallback(const std_msgs::String &msg)
{
	ROS_INFO("GET TARGET!!!");
	tf::Transform utm_aerial_t;
	std::string s = msg.data.c_str();
    ROS_INFO("I heard: [%s]", msg.data.c_str());
    rapidjson::Document doc;
    double lat = 0.0;
    double lng = 0.0;
    //首先进行解析，没有解析错误才能进行具体字段的解析
    if (!doc.Parse(s.data()).HasParseError())
    {
        const rapidjson::Value &json_lat = doc["lat"];
        lat = json_lat.GetDouble();
        cout << "lat:" << lat << endl;
        const rapidjson::Value &json_lng = doc["lng"];
        lng = json_lng.GetDouble();
        cout << "lng:" << lng << endl;
    }
    double N;
    double E;
	//move_base_msgs::MoveBaseActionGoal map_target;
    std::string ss;
    gps_common::LLtoUTM(lat, lng, N, E, ss);
    cout << "target_x/target_y" << N << "/" << E << endl;
    tf::Point p(E, N, 0);
	utm_aerial_t.setOrigin(p);
	tf::Transform aerial_0_aerial_t = utm_aerial_0.inverse() * utm_aerial_t;
	pubgoal(aerial_0_aerial_t);
}
void GlobalPlanConvertCallback(const nav_msgs::Path &msg)
{
  ROS_INFO("CONVERT GLOBALPLAN!!!");
  nav_msgs::Path map_path;
  for(int i = 0; i < msg.poses.size(); i++)
  {
	geometry_msgs::PoseStamped map_point;
    tf::Transform utm_aerial_m;
	double map_x = msg.poses[i].pose.position.x;
	double map_y = msg.poses[i].pose.position.y;
	tf::Point p(map_x, map_y, 0);
	utm_aerial_m.setOrigin(p);
	tf::Transform aerial_0_aerial_m = utm_aerial_0 * utm_aerial_m;
	map_point.pose.position.x = aerial_0_aerial_m.getOrigin().getX() + offset_x;
	map_point.pose.position.y = aerial_0_aerial_m.getOrigin().getY() + offset_y;
	map_path.poses.push_back(map_point);
  } 
  pubhomepath(map_path);
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "utm_odometry_node");
	ros::NodeHandle node;
	ROS_INFO("ROS node running\n");
	ROS_INFO("ROS node running\n");
	ROS_INFO("ROS node running\n");
	ROS_INFO("ROS node running\n");
	ROS_INFO("ROS node running\n");
	ROS_INFO("ROS node running\n");
    rviz_target = node.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal",1); 
 	odom_pub = node.advertise<nav_msgs::Odometry>("utm_odom", 10);//utm_odom
	home_path = node.advertise<std_msgs::String>("/satellite_map/gps_coverage_path_final", 1);
	ros::Subscriber fix_sub = node.subscribe("sensor/gps", 10, gps_msg_callback);
	ros::Subscriber car_odom_sub = node.subscribe("/car/odom", 10, car_odom_callback);
	ros::Subscriber gps_goal_sub = node.subscribe("/satellite_map/gps_target", 1, gpsTargetMsgCallback);
	ros::Subscriber global_plan = node.subscribe("/move_base/GlobalPlanner/plan", 1, GlobalPlanConvertCallback);
	pathPub = node.advertise<visualization_msgs::Marker>("gps_marker", 10);

	node.param<double>("offset_heading", offset_heading, 4.52128);
	node.param<double>("offset_x", offset_x, -0.375612);
	node.param<double>("offset_y", offset_y, 0.0103665);
	node.param<bool>("record_starting_point", record_starting_point, false);
	node.param<bool>("get_starting_point", get_starting_point, false);
	std::string str; 
	node.param<std::string>("map_origin", str, "646398.477401,4130918.570819,0.000000,0.000000,0.000000,-0.680838,0.732434");

	Split(str, "," , dst);
	tf::Point p1(std::stod(dst[0]), std::stod(dst[1]), std::stod(dst[2]));
	tf::Quaternion q1(std::stod(dst[3]), std::stod(dst[4]), std::stod(dst[5]), std::stod(dst[6]));
	utm_aerial_0.setOrigin(p1);
	utm_aerial_0.setRotation(q1);

	ros::spin();
}
