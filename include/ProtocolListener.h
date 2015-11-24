/*
 * ProtocolListener.h
 *
 *  Created on: Oct 25, 2015
 *      Author: dimitri
 */

#ifndef PROTOCOLLISTENER_H_
#define PROTOCOLLISTENER_H_

namespace ifabot {

class ProtocolListener {
public:
	virtual ~ProtocolListener(){};
	virtual void onDataDecoded(std::vector<int32_t> &data) = 0;
	virtual void onSkippedPackage(uint32_t receivedSequenceNumber, uint32_t expectedSequenceNumber) = 0;
	virtual void onUndecodableDatastream(std::string hexdump) = 0;
};

} /* namespace ifabot */

#endif /* PROTOCOLLISTENER_H_ */
