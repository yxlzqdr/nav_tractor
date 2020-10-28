#ifndef GPS_H
#define GPS_H

#include <memory>
#include <vector>
#include <ros/ros.h>
#include <sstream>

class GPS
{
  public:
	struct Data;
	std::string GPGGA;
	std::string GNHDT;
	std::string GPRMC;
	std::string GPHCD;
	std::string HEADINGA;
	std::string gps_string;
	
	GPS(ros::Publisher &gpsPub);
	~GPS();
	void processGpsStr(const std::string &str);
	bool processBuf(int i, char* buf, std::vector<std::string> &strVec);

  private:
	std::shared_ptr<Data> pData_;
	
	/* data */
};

#endif /* ifndef GPS */
