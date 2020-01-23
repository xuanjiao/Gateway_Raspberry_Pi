#include "ble_process.h"
#include "gatt_client.h"

#define ADC_ADDR "EB:32:5F:E4:7E:5C"

int main()
{
	/*
    BLEProcess ble_process;

    if(ble_process.start_scan())
    {
        printf("Scan completed\n");
        ble_process.stop_scan();
    }
*/

    GATTClient gatt_client;
    if(gatt_client.connect())
    {
        // gatt_client.discover_services();
        // gatt_client.discover_characteristics();
        gatt_client.read_char_value();
        gatt_client.disconnect();
        printf("disconnected");
    }
    
}
