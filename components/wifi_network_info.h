#pragma once
#include <string>
#include <vector>

struct WifiNetworkInfo {
    std::wstring ssid;
    int signalQuality;
    std::wstring ipAddress;
};