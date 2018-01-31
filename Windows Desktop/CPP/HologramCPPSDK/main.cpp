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
		MODEM_INFO info;
		nova.getInfo(info);
		std::wstring imsi;
		nova.getIMSI(imsi);
		nova.connect();
		auto status = nova.getConnectionStatus();
		std::vector<std::string> resultArray;
		std::string input = "";
		while (input != "exit") {
			std::cout << "AT Command? ";
			std::getline(std::cin, input);
			nova.sendAndParseATCommand(input, resultArray);
			for (auto result : resultArray) {
				std::cout << "Result: " << result << std::endl;
			}
		}
		nova.disconnect();
	}
	return 0;
}