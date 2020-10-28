#ifndef CARMONTIOCONTROL_H
#define CARMONTIOCONTROL_H
#include "candriver.h"
#include <thread>
#include <mutex>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>


class CarMontionControl
{
public:
    enum UnmannedMdSts
    {
        PowerON = 0,
        TaskExec,
        Obstacle,
        TaskFinsh,
        SystemFault
    };

    enum Gear
    {
        neutral_gear = 0,
        forward_gear = 1,
        backward_gear,
        park
    };

    enum Upgrade
    {
        stop = 0,
        down ,
        up
    };

    struct ActionTypeGet
    {
        unsigned char heartBeat;    //心跳信号
        unsigned char workMd;	    //工作模式
        unsigned char gear;			//车辆档位
        short speed;				//车辆实时速度
        short steeringAngle;        //前轮实时转向角度
        unsigned char brakeSts;     //刹车状态
        double soc;          //剩余电量SOC
    };

    struct MontionStatusGet
    {
        unsigned char heartBeat;	 //心跳信号
        unsigned char vehSysFltLvl;  //整车故障等级
        unsigned char upGradeStatus; //提升状态
    };

    struct ActionTypeSet
    {
        unsigned char heartBeat;     //心跳信号
        unsigned char unmannedMdSts; //无人状态
        unsigned char gearRequest;   //档位请求
        short speedRequest;          //车辆速度请求
        short steeringRequest;       //转向角度请求
        unsigned char brkRequest;    //刹车请求
        unsigned char UpgradeRequest;//提升请求
    };


    struct Data
    {
        ActionTypeGet actionGet;
        ActionTypeSet actionSet;
        MontionStatusGet statusGet;
        double max_angle ;
        double max_vel;
        double battery_min;

        std::shared_ptr<ros::NodeHandle> pNode;
        ros::Subscriber cmdSubscrible;
        ros::Publisher cancmdPublisher;
        ros::Publisher carstatusPublisher;
        ros::Subscriber taskSubscrible;

        Data() {
            actionSet.unmannedMdSts = 0;
            actionSet.gearRequest = 0;
            actionSet.brkRequest = 1;

            battery_min = -1;
            max_angle = 0.3314;
            max_vel = 1.4;

        }
    };

    CarMontionControl();
    ~CarMontionControl();

    uint8_t getworkMd();
    uint8_t getGear();
    int16_t getSpeed();
    int16_t getSteeringAngle();
    uint8_t getBrakeSts();
    uint8_t getSOC();

    uint8_t status_HeartBeat();
    uint8_t status_VehSysFltLvl();
    uint8_t status_UpGradeSts();

    void setUnmannedMdSts(UnmannedMdSts unMantype);
    void setGearRequest(Gear gear);
    void setSpeedRequest(double vel);
    void setSteeringReques(double angle);
    void setBrkRequest(bool bBrk);
    void setUpgradeRequest(Upgrade upgrade);
    void parseFrame();
    bool sendcmd(ActionTypeSet set);
    std::shared_ptr<Data> pdata;
private:
    std::shared_ptr<CANDRIVER >candriver;
    bool sendcmd();
    int16_t calucateAgnle();

    void cmdMsgReceCallBack(const geometry_msgs::Twist::ConstPtr &msg);
    void taskMsgCallBack(const std_msgs::String::ConstPtr &msg);
    std::thread canSendThd;
    std::mutex send_mutex;
    std::mutex parse_mutex;
    std::mutex status_mutex;

};
#endif // CANMONTIOCONTROL_H
