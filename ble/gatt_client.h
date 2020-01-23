#ifndef GATT_CLIENT_H
#define GATT_CLIENT_H

#include "gattlib.h"
#include <stdio.h>
#include <string.h>


class GATTClient{
private:
    gatt_connection_t* gatt_connection = NULL;
    gattlib_primary_service_t* services = NULL;
    gattlib_characteristic_t* characteristics = NULL;
    char target[20];
   uuid_t light_uuid;
public:
    bool connect()
    {
	    strcpy(target,"EB:32:5F:E4:7E:5C");
	    printf("connecting to %s...\n",target);

        gatt_connection = gattlib_connect(NULL,target,GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
        if(gatt_connection == NULL)
        {
            printf("Failed to connect to blue device %s\n",target);
            printf("------DONE--------\n");
            return false;
        }

        printf("Connected to %s\n",target);

	    return true;
    }

    bool discover_services(){

        int ret = 0, i, services_count;
        if((ret = gattlib_discover_primary(gatt_connection,&services,&services_count))!= 0)
        {
            printf("Failed to discover primary services. Error code: %d\n",ret);
            disconnect();
            return false;
        }


	    char uuid_str[MAX_LEN_UUID_STR + 1];
        for (i = 0; i < services_count; i++) {
		    gattlib_uuid_to_string(&services[i].uuid, uuid_str, sizeof(uuid_str));

		    printf("service[%d] start_handle:%02x end_handle:%02x uuid:%s\n", i,
				services[i].attr_handle_start, services[i].attr_handle_end,
				uuid_str);

            if( strcmp(uuid_str,"0x1801") == 0 )
            {
                printf("light service %s found.\n",uuid_str);
            }    
	    }

        return true;
    }

    bool discover_characteristics(){
        int ret = 0, i, characteristics_count;
        char uuid_str[MAX_LEN_UUID_STR + 1];
    	ret = gattlib_discover_char(gatt_connection, &characteristics, &characteristics_count);
	    if (ret != 0) 
        {
		    printf("Fail to discover characteristics. Error code %d\n",ret);	
    	}
        
    	for (i = 0; i < characteristics_count; i++) {
		    gattlib_uuid_to_string(&characteristics[i].uuid, uuid_str, sizeof(uuid_str));

		    printf("characteristic[%d] properties:%02x value_handle:%04x uuid:%s\n", i,
				characteristics[i].properties, characteristics[i].value_handle,
				uuid_str);
            if(strcmp(uuid_str,"0xa001") == 0)
            {
                printf("light characteristic %s found\n",uuid_str);
	    }
        }
        return true;
    }



    bool read_char_value()
    {
        int ret;
	    size_t len;
        uint8_t* buffer;
        char uuid_str[MAX_LEN_UUID_STR + 1];
	    static uuid_t g_uuid;

        strcpy( uuid_str,"0xa001");

        if (gattlib_string_to_uuid( uuid_str, sizeof(uuid_str), &g_uuid) < 0) {
            printf("can not use char uuid\n");
		    return false;
	    }

	    ret  = gattlib_read_char_by_uuid(gatt_connection,&g_uuid,(void**)&buffer,&len);
        if(ret !=  GATTLIB_SUCCESS)
        {
            printf("Failed to read light characteristic, error code %d\n",ret);
            return false;
        }
	
	    printf("read light characterisic completed\n");

        for(int i= 0; i < len; i++)
        {
            printf("%02x",buffer[i]);
        }
        printf("\n");
        return true;
    }

    void disconnect()
    {
        if(gatt_connection != NULL)
        {
		printf("disconnected");
            gattlib_disconnect(gatt_connection);
        }
    }

    
};

#endif
