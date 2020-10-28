#include <iostream>
#include <sensor_msgs/NavSatFix.h>
#include <sstream> //使用stringstream需要引入这个头文件
#include <vector>

#include "ub482/GPS.h"

struct GPS::Data
{
	double timeStamp;
	uint32_t seq;
	double longitude;
	double latitude;
	double altitude;
	double heading;
	uint8_t status;

	ros::Publisher gpsPublisher;

	Data(ros::Publisher &gpsPub)
	{
		timeStamp = 0;
		seq = 0;
		longitude = 0;
		latitude = 0;
		altitude = 0;
		heading = 0;
		status = 0; //状态，A=定位，V=未定位
		gpsPublisher = gpsPub;
	}
};

GPS::GPS(ros::Publisher &gpsPub)
{
	pData_ = std::make_shared<Data>(gpsPub);
}

GPS::~GPS()
{
}

/**
 * @brief 将字符串转换为数字
 * 
 * @tparam Type double float int
 * @param str 
 * @return Type 
 */
template <class Type>
Type stringToNum(const std::string &str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

/**
 * @brief 度分格式转换成为度格式
 * 
 * @param dm 
 * @return double 
 */
double convert_dm2d(double dm)
{
	double d = dm / 100;
	int d_int = (int)d;
	double d_remain = (d - d_int) * 100.0 / 60.0;
	double reval = d_int + d_remain;
	return reval;
}

void splitString(const std::string &str, const std::string &delimit, std::vector<std::string> &strVec)
{
	std::string::size_type pos1, pos2;
	pos2 = str.find(delimit);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		strVec.push_back(str.substr(pos1, pos2 - pos1));

		pos1 = pos2 + delimit.size();
		pos2 = str.find(delimit, pos1);
	}
	if (pos1 != str.length())
		strVec.push_back(str.substr(pos1));
}

void GPS::processGpsStr(const std::string &str)
{
	std::vector<std::string> strVec;
	splitString(str, ",", strVec);


	if((strVec[0] == "$GPGGA") || (strVec[0] == "$GNGGA")) {
		GPGGA = str;

		pData_->status = stringToNum<int>(strVec[6]);
	}

	if (strVec[0] == "#HEADINGA") {
		HEADINGA = str;
		pData_->heading = stringToNum<double>(strVec[12]);

	}

	if ((strVec[0] == "$GPRMC") || (strVec[0] == "$GNRMC")) {
		GPRMC = str;
		pData_->timeStamp = stringToNum<double>(strVec[1]);

		pData_->seq++;
		pData_->latitude = convert_dm2d(stringToNum<double>(strVec[3]));
		pData_->longitude = convert_dm2d(stringToNum<double>(strVec[5]));
		// pData_->status = 1;
		
		sensor_msgs::NavSatFix gpsMsg;
		gpsMsg.header.seq = pData_->seq;
		gpsMsg.header.stamp = ros::Time::now();
		gpsMsg.header.frame_id = "gps";
		gpsMsg.status.status = pData_->status;
		gpsMsg.latitude = pData_->latitude;
		gpsMsg.longitude = pData_->longitude;
		gpsMsg.altitude = pData_->heading;

		pData_->gpsPublisher.publish(gpsMsg);

		std::stringstream lat;
		std::stringstream lng;

		lat << std::setprecision(15) << pData_->latitude;
		lng << std::setprecision(15) << pData_->longitude;

		gps_string = "{\"lat\":" + lat.str() + ",\"lng\":" + lng.str() + "}";
	}
}

static std::vector<char> tmpVec;
bool GPS::processBuf(int bufLen, char *buf, std::vector<std::string> &strVec)
{
	// bool ret = false;
	strVec.clear();
	for (int i = 0; i < bufLen; i++)
	{
		if ('\n' != buf[i])
		{
			tmpVec.emplace_back(buf[i]);
		}
		else
		{
			if (!tmpVec.empty() && '$' == tmpVec[0])
			{
				std::string str;
				str.assign(tmpVec.begin(), tmpVec.end());
				if (tmpVec.empty())
				{
					continue;
				}
				strVec.emplace_back(str);
			} else if (!tmpVec.empty() && '#' == tmpVec[0]) {
				std::string str;
				str.assign(tmpVec.begin(), tmpVec.end());
				if (tmpVec.empty())
				{
					continue;
				}
				strVec.emplace_back(str);
 			}
				tmpVec.clear();
		}
	}
	return (!strVec.empty());
}
