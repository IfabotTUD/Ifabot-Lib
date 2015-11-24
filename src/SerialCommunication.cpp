/*
 * SerialCommunication.cpp
 *
 *  Created on: 23.10.2015
 *      Author: Dimitri
 */

#include "SerialCommunication.h"

namespace ifabot {

SerialCommunication::SerialCommunication(const std::string& portName, int baud):
	mPortName(portName),
	mBaud(baud),
	mIO(),
	mPort(mIO),
	mListener(0) {
}

SerialCommunication::~SerialCommunication(){
	if(mPort.is_open()){
		disconnect();
	}
}

bool SerialCommunication::connect() {
	using namespace boost::asio;
	mPort.open(mPortName);
	mPort.set_option(serial_port::baud_rate(mBaud));
	mPort.set_option(serial_port::flow_control(serial_port::flow_control::none));
	if(mPort.is_open()){
		startReceive();
		mBackgroundThread = boost::thread( boost::bind(&boost::asio::io_service::run, &mIO));
	}
	return mPort.is_open();
}

void SerialCommunication::disconnect(){
	mIO.stop();
	mBackgroundThread.join();
}

bool SerialCommunication::isConnected(){
	return mPort.is_open();
}

void SerialCommunication::writeBlocking(const std::string& data){
	boost::asio::write(mPort, boost::asio::buffer(data));
}

void SerialCommunication::setListener(SerialListener* listener){
	mListener = listener;
}

void SerialCommunication::startReceive(){
	using namespace boost::asio;
	mPort.async_read_some(
			buffer(mBuffer),
			boost::bind(
					&SerialCommunication::onData,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)
	);
}

void SerialCommunication::onData(const boost::system::error_code& e, std::size_t size) {
	std::string data(size,' ');
	for(size_t i = 0; i < size; ++i) data[i] = mBuffer[i];
	mListener->onDataReceived(data);
	startReceive();
}

} /* namespace ifabot */
