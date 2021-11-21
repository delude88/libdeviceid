#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#ifndef _MSC_VER
#include <cpuid.h>
#else
#include <intrin.h>
#endif

namespace deviceid {
// we just need this for purposes of unique machine id. So any one or two mac's is       
// fine. 
UINT16 hashMacAddress(PIP_ADAPTER_INFO info) {
  UINT16 hash = 0;
  for (UINT32 i = 0; i < info->AddressLength; i++) {
    hash += (info->Address[i] << ((i & 1) * 8));
  }
  return hash;
}

void getMacHash(UINT16 &mac1, UINT16 &mac2) {
  IP_ADAPTER_INFO AdapterInfo[32];
  ULONG dwBufLen = sizeof(AdapterInfo);

  ULONG dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
  if (dwStatus != ERROR_SUCCESS)
    return; // no adapters.

  PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
  mac1 = hashMacAddress(pAdapterInfo);
  if (pAdapterInfo->Next)
    mac2 = hashMacAddress(pAdapterInfo->Next);

  // sort the mac addresses. We don't want to invalidate
  // both macs if they just change order.
  if (mac1 > mac2) {
    UINT16 tmp = mac2;
    mac2 = mac1;
    mac1 = tmp;
  }
}

UINT16 getVolumeHash() {
  DWORD serialNum = 0;

  // Determine if this volume uses an NTFS file system.
  GetVolumeInformation("c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
  UINT16 hash = (UINT16)((serialNum + (serialNum >> 16)) & 0xFFFF);

  return hash;
}

UINT16 getCpuHash() {
  int cpuinfo[4] = {0, 0, 0, 0};
  __cpuid(cpuinfo, 0);
  UINT16 hash = 0;
  UINT16 *ptr = (UINT16 * )(&cpuinfo[0]);
  for (UINT32 i = 0; i < 8; i++)
    hash += ptr[i];

  return hash;
}

const char *getMachineName() {
  static char computerName[1024];
  DWORD size = 1024;
  GetComputerName(computerName, &size);
  return &(computerName[0]);
}
}
