#include "ble_process.h"
#include "gatt_client.h"


int main()
{

    BLEProcess ble_process;

    if(ble_process.start_scan())
    {
        printf("Scan completed\n");
        if(ble_process.stop_scan())
	{
		printf("Scan stoped\n");

		GATTClient gatt_client;
    		if(gatt_client.connect())
    		{
        		gatt_client.discover_services();
        		gatt_client.discover_characteristics();
        
        		printf("disconnected");
    		}
	}	
    }


   
}
