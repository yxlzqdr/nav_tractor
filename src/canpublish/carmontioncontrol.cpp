#include "carmontioncontrol.h"
#include "comm.h"
#include <math.h>

CarMontionControl::CarMontionControl()
{
    candriver = std::make_shared<CANDRIVER>(CANDRIVER(VCI_USBCAN2, 0, 0, BAUD_250));
    candriver->initCan();
    pdata = std::make_shared<Data>();
    pdata->pNode = std::make_shared<ros::NodeHandle>();
    pdata->actionSet.UpgradeRequest = up;
    pdata->cmdSubscrible = pdata->pNode->subscribe("/cmd_vel", 1, &CarMontionControl::cmdMsgReceCallBack, this);
    //    pdata->cancmdPublisher = pdata->pNode->advertise<geometry_msgs::Twist>("/car/cancmd",100);
    //    pdata->carstatusPublisher = pdata->pNode->advertise<std_msgs::String>("/car/status",1);

    canSendThd = std::thread([&] {
        while (true)
        {
            ros::spinOnce();
            this->sendcmd();
            parseFrame();
            usleep(30000);
        }
    });
    //    recvsendThd.detach();
}

CarMontionControl::~CarMontionControl()
{
}

uint8_t CarMontionControl::getworkMd()
{
    return pdata->actionGet.workMd;
}

uint8_t CarMontionControl::getGear()
{
    return pdata->actionGet.gear;
}

int16_t CarMontionControl::getSpeed()
{
    return pdata->actionGet.speed;
}

int16_t CarMontionControl::getSteeringAngle()
{
    return pdata->actionGet.steeringAngle;
}

uint8_t CarMontionControl::getBrakeSts()
{
    return pdata->actionGet.brakeSts;
}

uint8_t CarMontionControl::getSOC()
{
    return pdata->actionGet.soc;
}

uint8_t CarMontionControl::status_HeartBeat()
{
    return pdata->statusGet.heartBeat;
}

uint8_t CarMontionControl::status_VehSysFltLvl()
{
    return pdata->statusGet.vehSysFltLvl;
}

/*
 * @Brief:
 * @Param:  First
 * @Param:  Second
 * @Return:
 */
uint8_t CarMontionControl::status_UpGradeSts()
{
    return pdata->statusGet.upGradeStatus;
}

/*
 * @Brief:
 * @Param: unMantype
 * 	0:系统开机
    1:任务执行
    2:避障停车
    3:任务结束
    4:系统故障
 * @Return: NULL
 */
void CarMontionControl::setUnmannedMdSts(CarMontionControl::UnmannedMdSts unMantype)
{
    pdata->actionSet.unmannedMdSts = unMantype;
}

/*
 * @Brief:
 * @Param:  gear
 *  0:空挡
 *  1:前进
 *  2:后退
 *  3:驻车
 * @Return: NULL
 */
void CarMontionControl::setGearRequest(CarMontionControl::Gear gear)
{
    pdata->actionSet.gearRequest = gear;
}

/*
 * @Brief:
 * @Param: vel 车辆速度期望值:-100-100Km/h
 * 如果整车达不到,NCU不处理速度按实际值处理
 * @Return: NULL
 */
void CarMontionControl::setSpeedRequest(double vel)
{
    pdata->actionSet.speedRequest = vel ;//> pdata->max_vel? pdata->max_vel:vel;
}

/*
 * @Brief: set Steering
 * @Param:  angle 范围:-90°~+90°
 * @Return: NULL
 */
void CarMontionControl::setSteeringReques(double angle)
{
    pdata->actionSet.steeringRequest = angle;//abs(angle) > pdata->max_angle ? (pdata->max_angle= angle >0?pdata->max_angle:pdata->max_angle*(-1)): angle;
}

/*
 * @Brief:  提升的位置由VCU控制
 * @Param:  bBrk  目前只发0(无刹车请求)、1(有刹车请求)
 * @Return: NULL
 */
void CarMontionControl::setBrkRequest(bool bBrk)
{
    pdata->actionSet.brkRequest = bBrk ? 1 : 0;
}

/*
 * @Brief: set upgrade function
 * @Param:  upgrade
 * 	0:停止
    1:下降
    2:上升
 * @Return: NULL
 */
void CarMontionControl::setUpgradeRequest(CarMontionControl::Upgrade upgrade)
{
    pdata->actionSet.UpgradeRequest = upgrade;
}

/*
 * @Brief:
 * @Return: NULL
 */
