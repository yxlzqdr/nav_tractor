/***
  * @projectName  canpublish
  * @brief can communication driver
  * @author bruce geng
  * @date 2020-06-04
  */
#ifndef CANDRIVER_H
#define CANDRIVER_H
#include "thirdparty/include/controlcan.h"
#include <linux/can.h>
#include <vector>


class CANDRIVER
{
public:
    CANDRIVER(unsigned int _idevType,unsigned int _idevIndex,unsigned int _icanDev,unsigned int _iBaud);
    ~CANDRIVER();
    void initCan();
    bool openDevice(unsigned int idevType,unsigned int idevIndex, unsigned int iCanInd);
    bool canTransimit(const can_frame *frame);
    bool canReceive(std::vector<can_frame>&frame_vec);
private:
    bool canConvertFrame(const struct can_frame *pFrame, VCI_CAN_OBJ *can_obj);
    unsigned int idevType;
    unsigned int idevIndex;
    unsigned int icanDev;
    unsigned int iBaud;
    const unsigned int MAX_BUFFER = 512;
    bool bopen;
};

#endif // CANDRIVER_H
