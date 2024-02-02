#include <iostream>
#include <string>
#include <algorithm>

#include "segwayrmp/segwayrmp.h"

void handleSegwayStatus(segwayrmp::SegwayStatus::Ptr ss) {
  std::cout << ss->str() << std::endl << std::endl;
}

int run_segway(segwayrmp::InterfaceType interface_type, std::string configuration, int config_type = 0) {
  std::cout << "!!!!!!!!!!!!WARNING!!!!!!!!!!!!" << std::endl;
  std::cout << "This example moves the segway!!" << std::endl;
  std::cout << "(If you continue, ctrl-c quits)" << std::endl;
  std::cout << "Do you want to continue? [No/yes]  ";
  std::string response;
  std::getline(std::cin, response);
  std::transform(response.begin(), response.end(),
  response.begin(), ::tolower);
  if (response != std::string("yes") and response != std::string("y")) {
    std::cout << "Aborting." << std::endl;
    return 1;
  }
  try {
    // segwayrmp::SegwayRMP rmp(interface_type, segwayrmp::rmp100);
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
          std::cout << "Invalid interface type provided."
                    << std::endl;
          return 0;
      }
    }
    rmp.setStatusCallback(handleSegwayStatus);
    rmp.connect();
    rmp.setOperationalMode(segwayrmp::tractor);
    while(true) {
      float given_x, given_y;
      given_x = 0.1;
      given_y = 0.1;
      //std::cout << "Type a number: ";
      //std::cin >> given_x;
      //std::cout << "Type another number: ";
      //std::cin >> given_y;
      rmp.move(given_x, given_y);

      std::cout << "---------------------------------------- \n";

      std::cout << "connected_: \n";
      std::cout << rmp.connected_ << "\n";

      std::cout << "ui_battery_voltage: \n";
      std::cout << rmp.segway_status_->ui_battery_voltage << "\n";

      std::cout << "commanded_velocity: \n";
      std::cout << rmp.segway_status_->commanded_velocity << "\n";

      std::cout << "commanded_yaw_rate: \n";
      std::cout << rmp.segway_status_->commanded_yaw_rate << "\n";

      std::cout << "integrated_left_wheel_position: \n";
      std::cout << rmp.segway_status_->integrated_left_wheel_position << "\n";
      usleep(100000);
      //Packet packet;
      //try {
      //  rmp.rmp_io_->getPacket(packet);
      //  rmp.ProcessPacket_(packet);
      //} catch (PacketRetrievalException &e) {
      //  if (e.error_number() == 2) // Failed Checksum
      //    this->error_("Checksum mismatch...");
      //  else if (e.error_number() == 3) // No packet received
      //    this->error_("No data from Segway...");
      //  else
      //    this->handle_exception_(e);
      //}
    }
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}

void print_usage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "       segwayrmp_interactive_example usb <serial_number | description | "
               "index> <\"00000056\" | \"Robotic Mobile Platform\" | 0>"
            << std::endl;
  std::cout << "       or" << std::endl;
  std::cout << "       segwayrmp_interactive_example serial <serial port>" << std::endl;
  std::cout << "Examples:" << std::endl;
  std::cout << "       segwayrmp_interactive_example usb index 0" << std::endl;
  std::cout << "       segwayrmp_interactive_example usb serial_number \"00000056\""
            << std::endl;
  std::cout << "       segwayrmp_interactive_example serial /dev/ttyUSB0" << std::endl;
  std::cout << "       segwayrmp_interactive_example serial COM0" << std::endl;
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
