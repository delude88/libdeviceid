//
// Created by Tobias Hegemann on 28.10.21.
//

#ifndef CLIENT_INCLUDE_LIBDEVICEID_SRC_DEVICE_ID_H_
#define CLIENT_INCLUDE_LIBDEVICEID_SRC_DEVICE_ID_H_

#include <cstdio>
#include <functional>
#include <string>

#ifdef _WIN32
#include "deviceid/machine/machine_id_win.h"
#else
#include "deviceid/machine/machine_id_unix.h"
#endif

namespace deviceid {
std::size_t get() {
  std::size_t h1 = std::hash<std::string>{}(getMachineName());
  std::size_t h2 = getCpuHash();
  std::size_t h3 = getVolumeHash();
  return (h1 ^ (h2 << 1)) ^ (h3 << 1);
}
};

#endif //CLIENT_INCLUDE_LIBDEVICEID_SRC_DEVICE_ID_H_
