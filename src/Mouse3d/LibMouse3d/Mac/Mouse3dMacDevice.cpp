#include "Mouse3d/LibMouse3d/Mac/Mouse3dMacDevice.h"

#include <QDebug>
#include <assert>

namespace Mouse3dMacDeviceGlob{
	
	Mouse3dMacDevice *ptr = nullptr;
	
}

Mouse3dMacDevice::Mouse3dMacDevice(): clientID(0) {
	assert(Mouse3dMacDeviceGlob::ptr != nullptr);
	Mouse3dMacDeviceGlob::Mouse3dMacDevice = this;
	
	InstallConnexionHandlers( MouseHandler, 0L, 0L );
	uint32_t signature = kConnexionClientWildcard;
    uint8_t *name = nullptr;
    uint16_t mode = kConnexionClientModeTakeOver;
    uint32_t mask = kConnexionMaskAll;
    this->clientID = RegisterConnexionClient(signature, name, mode, mask);
	qDebug() << "Mouse3dDevice: Registered with ClientID=" << this->clientID;
}

Mouse3dMacDevice::~Mouse3dMacDevice() {
	UnregisterConnexionClient(this->clientID);
	CleanupConnexionHandlers();
	qDebug() << "Mouse3dDevice: Unregistered";
	Mouse3dMacDeviceGlob::Mouse3dMacDevice = nullptr;
}

void MouseHandler(unsigned int connection, unsigned int messageType, void *messageArgument) {
	if (messageType == kConnexionMsgDeviceState) {
		ConnexionDeviceState *s;
		s = static_cast<ConnexionDeviceState *>(messageArgument);
		
		if (s->command == kConnexionCmdHandleAxis) {
			std::vector<float> motionData;
			motionData.push_back(s->axis[0]); // X
			motionData.push_back(-s->axis[1]); // Y
			motionData.push_back(-s->axis[2]); // Z
			
			motionData.push_back(s->axis[3]); // A
			motionData.push_back(s->axis[4]); // B
			motionData.push_back(s->axis[5]); // C
			
			qDebug() <<  "Mouse3dDevice: x=" << motionData[0] <<
						"y=" << motionData[1] <<
						"z=" << motionData[2] <<
						"a=" << motionData[3] <<
						"b=" << motionData[4] <<
						"c=" << motionData[5];
						
			Mouse3dMacDeviceGlob::ptr->PassMotion( motionData );
		}
	}
}