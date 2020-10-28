### 功能
这个package是一个将gps转换到utm坐标系并以里程计的形式发布的package

### utm坐标系
UTM(UNIVERSALTRANSVERSEMERCARTORGRIDSYSTEM，通用横墨卡托格网系统）坐标是一种平面直角坐标，这种坐标格网系统及其所依据的投影已经广泛用于地形图，作为卫星影像和自然资源数据库的参考格网以及要求精确定位的其他应用。在UTM系统中，北纬84度和南纬80度之间的地球表面积按经度6度划分为南北纵带（投影带）。从180度经线开始向东将这些投影带编号，从1编至60(北京处于第50带）。每个带再划分为纬差8度的四边形。四边形的横行从南纬80度开始。用字母C至X（不含I和O）依次标记（第X行包括北半球从北纬72度至84度全部陆地面积，共12度）每个四边形用数字和字母组合标记。参考格网向右向上读取。每一四边形划分为很多边长为1000000米的小区，用字母组合系统标记。在每个投影带中，位于带中心的经线，赋予横坐标值为500000米。对于北半球赤道的标记坐标值为0，对于南半球为10000000米，往南递减。

### 发布的话题
 /utm_odom

### 消息格式
    odom.pose.pose.position.x = easting;	（gps经度）
    odom.pose.pose.position.y = northing;	（gps维度）
    odom.pose.pose.position.z = 0;		（由于是平面坐标系，去掉高度信息）
    
    odom.pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(0,0,theta);	（为IMU转换的数据）
