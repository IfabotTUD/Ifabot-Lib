/*
 * Board.h
 *
 *  Created on: Oct 25, 2015
 *      Author: dimitri
 */

#ifndef BOARD_H_
#define BOARD_H_


#include <iostream>
#include <stdint.h>

#include "SerialCommunication.h"
#include "ProtocolParser.h"


enum ErrorType{LOST_CONNECTION, LOST_PACKAGE, UNDECODABLE_DATASTREAM};


namespace ifabot {

struct StatusData{
	uint32_t status;
	uint32_t timestamp;
};

class Board : public SerialListener, public ProtocolListener {
public:
	Board(std::string serialPortName, uint32_t baud, unsigned char protocolID, unsigned int protocolLength);
	bool connect();
	bool isConnected();
	void disconnect();
	void setOnErrorCallback(void (*onErrorCallback)(ErrorType,std::string));
protected:
	static const void appendInt(std::ostringstream* oss, int32_t value);
	void processAndSend(std::ostringstream* oss);
	static const unsigned char caluculateCRC(const std::string& message);
	void onDataReceived(std::string &data);
	virtual void onDataDecoded(std::vector<int32_t> &data) = 0;
	void onSkippedPackage(uint32_t receivedSequenceNumber, uint32_t expectedSequenceNumber);
	void onUndecodableDatastream(std::string hexdump);

	SerialCommunication mSerial;
	ProtocolParser mProtocolParser;
	void (*mOnErrorCallback)(ErrorType,std::string);
};

} /* namespace ifabot */

#endif /* BOARD_H_ */
