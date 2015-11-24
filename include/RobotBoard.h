/*
 * RobotBoard.h
 *
 *  Created on: 23.10.2015
 *      Author: Dimitri
 */

#ifndef ROBOTBOARD_H_
#define ROBOTBOARD_H_

#include <iostream>
#include <stdint.h>
#include "SerialCommunication.h"
#include "ProtocolParser.h"
#include "Board.h"

namespace ifabot {

struct UltrasonicSensorData{
	int32_t left; // in 43 mm steps
	uint32_t timestampLeft; // in ms
	int32_t center; // in 43 mm steps
	uint32_t timestampCenter; // in ms
	int32_t right; // in 43 mm steps
	uint32_t timestampRight; // in ms
	int32_t back; // in 43 mm steps
	uint32_t timestampBack; // in ms
};

struct RobotBoardStatus{
	uint32_t sequenceNumber;
	int32_t linearVelocity; // in 10^-3 m/s
	int32_t angularVelocity;// in 10^-3 m/s
	uint32_t timestampVelocity; // in ms
	int32_t motorCurrentLeft; // in mA
	uint32_t timestampMotorCurrentLeft; // in ms
	int32_t motorCurrentRight; // in mA
	uint32_t timestampMotorCurrentRight; // in ms
	int32_t accuVoltage; // in mV
	int32_t timestampAccuVoltage; // in ms
	int32_t tickCountRight; // 1024 ticks = 1 motor rotation, 44032 ticks = one wheel rotation
	uint32_t timestampTickCountRight; // in ms
	int32_t tickCountLeft; // 1024 ticks = 1 motor rotation, 44032 ticks = one wheel rotation
	uint32_t timestampTickCountLeft; // in ms
	UltrasonicSensorData ultrasonic;
	StatusData status;
};

struct PIDParameter{
	int32_t PLinearVelocity;
	int32_t ILinearVelocity;
	int32_t DLinearVelocity;
	int32_t PAngularVelocity;
	int32_t IAngularVelocity;
	int32_t DAngularVelocity;
	int32_t ICurrentLeft;
	int32_t ICurrentRight;
};

struct UltrasonicParameter{
	int32_t leftRange;
	int32_t leftGain;
	int32_t centerRange;
	int32_t centerGain;
	int32_t rightRange;
	int32_t rightGain;
	int32_t backRange;
	int32_t backGain;
};

class RobotBoard : public Board {
public:
	static const std::string DEFAULT_SERIAL_PORT;
	static const uint32_t DEFAULT_BAUD_RATE = 115200;
	static const uint32_t DEFAULT_RECEIVE_STATUS_PERIOD_MS = 50;

	RobotBoard();
	RobotBoard(std::string port, uint32_t baud);
	bool connectAndStartReceiving();
	void setSendStatusPeriod(int32_t cycleTime);
	void setMotorOff();
	void setVelocity(int32_t linear, int32_t angular);
	void setPIDCoefficiants(PIDParameter param);
	void setUltrasonicParameters(UltrasonicParameter param);
	void setOnDataReceivedCallback(void (*onReceiveCallback)(RobotBoardStatus));
private:
	void onDataDecoded(std::vector<int32_t> &data);

	static const unsigned char ROBOTBOARD_PROTOCOL_START_ID = 0x06;
	static const unsigned int ROBOTBOARD_PROTOCOL_LENGTH_IN_BYTES = 104;
	void (*mOnReceiveCallback)(RobotBoardStatus);
};

} /* namespace ifabot */

#endif /* ROBOTBOARD_H_ */
