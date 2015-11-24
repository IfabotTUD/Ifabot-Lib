/*
 * RobotBoard.cpp
 *
 *  Created on: 23.10.2015
 *      Author: Dimitri
 */

#include "RobotBoard.h"

namespace ifabot {

const std::string RobotBoard::DEFAULT_SERIAL_PORT = "/dev/ttyS0";

RobotBoard::RobotBoard():
		Board(	DEFAULT_SERIAL_PORT,
				DEFAULT_BAUD_RATE,
				ROBOTBOARD_PROTOCOL_START_ID,
				ROBOTBOARD_PROTOCOL_LENGTH_IN_BYTES),
				mOnReceiveCallback(0){}

RobotBoard::RobotBoard(std::string port, uint32_t baud):
	Board(	port,
			baud,
			ROBOTBOARD_PROTOCOL_START_ID,
			ROBOTBOARD_PROTOCOL_LENGTH_IN_BYTES),
			mOnReceiveCallback(0){}

bool RobotBoard::connectAndStartReceiving(){
	if(connect()){
		setSendStatusPeriod(DEFAULT_RECEIVE_STATUS_PERIOD_MS);
		return true;
	} else {
		return false;
	}
}

void RobotBoard::setSendStatusPeriod(int32_t cycleTime){
	int32_t rrmk = 0xffffffff; // We always want all the data!
	std::ostringstream oss;
	appendInt(&oss,5);
	appendInt(&oss, cycleTime);
	appendInt(&oss, rrmk);
	processAndSend(&oss);
}

void RobotBoard::setMotorOff(){
	std::ostringstream oss;
	appendInt(&oss,1);
	processAndSend(&oss);
}

void RobotBoard::setVelocity(int32_t linear, int32_t angular){
	std::ostringstream oss;
	appendInt(&oss,2);
	appendInt(&oss,linear);
	appendInt(&oss,angular);
	processAndSend(&oss);
}

void RobotBoard::setPIDCoefficiants(PIDParameter param) {
	std::ostringstream oss;
	appendInt(&oss,3);
	appendInt(&oss,param.PLinearVelocity);
	appendInt(&oss,param.ILinearVelocity);
	appendInt(&oss,param.DLinearVelocity);
	appendInt(&oss,param.PAngularVelocity);
	appendInt(&oss,param.IAngularVelocity);
	appendInt(&oss,param.DAngularVelocity);
	appendInt(&oss,param.ICurrentLeft);
	appendInt(&oss,param.ICurrentRight);
	processAndSend(&oss);
}

void RobotBoard::setUltrasonicParameters(UltrasonicParameter param) {
	std::ostringstream oss;
	appendInt(&oss,4);
	appendInt(&oss,param.leftRange);
	appendInt(&oss,param.leftGain);
	appendInt(&oss,param.centerRange);
	appendInt(&oss,param.centerGain);
	appendInt(&oss,param.rightRange);
	appendInt(&oss,param.rightGain);
	appendInt(&oss,param.backRange);
	appendInt(&oss,param.backGain);
	processAndSend(&oss);
}

void RobotBoard::setOnDataReceivedCallback(void (*onReceiveCallback)(RobotBoardStatus)){
	mOnReceiveCallback = onReceiveCallback;
}

// @Override
void RobotBoard::onDataDecoded(std::vector<int32_t> &data){
	RobotBoardStatus status;
	status.sequenceNumber = data[1];
	status.linearVelocity = data[2];
	status.angularVelocity = data[3];
	status.timestampVelocity = data[4];
	status.motorCurrentLeft = data[5];
	status.timestampMotorCurrentLeft = data[6];
	status.motorCurrentRight = data[7];
	status.timestampMotorCurrentRight = data[8];
	status.accuVoltage = data[9];
	status.timestampAccuVoltage = data[10];
	status.tickCountRight = data[11];
	status.timestampTickCountRight = data[12];
	status.tickCountLeft = data[13];
	status.timestampTickCountLeft = data[14];
	status.ultrasonic.left = data[15];
	status.ultrasonic.timestampLeft = data[16];
	status.ultrasonic.center = data[17];
	status.ultrasonic.timestampCenter = data[18];
	status.ultrasonic.right = data[19];
	status.ultrasonic.timestampRight = data[20];
	status.ultrasonic.back = data[21];
	status.ultrasonic.timestampBack = data[22];
	status.status.status = data[23];
	status.status.timestamp = data[24];
	if(mOnReceiveCallback) mOnReceiveCallback(status);
}

} /* namespace ifabot */
