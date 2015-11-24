/*
 * IMUBoard.h
 *
 *  Created on: Oct 25, 2015
 *      Author: dimitri
 */

#ifndef IMUBOARD_H_
#define IMUBOARD_H_

#include "Board.h"

namespace ifabot {

struct AngleCommand{
	int32_t angleGyro;
	int32_t imuX;
	int32_t imuY;
	int32_t imuZ;
};

struct VelocityCorrection{
	int32_t velocityX;
	int32_t velocityY;
	int32_t velocityZ;
	uint32_t parameterMask;
};

struct GyrosensorData{
	int32_t angle;
	int32_t angularVelocity;
	uint32_t timestamp;
};

struct AccelerometerData{
	int32_t accelerationX;
	int32_t accelerationY;
	int32_t accelerationZ;
	int32_t velocityX;
	int32_t velocityY;
	int32_t velocityZ;
	uint32_t timestamp;
};

struct IMUData{
	int32_t angleX;
	int32_t angleY;
	int32_t angleZ;
	int32_t angularVelocityX;
	int32_t angularVelocityY;
	int32_t angularVelocityZ;
	uint32_t timestampAnglesAngularVelocity;
	int32_t accelerationX;
	int32_t accelerationY;
	int32_t accelerationZ;
	int32_t velocityX;
	int32_t velocityY;
	int32_t velocityZ;
	uint32_t timestampAccelerationVelocity;
};

struct TemperatureData{
	int32_t temperature;
	uint32_t timestamp;
};

struct IMUBoardStatus {
	uint32_t sequenceNumber;
	GyrosensorData gyrosensor;
	AccelerometerData accelerometer;
	IMUData imu;
	TemperatureData temperature;
	StatusData status;
};

class IMUBoard: public Board {
public:
	static const std::string DEFAULT_SERIAL_PORT;
	static const uint32_t DEFAULT_BAUD_RATE = 115200;
	static const uint32_t DEFAULT_RECEIVE_STATUS_PERIOD_MS = 50;

	IMUBoard();
	IMUBoard(std::string port, uint32_t baud);
	bool connectAndStartReceiving();
	void setSendStatusPeriod(int32_t cycleTime);
	void setVelocityCorrection(VelocityCorrection correction);
	void startGyroscopeCalibration(uint32_t duration);
	void setAngles(AngleCommand angles);
	void setOnDataReceivedCallback(void (*onReceiveCallback)(IMUBoardStatus));
private:
	void onDataDecoded(std::vector<int32_t> &data);

	static const unsigned char IMUBOARD_PROTOCOL_START_ID = 0x0f;
	static const unsigned int IMUBOARD_PROTOCOL_LENGTH_IN_BYTES = 124;
	void (*mOnReceiveCallback)(IMUBoardStatus);
};

} /* namespace ifabot */

#endif /* IMUBOARD_H_ */
