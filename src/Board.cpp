/*
 * Board.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: dimitri
 */

#include "Board.h"

namespace ifabot {

Board::Board(std::string serialPortName, uint32_t baud, unsigned char protocolID, unsigned int protocolLength) :
			mSerial(serialPortName, baud),
			mProtocolParser(protocolID, protocolLength),
			mOnErrorCallback(0) {
			mSerial.setListener(this);
			mProtocolParser.setListener(this);
}

bool Board::connect(){
	return mSerial.connect();
}

void Board::disconnect(){
	mSerial.disconnect();
}

bool Board::isConnected(){
	return mSerial.isConnected();
}

const void Board::appendInt(std::ostringstream* oss, int32_t value){
	char* ptr = (char*) &value;
	for(uint32_t i = 0; i< 4; ++i) {
		*oss << ptr[i];
	}
}

void Board::processAndSend(std::ostringstream* oss) {
	*oss << caluculateCRC(oss->str()) << (char)0 << (char)0 << (char)0;
	std::string message = oss->str();
	if(mSerial.isConnected()){
		mSerial.writeBlocking(message);
	} else {
		mOnErrorCallback(LOST_CONNECTION,"");
	}
}

const unsigned char Board::caluculateCRC(const std::string& message){
	unsigned char crc = 0;
	for(uint32_t i = 0; i<message.size(); ++i){
		crc ^= message[i];
	}
	return crc;
}

void Board::setOnErrorCallback(void (*onErrorCallback)(ErrorType,std::string)){
	mOnErrorCallback = onErrorCallback;
}

// @Override
void Board::onSkippedPackage(uint32_t receivedSequenceNumber, uint32_t expectedSequenceNumber){
	std::ostringstream oos;
	oos << "Expected package with sequence number: " << expectedSequenceNumber;
	oos << " but reiceved package number: " << receivedSequenceNumber;
	int lost = receivedSequenceNumber-expectedSequenceNumber;
	oos << ". Lost " << lost << " Package" << ((lost > 1)?"s":"") << ".";
	mOnErrorCallback(LOST_PACKAGE, oos.str());
}

// @Override
void Board::onUndecodableDatastream(std::string hexdump) {
	mOnErrorCallback(UNDECODABLE_DATASTREAM, hexdump);
}

// @Override
void Board::onDataReceived(std::string &data) {
	for(size_t i = 0; i < data.length(); ++i)
		mProtocolParser.parse((unsigned char)data[i]);
}


} /* namespace ifabot */
