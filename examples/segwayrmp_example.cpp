#include <iostream>
#include <string.h>

#include "segwayrmp.h"

void handleSegwayStatus(segwayrmp::SegwayStatus &ss) {
    // std::cout << ss.str() << std::endl << std::endl;
}

int run_segway(segwayrmp::InterfaceType interface_type, std::string configuration, int config_type = 0) {
    try {
        segwayrmp::SegwayRMP rmp(interface_type);
        if (interface_type == segwayrmp::serial) {
            rmp.configureSerial(configuration);
        } else if (interface_type == segwayrmp::usb) {
            switch (config_type) {
                case 1:
                    rmp.configureUSBBySerial(configuration);
                    break;
                case 2:
                    rmp.configureUSBByDescription(configuration);
                    break;
                case 3:
                    rmp.configureUSBByIndex(atoi(configuration.c_str()));
                    break;
                case 0:
                default:
                    std::cout << "Invalid interface type provided." << std::endl;
                    return 0;
            }
        }
        rmp.setStatusCallback(handleSegwayStatus);
        rmp.connect();
        while(true) {
            rmp.move(0.3, 0);
            usleep(100000);
        }
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void print_usage() {
    std::cout << "Usage: segwayrmp_example usb <serial_number | description | index> <\"00000056\" | \"Robotic Mobile Platform\" | 0>" << std::endl;
    std::cout << "       or" << std::endl;
    std::cout << "       segwayrmp_example serial <serial port>" << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "       segwayrmp_example usb serial_number \"00000056\"" << std::endl;
    std::cout << "       segwayrmp_example serial /dev/ttyUSB0" << std::endl;
    std::cout << "       segwayrmp_example serial COM0" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 0;
    }
    if (std::strcmp(argv[1], "serial") == 0) {
        if (argc < 3) {
            print_usage();
            return 0;
        }
        run_segway(segwayrmp::serial, std::string(argv[2]));
    } else if (std::strcmp(argv[1], "usb") == 0) {
        if (argc < 4) {
            print_usage();
            return 0;
        }
        
        if (std::strcmp(argv[2], "serial_number") == 0) {
            run_segway(segwayrmp::usb, std::string(argv[3]), 1);
        } else if (std::strcmp(argv[2], "description") == 0) {
            run_segway(segwayrmp::usb, std::string(argv[3]), 2);
        } else if (std::strcmp(argv[2], "index") == 0) {
            run_segway(segwayrmp::usb, std::string(argv[3]), 3);
        } else {
            print_usage();
            return 0;
        }
    } else {
        print_usage();
        return 0;
    }
}
