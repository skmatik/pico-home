#include "EthernetController.h"
#include <iostream>

EthernetController::EthernetController()
{
    ipAssigned = false;
}

EthernetController::~EthernetController()
{
}

bool EthernetController::initialize()
{
    cout << "Initializing Ethernet controller" << endl;
    initializeClock();
    wizchip_spi_initialize();
    wizchip_cris_initialize();
    wizchip_reset();

    wizchip_initialize();
    wizchip_check();
    cout << "Ethernet controller initialized" << endl;

    wizchip_1ms_timer_initialize(timerCallback);

    return dhcp();
}

bool EthernetController::dhcp()
{
    uint8_t result = 0;
    uint8_t retryCounter = 0;

    if (net_info.dhcp == NETINFO_DHCP) // DHCP
    {
        cout << "DHCP client running" << endl;
        DHCP_init(SOCKET_DHCP, buffer);
        reg_dhcp_cbfunc(EthernetController::dhcpAssign, EthernetController::dhcpAssign, EthernetController::dhcpConflict);
    }
    else // static
    {
        network_initialize(net_info);
        /* Get network information */
        //print_network_information(net_info);
        ipAssigned = true;
    }

    DNS_init(SOCKET_DNS, buffer);

    while ((!ipAssigned) && (retryCounter <= DHCP_RETRY_COUNT))
    {
        /* Assigned IP through DHCP */
        if (net_info.dhcp == NETINFO_DHCP)
        {
            result = DHCP_run();

            if (result == DHCP_IP_LEASED)
            {
                if (!ipAssigned)
                {
                    cout << "DHCP success" << endl;
                    ipAssigned = true;
                }
            }
            else if (result == DHCP_FAILED)
            {
                ipAssigned = false;
                retryCounter++;

                if (retryCounter <= DHCP_RETRY_COUNT)
                {
                    cout << "DHCP timeout occurred, retry attempt no: " << retryCounter << endl;
                }
            }

            if (retryCounter > DHCP_RETRY_COUNT)
            {
                cout << "DHCP failed" << endl;
                DHCP_stop();
            }

            wizchip_delay_ms(1000); // wait for 1 second
        }
    }
    return ipAssigned;
}

std::unique_ptr<uint8_t[]> EthernetController::resolveDomain(uint8_t *domain)
{

    uint8_t result = 0;
    uint8_t retryCounter = 0;
    bool ipResolved = false;
    std::unique_ptr<uint8_t[]> targetIp = std::make_unique<uint8_t[]>(4);

    DNS_init(SOCKET_DNS, buffer);

    /* Get IP through DNS */
    while (!ipResolved && (retryCounter <= DNS_RETRY_COUNT))
    {
        if (DNS_run(net_info.dns, domain, targetIp.get()) > 0)
        {
            cout << "DNS success" << endl;
            cout << "Target domain : " << domain << endl;
            cout << "IP of target domain : " << static_cast<int>(targetIp[0]) << "." << static_cast<int>(targetIp[1]) << "." << static_cast<int>(targetIp[2]) << "." << static_cast<int>(targetIp[3]) << endl;

            ipResolved = true;

            return targetIp;
        }
        else
        {
            retryCounter++;

            if (retryCounter <= DNS_RETRY_COUNT)
            {
                cout << "DNS lookup timeout occurred and retry " << retryCounter << endl;
            }
        }

        if (retryCounter > DNS_RETRY_COUNT)
        {
            cout << "DNS lookup failed" << endl;
        }

        wizchip_delay_ms(1000); // wait for 1 second
    }
    return nullptr;
}

std::string EthernetController::getIPAddress()
{
    return std::to_string(net_info.ip[0]) + "." + std::to_string(net_info.ip[1]) + "." + std::to_string(net_info.ip[2]) + "." + std::to_string(net_info.ip[3]);
}


void EthernetController::initializeClock()
{
    // set a system clock frequency in khz
    set_sys_clock_khz(PLL_SYS_KHZ, true);

    // configure the specified clock
    clock_configure(
        clk_peri,
        0,                                                // No glitchless mux
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, // System PLL on AUX mux
        PLL_SYS_KHZ * 1000,                               // Input frequency
        PLL_SYS_KHZ * 1000                                // Output (must be same as no divider)
    );
}

void EthernetController::timerCallback(void)
{
    EthernetController::milisecondCounter++;
    if (EthernetController::milisecondCounter >= 1000 - 1)
    {
        EthernetController::milisecondCounter = 0;
        EthernetController::secondCounter++;

        DHCP_time_handler();
        DNS_time_handler();
    }

    MilliTimer_Handler();
}

void EthernetController::dhcpAssign()
{
    getIPfromDHCP(net_info.ip);
    getGWfromDHCP(net_info.gw);
    getSNfromDHCP(net_info.sn);
    getDNSfromDHCP(net_info.dns);

    net_info.dhcp = NETINFO_DHCP;

    /* Network initialize */
    network_initialize(net_info); // apply from DHCP
    //print_network_information(net_info);

    cout << "DHCP leased time : " << getDHCPLeasetime() << " seconds" << endl;
}

void EthernetController::dhcpConflict()
{
    cout << "Conflict IP from DHCP" << endl;

    // halt or reset or any...
    while (1)
        ; // this example is halt.
}

volatile uint32_t EthernetController::milisecondCounter = 0;
volatile uint32_t EthernetController::secondCounter = 0;

wiz_NetInfo EthernetController::net_info =
    {
        .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x56},
        .ip = {192, 168, 1, 171},
        .sn = {255, 255, 255, 0},
        .gw = {192, 168, 1, 1},
        .dns = {8, 8, 8, 8},
        .dhcp = NETINFO_DHCP};

uint8_t EthernetController::buffer[ETHERNET_BUFFER_SIZE] = {
    0,
};