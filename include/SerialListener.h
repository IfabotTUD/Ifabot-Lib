/*
 * SerialListener.h
 *
 *  Created on: 23.10.2015
 *      Author: Elena
 */

#ifndef SERIALLISTENER_H_
#define SERIALLISTENER_H_

#include <iostream>

namespace ifabot {

class SerialListener {
public:
	virtual ~SerialListener(){};
	virtual void onDataReceived(std::string &data) = 0;
};

} /* namespace ifabot */

#endif /* SERIALLISTENER_H_ */
