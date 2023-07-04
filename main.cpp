// main.cpp
#include <iostream>
#include <fstream>
#include <json/json.h> // You need to have jsoncpp library installed
#include "wifi_network_info.h"
#include "wifi_network_ip.h"

void displayWifiNetworks()
{
    DWORD dwClientVersion = 0;
    HANDLE hClient = NULL;
    DWORD dwMaxClient = 2;
    DWORD dwCurVersion = 0;
    DWORD dwResult = 0;
    DWORD dwRetVal = 0;
    WLAN_INTERFACE_INFO_LIST *pIfList = NULL;
    WLAN_INTERFACE_INFO *pIfInfo = NULL;
    WCHAR GuidString[39] = {0};

    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS)
    {
        std::cout << "WlanOpenHandle failed with error: " << dwResult << std::endl;
        return;
    }

    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS)
    {
        std::cout << "WlanEnumInterfaces failed with error: " << dwResult << std::endl;
        WlanCloseHandle(hClient, NULL);
        return;
    }

    std::vector<WifiNetworkInfo> networkInfos;
    if (GetWifiNetworkIPs(networkInfos))
    {
        for (size_t i = 0; i < networkInfos.size(); ++i)
        {
            std::wcout << "SSID: " << networkInfos[i].ssid << std::endl;
            std::wcout << "Signal Quality: " << networkInfos[i].signalQuality << std::endl;
            std::wcout << "IP Address: " << networkInfos[i].ipAddress << std::endl;
            std::cout << std::endl;
        }

        std::ofstream jsonFile("/components/list.json");
        if (jsonFile.is_open())
        {
            Json::StreamWriterBuilder builder;
            std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            writer->write(networksJson, &jsonFile);
            jsonFile.close();
            std::cout << "WiFi networks saved to /components/list.json" << std::endl;
        }
        else
        {
            std::cout << "Error getting IP addresses for WiFi networks" << std::endl;
        }
    }

    int main()
    {
        displayWifiNetworks();
        return 0;
    }