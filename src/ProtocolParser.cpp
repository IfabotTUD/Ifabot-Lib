/*
 * ProtocolParser.cpp
 *
 *  Created on: 23.10.2015
 *      Author: Dimitri
 */

#include "ProtocolParser.h"

namespace ifabot {

ProtocolParser::ProtocolParser(unsigned char protocolStartID, unsigned int protocolLengthInByte):
		mProtocolStartID(protocolStartID),
		mProtocolLengthInByte(protocolLengthInByte),
		mState(0),
		mLastMessageIndex(0),
		mFirstValidMessageReceived(false),
		mBuffer(),
		mCounter(),
		mListener(0){}

void ProtocolParser::parse(unsigned char c){
	mBuffer.push_back(c);
	switch (mState) {
	case 0:
		mState = (c == mProtocolStartID)?1:0;
		break;

	case 1:
		mState = (c == 0x00)?2:0;
		break;

	case 2:
		mState = (c == 0x00)?3:0;
		break;

	case 3:
		if (c == 0x00) {
			mCounter.push_back(mProtocolLengthInByte-3);
		}
		mState = 0;
		break;
	}

	decrAllCounter();
	handleExpiredCounter();
	
	if(mBuffer.size()>MAX_BUFFER_SIZE){
		handleFullBuffer();
	}
}

void ProtocolParser::decrAllCounter(){
	for(size_t i = 0; i< mCounter.size(); ++i) mCounter[i]--;
}

void ProtocolParser::handleExpiredCounter(){
	for(size_t i = 0; i< mCounter.size(); ++i){
		int counterValue = mCounter[i];
		if(isCounterExpired(counterValue) && mBuffer.size() >= mProtocolLengthInByte){
			std::string message = readMessageWithoutCRCFromBufferEnd();
			if(isMessageValid(message)){
				processValidMessage(message);
			}
		}
	}
}

bool ProtocolParser::isCounterExpired(int count){
	return count == 0;
}

std::string ProtocolParser::readMessageWithoutCRCFromBufferEnd(){
	return std::string(mBuffer.end() - mProtocolLengthInByte, mBuffer.end() - 4);
}

bool ProtocolParser::isMessageValid(std::string message){
	unsigned char calculatedCRC = caluculateCRC(message);
	unsigned char receivedCRC = mBuffer[mBuffer.size() - 4];
	return calculatedCRC==receivedCRC;
}

unsigned char ProtocolParser::caluculateCRC(const std::string& message){
	unsigned char crc = 0;
	for(uint32_t i = 0; i<message.size(); ++i){
		crc ^= message[i];
	}
	return crc;
}

void ProtocolParser::processValidMessage(std::string message){
	std::string cnt = message.substr(4,4);
	int msg_cnt = charToInt((unsigned char)cnt[3],(unsigned char)cnt[2],(unsigned char)cnt[1],(unsigned char)cnt[0]);
	int diff = msg_cnt - mLastMessageIndex;
	if(diff > 1 && mFirstValidMessageReceived){
		mListener->onSkippedPackage(msg_cnt,mLastMessageIndex+1);
	}
	std::vector<int32_t> msg = toIntVector(message);
	mListener->onDataDecoded(msg);
	mLastMessageIndex = msg_cnt;
	mFirstValidMessageReceived = true;
	mBuffer.clear();
	mCounter.clear();
}

const std::vector<int32_t> ProtocolParser::toIntVector(std::string& message) {
	std::vector<int32_t> res;
	for(size_t i = 0; i< message.size(); i+= 4){
		int32_t num = charToInt(message[i+3],message[i+2],message[i+1],message[i]);
		res.push_back(num);
	}
	return res;
}

const int32_t ProtocolParser::charToInt(unsigned char c3,unsigned char c2,unsigned char c1,unsigned char c0) {
	int32_t res = (c3 << 24) + (c2 << 16) + (c1 << 8) + c0;
	return res;
}

void ProtocolParser::handleFullBuffer(){
	using namespace std;
	string buffer = string(mBuffer.begin(), mBuffer.end());
	ostringstream oss;
	for (std::size_t i = 0; i < buffer.size(); ++i) {
		oss << hex << uppercase << bitset<8>((unsigned char)buffer[i]).to_ulong();
		if(i!= buffer.size()-1) oss << " ";
  	}
  	mListener->onUndecodableDatastream(oss.str());
  	mBuffer.clear();
	mCounter.clear();
}

void ProtocolParser::printBits(std::string data){
	using namespace std;
	cout << "Byte Count: " << data.size() << endl;
	cout << "Bin: ";
  	for (std::size_t i = 0; i < data.size(); ++i)
  	{
	      		cout << bitset<8>(data[i]) << " ";
  	}
	cout << endl << "Hex: ";
  	for (std::size_t i = 0; i < data.size(); ++i)
  	{
		std::stringstream res;
		res << hex << uppercase << bitset<8>(data[i]).to_ulong();
		cout << res.str() << " ";
  	}
	cout << endl;
	}

void ProtocolParser::setListener(ProtocolListener* listener){
	mListener = listener;
}

} /* namespace ifabot */
