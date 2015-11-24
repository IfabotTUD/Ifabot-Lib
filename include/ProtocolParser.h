/*
 * MessageParser.h
 *
 *  Created on: 23.10.2015
 *      Author: Elena
 */

#ifndef PROTOCOLPARSER_H_
#define PROTOCOLPARSER_H_


#include <iostream>
#include <vector>
#include <stdint.h>
#include <bitset>
#include <sstream>

#include "ProtocolListener.h"

namespace ifabot {

class ProtocolParser {
public:
	ProtocolParser(unsigned char protocolStartID, unsigned int protocolLengthInByte);
	void parse(unsigned char c);
	void setListener(ProtocolListener* listener);
private:
	static const int MAX_BUFFER_SIZE = 2000;
	void decrAllCounter();
	void handleExpiredCounter();
	bool isCounterExpired(int count);
	std::string readMessageWithoutCRCFromBufferEnd();
	bool isMessageValid(std::string message);
	unsigned char caluculateCRC(const std::string& message);
	void processValidMessage(std::string message);
	void handleFullBuffer();
	static const std::vector<int32_t> toIntVector(std::string& message);
	static const int32_t charToInt(unsigned char c3,unsigned char c2,unsigned char c1,unsigned char c0);
	void printBits(std::string data);
	unsigned char mProtocolStartID;
	unsigned int mProtocolLengthInByte;
	unsigned int mState;
	int mLastMessageIndex;
	bool mFirstValidMessageReceived;
	std::vector<unsigned char> mBuffer;
	std::vector<int> mCounter;
	ProtocolListener* mListener;
};

} /* namespace ifabot */

#endif /* PROTOCOLPARSER_H_ */
