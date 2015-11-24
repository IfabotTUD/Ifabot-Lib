/*
 * IMUBoard.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: dimitri
 */

#include "IMUBoard.h"

namespace ifabot {

const std::string IMUBoard::DEFAULT_SERIAL_PORT = "/dev/ttyS1";

IMUBoard::IMUBoard() :
		Board(	DEFAULT_SERIAL_PORT,
				DEFAULT_BAUD_RATE,
				IMUBOARD_PROTOCOL_START_ID,
				IMUBOARD_PROTOCOL_LENGTH_IN_BYTES),
				mOnReceiveCallback(0){}

IMUBoard::IMUBoard(std::string port, uint32_t baud) :
		Board(	port,
				baud,
				IMUBOARD_PROTOCOL_START_ID,
				IMUBOARD_PROTOCOL_LENGTH_IN_BYTES),
				mOnReceiveCallback(0){}

bool IMUBoard::connectAndStartReceiving(){
	if(connect()){
		setSendStatusPeriod(DEFAULT_RECEIVE_STATUS_PERIOD_MS);
		return true;
	} else {
		return false;
	}
}

void IMUBoard::setSendStatusPeriod(int32_t cycleTime){
	int32_t irmk = 0xffffffff; // We always want all the data!
	std::ostringstream oss;
	appendInt(&oss,14);
	appendInt(&oss, cycleTime);
	appendInt(&oss, irmk);
	processAndSend(&oss);
}

void IMUBoard::setVelocityCorrection(VelocityCorrection correction){
	std::ostringstream oss;
	appendInt(&oss,11);
	appendInt(&oss,correction.velocityX);
	appendInt(&oss,correction.velocityY);
	appendInt(&oss,correction.velocityZ);
	appendInt(&oss,correction.parameterMask);
	processAndSend(&oss);
}

void IMUBoard::startGyroscopeCalibration(uint32_t duration){
	std::ostringstream oss;
	appendInt(&oss,12);
	appendInt(&oss,duration);
	processAndSend(&oss);
}

void IMUBoard::setAngles(AngleCommand angles){
	std::ostringstream oss;
	appendInt(&oss,13);
	appendInt(&oss,angles.angleGyro);
	appendInt(&oss,angles.imuX);
	appendInt(&oss,angles.imuY);
	appendInt(&oss,angles.imuZ);
	processAndSend(&oss);
}


void IMUBoard::setOnDataReceivedCallback(void (*onReceiveCallback)(IMUBoardStatus)){
	mOnReceiveCallback = onReceiveCallback;
}

// @Override
void IMUBoard::onDataDecoded(std::vector<int32_t> &data){
	IMUBoardStatus status;
	status.sequenceNumber = data[1];
	status.gyrosensor.angle = data[2];
	status.gyrosensor.angularVelocity = data[3];
	status.gyrosensor.timestamp = data[4];
	status.accelerometer.accelerationX = data[5];
	status.accelerometer.accelerationY = data[6];
	status.accelerometer.accelerationZ = data[7];
	status.accelerometer.velocityX = data[8];
	status.accelerometer.velocityY = data[9];
	status.accelerometer.velocityZ = data[10];
	status.accelerometer.timestamp = data[11];
	status.imu.angleX = data[12];
	status.imu.angleY = data[13];
	status.imu.angleZ = data[14];
	status.imu.angularVelocityX = data[15];
	status.imu.angularVelocityY = data[16];
	status.imu.angularVelocityZ = data[17];
	status.imu.timestampAnglesAngularVelocity = data[18];
	status.imu.accelerationX = data[19];
	status.imu.accelerationY = data[20];
	status.imu.accelerationZ = data[21];
	status.imu.velocityX = data[22];
	status.imu.velocityY = data[23];
	status.imu.velocityZ = data[24];
	status.imu.timestampAccelerationVelocity = data[25];
	status.temperature.temperature = data[26];
	status.temperature.timestamp = data[27];
	status.status.status = data[28];
	status.status.timestamp = data[29];
	mOnReceiveCallback(status);
}

} /* namespace ifabot */
