#include "Nova.h"
#include <iostream>

int main()
{
	Modem::getConnectionProfiles();
	auto modems = Modem::getConnectedModems();
	if (modems.size() > 0) {
		Nova nova;
		nova.setupCellularDataConnection(modems.at(0)->szDeviceName, TEXT("Hologram Nova"));
		for (auto device : Serial::getConnectedSerialDevices()) {
			if (wcsstr(device.friendlyName.c_str(), L"AT and Data") != nullptr) {
				if (nova.setupSerialPort(device.portName, 9600)) {
					break;
				}
			}
		}
		nova.connect();
		auto status = nova.getConnectionStatus();
		std::vector<std::string> resultArray;
		std::string input = "";
		while (input != "exit") {
			std::cout << "AT Command? ";
			std::getline(std::cin, input);
			if (nova.sendAndParseATCommand(input, resultArray)) {
				for (auto result : resultArray) {
					std::cout << "Result: " << result << std::endl;
				}
			}
			else {
				std::cout << "Sending AT Command Failed" << std::endl;
				for (auto result : resultArray) {
					std::cout << "Result: " << result << std::endl;
				}
			}
			resultArray.clear();
		}
		nova.disconnect();
	}
	return 0;
}