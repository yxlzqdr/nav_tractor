/***
  * @projectName  canpublish
  * @brief
  * @param
  * @author bruce geng
  * @date 2020-06-04
  */
#include "candriver.h"
#include <stdio.h>
#include "stdlib.h"
#include "sys/types.h"
#include <string.h>
#include <iostream>
#include <exception>.
#include <vector>

CANDRIVER::CANDRIVER(unsigned int _idevType, unsigned int _idevIndex, unsigned int _icanDev, unsigned int _iBaud):
    idevType(_idevType),idevIndex(_idevIndex),icanDev(_icanDev),iBaud(_iBaud),bopen(false)
{

}

CANDRIVER::~CANDRIVER()
{
    VCI_ClearBuffer(idevType,idevIndex,icanDev);
    VCI_CloseDevice(idevType,idevIndex);
    bopen = false;
}

void CANDRIVER::initCan()
{
    if(!openDevice(idevType,idevIndex,icanDev)){
        printf("OpenDevice Error\n");
        return;
    }

    printf("OpenDevice success\n");
    DWORD dw;
    VCI_INIT_CONFIG config;
    config.AccCode = 0;            /* 验收码 */
    config.AccMask = 0xffffffff;   /* 屏蔽码，推荐设置为 0xFFFFFFFF,即全部接收 */
    config.Filter = 1;             /* 滤波方式，1表示单滤波，0表示双滤波 */
    config.Mode = 0;               /* 模式，0表示正常模式，1表示只听模式 */
    config.Timing0 = iBaud & 0xff; /* 波特率定时器0 */
    config.Timing1 = iBaud >> 8;   /* 波特率定时器0 */

    dw = VCI_InitCAN(idevType,idevIndex,icanDev,&config);
    if(dw != STATUS_OK){
        printf("Init Device Erro\n");
        return;
    }

    dw = VCI_ClearBuffer(idevType,idevIndex,icanDev);
    dw = VCI_StartCAN(idevType,idevIndex,icanDev);
    if(dw != STATUS_OK){
        printf("Start CAN%d failed\n",icanDev);
        VCI_CloseDevice(idevType,idevIndex);
        return;
    }
    bopen = true;

    printf("Start CAN success!\n");

}

bool CANDRIVER::openDevice(unsigned int idevType, unsigned int idevIndex, unsigned int iCanInd)
{
    DWORD dwl = VCI_OpenDevice(idevType,idevIndex,0);
    if(dwl != STATUS_OK){
        printf("OpenDevice Error");
        return false;
    }
    return true;
}

bool CANDRIVER::canTransimit(const can_frame *frame)
{
    if(frame == nullptr){
        printf("frame error\n");
        return false;
    }

    if(!bopen){
        printf("CAN Device not Open!\n");
        return false;
    }

    try
    {
        VCI_CAN_OBJ can;
        canConvertFrame(frame,&can);

        DWORD dw = VCI_Transmit(idevType,idevIndex,icanDev,&can,1);
        if(dw != STATUS_OK){
            printf("send failed\n");
            return false;
        }
    }
    catch(std::exception &e)
    {
        printf("Exception %s\n",e.what());
        return false;
    }

    return true;
}

bool CANDRIVER::canReceive(std::vector<can_frame> &frame_vec)
{
    if(!bopen){
       printf("Device Not Open\n");
       return false;
    }

    if(0 == VCI_GetReceiveNum(idevType,idevIndex,icanDev)){
       printf("unread buffer is 0\n");
       return false;
    }
    VCI_CAN_OBJ can[MAX_BUFFER];
    try
    {
        int count = VCI_Receive(idevType,idevIndex,icanDev,can,MAX_BUFFER,0);
        if(count <1)
            return false;
        for(int i = 0;i<count;i++){
            can_frame frame = {0};
            frame.can_id = can[i].ID;
            frame.can_dlc = can[i].DataLen;
            for(int j = 0;j<can[i].DataLen;j++){
                frame.data[j] = can[i].Data[j];
            }
            frame_vec.push_back(frame);
        }

    }
    catch(std::exception &ex)
    {
        printf("exception: %s\n",ex);
        return false;
    }
    return true;

}

bool CANDRIVER::canConvertFrame(const struct can_frame *pFrame, VCI_CAN_OBJ *can_obj)
{
    if (pFrame == nullptr)
    {
        return false;
    }
    memset(can_obj, 0, sizeof(VCI_CAN_OBJ));
    can_obj->ID = pFrame->can_id;
    can_obj->SendType = 0;
    can_obj->RemoteFlag = 0;
    can_obj->ExternFlag = 1;
    can_obj->DataLen = pFrame->can_dlc;
    int len = can_obj->DataLen;
    int i = 0;
    for (; i < len; i++)
    {
        can_obj->Data[i] = pFrame->data[i];
    }
    return true;
}
