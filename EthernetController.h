#ifndef ETHERNET_CONTROLLER_H
#define ETHERNET_CONTROLLER_H

#include "port_common.h"
#include "wizchip_conf.h"
#include "w5x00_spi.h"
#include "mqtt_interface.h"
#include "timer.h"
#include "dhcp.h"
#include "dns.h"
#include <string>
#include <memory>

using namespace std;

constexpr uint ETHERNET_BUFFER_SIZE = (1024 * 2);

class EthernetController
{
private:
    static constexpr uint PLL_SYS_KHZ = (133 * 1000);
    static constexpr uint SOCKET_DHCP = 0;
    static constexpr uint SOCKET_DNS = 1;
    static constexpr uint DHCP_RETRY_COUNT = 5;
    static constexpr uint DNS_RETRY_COUNT = 5;

    static volatile uint32_t milisecondCounter;
    static volatile uint32_t secondCounter;
    static uint8_t buffer[ETHERNET_BUFFER_SIZE];
    static wiz_NetInfo net_info;

    bool ipAssigned;
    static void timerCallback();
    static void dhcpAssign();
    static void dhcpConflict();

    void initializeClock();
    bool dhcp();

public:
    EthernetController();
    ~EthernetController();
    bool initialize();
    std::unique_ptr<uint8_t[]> resolveDomain(uint8_t *domain);
    std::string getIPAddress();
};

#endif