bool CarMontionControl::sendcmd()
{
    can_frame frame = {0};
    frame.can_id = SETFRAMEID;
    frame.can_dlc = CAN_DATA_LEN;
    //    send_mutex.lock();
    frame.data[0] = pdata->actionSet.unmannedMdSts;
    frame.data[1] = pdata->actionSet.gearRequest;
    frame.data[2] = pdata->actionSet.speedRequest & 0xff;
    frame.data[3] = pdata->actionSet.speedRequest >> 8;
    frame.data[4] = pdata->actionSet.steeringRequest & 0xff;
    frame.data[5] = pdata->actionSet.steeringRequest >> 8;
    frame.data[6] = pdata->actionSet.brkRequest;
    frame.data[7] = pdata->actionSet.UpgradeRequest;
    //    send_mutex.unlock();

    if (!candriver->canTransimit(&frame))
    {
        ROS_INFO("send fail\n");
        return false;
    }
    return true;
}

/*
 * @Brief:
 * @Return: NULL
 */
void CarMontionControl::parseFrame()
{
    std::vector<can_frame> frame_vec;

    if (!candriver->canReceive(frame_vec))
    {
        ROS_INFO("not Receive message!");
        return;
    }

    for (auto frame : frame_vec)
    {
        switch (frame.can_id)
        {
        case GETFRAMEID:
        {
            if (frame.can_dlc < 8)
            {
                ROS_INFO("can data length error");
                return;
            }
            //            parse_mutex.lock();
            pdata->actionGet.workMd = frame.data[0];
            pdata->actionGet.gear = frame.data[1];
            pdata->actionGet.speed = frame.data[3] << 8 + frame.data[2];
            pdata->actionGet.steeringAngle = frame.data[5] << 8 + frame.data[4];
            pdata->actionGet.brakeSts = frame.data[6];
            pdata->actionGet.soc = frame.data[7] / 100;
            //            parse_mutex.unlock();
            if (pdata->actionGet.soc < pdata->battery_min)
                ROS_INFO("battery low");
            break;
        }
        case GETSTATUSFRAMEID:
        {
            if (frame.can_dlc != 8)
            {
                ROS_INFO("can data length error");
                return;
            }
            //            status_mutex.lock();
            pdata->statusGet.heartBeat = frame.data[0];
            pdata->statusGet.vehSysFltLvl = frame.data[1];
            pdata->statusGet.upGradeStatus = frame.data[2];
            //            status_mutex.unlock();
            break;
        }
        default:
            break;
        }
    }
}

bool CarMontionControl::sendcmd(CarMontionControl::ActionTypeSet set)
{
    can_frame frame = {0};
    frame.can_id = SETFRAMEID;
    frame.can_dlc = CAN_DATA_LEN;
    frame.data[0] = set.unmannedMdSts;
    frame.data[1] = set.gearRequest;
    frame.data[2] = set.speedRequest & 0xff;
    frame.data[3] = set.speedRequest >> 8;
    frame.data[4] = set.steeringRequest & 0xff;
    frame.data[5] = set.steeringRequest >> 8;
    frame.data[6] = set.brkRequest;
    frame.data[7] = set.UpgradeRequest;

    if (!candriver->canTransimit(&frame))
    {
        printf("send failed\n");
        return false;
    }
    return true;
}

/*
 * @Brief:callback
 * @Param:  msg
 * @Return: NULL
 */
void CarMontionControl::cmdMsgReceCallBack(const geometry_msgs::Twist::ConstPtr &msg)
{
    double vecle, angle;
    vecle = msg->linear.x;
    angle = -1*msg->angular.z;
    double angleMax;
    vecle = fabs(vecle) * 10 * 3.6 + OFFSET_VEL;                // m/s => km/h resolution:0.1 default value:1000
    angleMax = abs(angle) > pdata->max_angle ? (pdata->max_angle= angle >0?pdata->max_angle:pdata->max_angle*(-1)): angle ;
    
    angle = ((angleMax - 0.107) * 180 / M_PI) * 10 + OFFSET_ANGLE; //offset :-90,resolution: 0.1 default value :900

    printf("vecle :%f angle:%f \n", vecle, angle);

    send_mutex.lock();
    pdata->actionSet.unmannedMdSts = 1;

    pdata->actionSet.gearRequest = msg->linear.x >= 0 ? 1 : 2;
   
    pdata->actionSet.brkRequest = 0;
    pdata->actionSet.UpgradeRequest = msg->linear.z;

    setSpeedRequest(vecle);
    setSteeringReques(angle);
    send_mutex.unlock();
    ROS_INFO("subscribe message");
}
