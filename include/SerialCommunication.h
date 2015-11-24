/*
 * SerialCommunication.h
 *
 *  Created on: 23.10.2015
 *      Author: Dimitri
 */

#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "SerialListener.h"

namespace ifabot {

class SerialCommunication {
public:
	SerialCommunication(const std::string& portName, int baud);
	~SerialCommunication();
	bool connect();
	void disconnect();
	bool isConnected();
	void writeBlocking(const std::string& data);
	void setListener(SerialListener* listener);
private:
	void startReceive();
	void onData(const boost::system::error_code& e, std::size_t size);
	static const int BUFFER_SIZE = 100;
	static const int TIMEOUT_IN_SECONDS = 1;
	std::string mPortName;
	int mBaud;
	boost::asio::io_service mIO;
	boost::asio::serial_port mPort;
	boost::thread mBackgroundThread;
	unsigned char mBuffer[BUFFER_SIZE];
	SerialListener* mListener;
};

} /* namespace ifabot */

#endif /* SERIALCOMMUNICATION_H_ */
