// wifi_network_info.cpp
#include "wifi_network_info.h"

WifiNetworkInfo::WifiNetworkInfo(const std::wstring& ssid, int signalQuality)
    : ssid(ssid), signalQuality(signalQuality) {}
