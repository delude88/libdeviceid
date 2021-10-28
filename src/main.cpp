
//
// Created by Tobias Hegemann on 28.10.21.
//

#include <deviceid/id.h>
#include <iostream>

int main(int, char *[]) {
  auto device_id = deviceid::get();
  std::cout << "DeviceID: " << device_id << std::endl;
  return 0;
}