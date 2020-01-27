#ifndef BLE_PROCESS_H
#define BLE_PROCESS_H

#include <stdio.h>
#include <vector>
#include "gattlib.h"
#include "ble_config.h"

#define ADV_UUID "0xFFFF"
class BLEProcess
{
    private:
    void* adapter = NULL;
    const char* adapter_name = NULL;
    public:
    bool start_scan()
    {
        int ret;

        // Open default ble adapter.
        if ((ret = gattlib_adapter_open(adapter_name,&adapter)) != GATTLIB_SUCCESS)
        {
            printf("Failed to open adapter. Error code %d\n",ret);
            return false;
        }

        if((ret = gattlib_adapter_scan_enable(adapter,when_device_discovered,BLE_SCAN_TIMEOUT,NULL))!= GATTLIB_SUCCESS)
        {
            printf("Failed to scan. Error code%d\n",ret);
	    return false;
        }

        return true;
    }

    bool start_scan_with_filter(){
        int ret;
        uuid_t g_uuid;
	char beacon_uuid[] = "0xFFFF"; 
	char uuid_str[MAX_LEN_UUID_STR + 1];

        if( (ret = gattlib_string_to_uuid(beacon_uuid,strlen(beacon_uuid)+1,&g_uuid))!= GATTLIB_SUCCESS)
        {
            printf("Convert string to uuid failed");
            return false;
        }
	
	if( (ret = gattlib_uuid_to_string(&g_uuid,uuid_str,sizeof(uuid_str)))!= GATTLIB_SUCCESS)
	{
	printf("failed to convert uuid %s to string. Error code %d\n",uuid_str,ret);
	return false;

	}
	
        uuid_t uuid_list[] = {g_uuid};
        uuid_t* p_uuid_list = uuid_list;
        // Open default ble adapter.
        if ((ret = gattlib_adapter_open(adapter_name,&adapter))!= GATTLIB_SUCCESS)
        {
            printf("Failed to open adapter. Error code %d\n",ret);
            return false;
        }

        if((ret = gattlib_adapter_scan_enable_with_filter(
            adapter,
            &p_uuid_list,
            0,
	    GATTLIB_DISCOVER_FILTER_USE_UUID,
            when_device_discovered,
            BLE_SCAN_TIMEOUT,
            NULL)) != GATTLIB_SUCCESS){
                    printf("Failed to scan with filter. Error code: %d\n",ret);
                    return false;
            }
	printf("scan with filter result:%d\n",ret);
        return true;
    }

    int stop_scan()
    {

	int ret;
        if((ret = gattlib_adapter_scan_disable(adapter))!= GATTLIB_SUCCESS)
	{
		printf("Failed to disable scan. Error code: %d\n",ret);
		return false;
	}

	
	if((ret = gattlib_adapter_close(adapter))!= GATTLIB_SUCCESS)
	{
		printf("Failed to close adapter. Error code:%d\n",ret);
		return false;
        }
	return true;
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
