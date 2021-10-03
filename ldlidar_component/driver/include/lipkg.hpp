/**
* @file         lipkg.h
* @author       LD Robot
* @version      V02

* @brief
* @note
* @attention    COPYRIGHT LDROBOT
* @note Modified by Myzhar@2021
**/

#ifndef __LIPKG_H
#define __LIPKG_H
#include <stdint.h>

#include <array>
#include <iostream>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <vector>

#define ANGLE_TO_RADIAN(angle) ((angle)*3141.59 / 180000)

namespace ldlidar
{
enum
{
  PKG_HEADER = 0x54,
  PKG_VER_LEN = 0x2C,
  POINT_PER_PACK = 12,
};

typedef struct __attribute__((packed))
{
  uint16_t distance;
  uint8_t confidence;
} LidarPointStructDef;

typedef struct __attribute__((packed))
{
  uint8_t header;
  uint8_t ver_len;
  uint16_t speed;
  uint16_t start_angle;
  LidarPointStructDef point[POINT_PER_PACK];
  uint16_t end_angle;
  uint16_t timestamp;
  uint8_t crc8;
} LiDARFrameTypeDef;

struct PointData
{
  float angle;
  uint16_t distance;
  uint8_t confidence;
  double x;
  double y;
  PointData(float angle, uint16_t distance, uint8_t confidence, double x = 0, double y = 0)
  {
    this->angle = angle;
    this->distance = distance;
    this->confidence = confidence;
    this->x = x;
    this->y = y;
  }
  PointData()
  {
  }
  friend std::ostream& operator<<(std::ostream& os, const PointData& data)
  {
    os << data.angle << " " << data.distance << " " << (int)data.confidence << " " << data.x << " " << data.y;
    return os;
  }
};

/**
  \enum UNIT
  \brief Lists available unit for measures.
   */
enum class UNIT
{
  MILLIMETER, /**< International System, 1/1000 METER. */
  CENTIMETER, /**< International System, 1/100 METER. */
  METER       /**< International System, 1 METER [ROS standard]*/
};

/**
  \enum ROTATION
  \brief Lists available rotation verses
   */
enum class ROTATION
{
  CLOCKWISE,       /**< Right thumb looking down  */
  COUNTERCLOCKWISE /**< Right thumb looking up [ROS standard]*/
};

class LiPkg
{
public:
public:
  LiPkg(UNIT unit = UNIT::METER, ROTATION rotVerse = ROTATION::COUNTERCLOCKWISE);
  double GetSpeed(void); /*Lidar spin speed (Hz)*/
  uint16_t GetTimestamp(void)
  {
    return mTimestamp;
  } /*time stamp of the packet */
  bool IsPkgReady(void)
  {
    return mIsPkgReady;
  } /*a packet is ready */
  bool IsFrameReady(void)
  {
    return mFrameReady;
  } /*Lidar data frame is ready*/
  void ResetFrameReady(void)
  {
    mFrameReady = false;
  }
  long GetErrorTimes(void)
  {
    return mErrorTimes;
  } /*the number of errors in parser process of lidar data frame*/
  const std::array<PointData, POINT_PER_PACK>& GetPkgData(void); /*original data package*/
  bool Parse(const uint8_t* data, long len);                     /*parse single packet*/
  bool AssemblePacket(); /*combine stantard data into data frames and calibrate*/
  sensor_msgs::msg::LaserScan GetLaserScan()
  {
    return mOutScan;
  }

protected:
  void ToLaserscan(std::vector<PointData> src);

private:
  uint16_t mTimestamp;
  double mSpeed;
  std::vector<uint8_t> mDataTmp;
  long mErrorTimes;
  std::array<PointData, POINT_PER_PACK> mOnePkg;
  std::vector<PointData> mFrameTmp;
  bool mIsPkgReady;
  bool mFrameReady;
  sensor_msgs::msg::LaserScan mOutScan;

  // ----> Parameters
  double mUnitScale = 1e-3;
  ROTATION mRotVerse = ROTATION::COUNTERCLOCKWISE;
  // <---- Parameters
};
}  // namespace ldlidar
#endif
/********************* (C) COPYRIGHT LD Robot *******END OF FILE ********/
