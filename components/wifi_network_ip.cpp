// wifi_network_ip.cpp
#include "wifi_network_ip.h"
#include "wifi_network_info.h"
#include <windows.h>
#include <iphlpapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

bool GetWifiNetworkIPs(std::vector<WifiNetworkInfo>& networkInfos)
{
    ULONG family = AF_INET; // You can use AF_INET6 for IPv6 addresses if needed

    DWORD dwSize = 0;
    if (GetAdaptersAddresses(family, 0, NULL, NULL, &dwSize) != ERROR_BUFFER_OVERFLOW) {
        return false;
    }

    IP_ADAPTER_ADDRESSES* pAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new BYTE[dwSize]);
    if (!pAddresses) {
        return false;
    }

    if (GetAdaptersAddresses(family, 0, NULL, pAddresses, &dwSize) != ERROR_SUCCESS) {
        delete[] pAddresses;
        return false;
    }

    IP_ADAPTER_ADDRESSES* pAdapter = pAddresses;
    while (pAdapter) {
        if (pAdapter->OperStatus == IfOperStatusUp && pAdapter->IfType == IF_TYPE_IEEE80211) {
            IP_ADAPTER_UNICAST_ADDRESS* pUnicast = pAdapter->FirstUnicastAddress;
            while (pUnicast) {
                sockaddr* saAddr = pUnicast->Address.lpSockaddr;
                if (saAddr->sa_family == family) {
                    wchar_t ipStr[INET_ADDRSTRLEN];
                    inet_ntop(family, &reinterpret_cast<sockaddr_in*>(saAddr)->sin_addr, ipStr, INET_ADDRSTRLEN);

                    std::wstring ssid = pAdapter->Description;
                    int signalQuality = 0; // You should get the signal quality from the previously obtained data.

                    networkInfos.push_back(WifiNetworkInfo(ssid, signalQuality));
                    break;
                }
                pUnicast = pUnicast->Next;
            }
        }
        pAdapter = pAdapter->Next;
    }

    delete[] pAddresses;
    return true;
}