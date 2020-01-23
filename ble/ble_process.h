#ifndef BLE_PROCESS_H
#define BLE_PROCESS_H

#include <stdio.h>
#include <vector>
#include "gattlib.h"

#define UUID 0xFFFF
class BLEProcess
{
    private:
    void* adapter = NULL;
    const char* adapter_name = NULL;
    static const int BLE_SCAN_TIMEOUT = 4; 
    public:
    bool start_scan()
    {
        int ret;

        // Open default ble adapter.
        if ((ret = gattlib_adapter_open(adapter_name,&adapter))<0)
        {
            printf("Failed to open adapter. Error code %d\n",ret);
            return false;
        }

        if((ret = gattlib_adapter_scan_enable(adapter,when_device_discovered,BLE_SCAN_TIMEOUT,NULL))<0)
        {
            printf("Failed to scan. Error code%d\n",ret);
        }

        return true;
    }

    void stop_scan()
    {
        gattlib_adapter_scan_disable(adapter);

        gattlib_adapter_close(adapter);
    }

   /** callback called for each new Bluetooth device discovered.
    * adapter: adapter that has found for ble device
    * addr: MAC address of the BLE device
    * name: name of the BLE device
    * user_data: Data defined
    */
    static void when_device_discovered(void* adapter, const char* addr, const char* name, void *user_data)
    {
        if(name)
        {
            printf("Discovered %s - %s\n",addr,name);
        }else
        {
            printf("Discovered %s - [unknown]\n",addr);
        }

    }
    
};


#endif