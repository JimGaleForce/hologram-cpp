#include "CustomCloud.h"

CustomCloud::CustomCloud(std::string sendHost, int sendPort, std::string recvHost, int recvPort) : Cloud(sendHost, sendPort, recvHost, recvPort)
{
	useATSocket = true;
}

CustomCloud::~CustomCloud()
{
}

bool CustomCloud::isReadyToSend()
{
	return networkManager.getNetwork() == NULL || networkManager.getNetwork()->isConnected();
}

std::string CustomCloud::sendMessage(std::string message, int timeout, bool closeSocket)
{
	if (!isReadyToSend()) {
		addPayloadToBuffer(message);
		return "";
	}
	std::string result;
	openSendSocket(timeout);

	if (useATSocket) {
		result = networkManager.getNetwork()->sendMessage(message);
		EventBus::FireEvent(MessageSentEvent(message, true));
	}
	else {
		result = receiveSendSocket();
		EventBus::FireEvent(MessageSentEvent(message, false));
	}

	if (closeSocket) {
		closeSendSocket();
	}
	return result;
}

void CustomCloud::openSendSocket(int timeout)
{
	if (sendSocketOpen) {
		return;
	}

	if (useATSocket) {
		networkManager.getNetwork()->createSocket();
		networkManager.getNetwork()->connectSocket(sendHost, sendPort);
	}
	else {
		//native socket impl
	}
	sendSocketOpen = true;
}

void CustomCloud::closeSendSocket()
{
	if (useATSocket) {
		networkManager.getNetwork()->closeSocket();
	}
	else {
		//native sockets
	}
	sendSocketOpen = false;
}

std::string CustomCloud::receiveSendSocket(unsigned int maxBytesRead)
{
	std::string result;
	//native socket goes here
	return result;
}

void CustomCloud::openReceiveSocket()
{
	if (useATSocket) {
		networkManager.getNetwork()->openReceiveSocket(recvPort);
	}
	else {

	}
}

void CustomCloud::closeReceiveSocket()
{
	if (useATSocket) {
		networkManager.getNetwork()->closeSocket();
	}
	else {

	}
}
