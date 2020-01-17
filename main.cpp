#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

struct hci_state {
    int device_id;
    int device_handle;
    struct hci_filter original_filter;
    int state;
    int has_error;
    char error_message[1024];
} hci_state;

#define HCI_STATE_NONE      0
#define HCI_STATE_OPEN      2
#define HCI_STATE_SCANNING  3
#define HCI_STATE_FILTERING 4

struct hci_state current_hci_state;

struct hci_state open_default_hci_device()
{
    struct hci_state current_hci_state = {0};

    current_hci_state.device_id = hci_get_route(NULL);

    if((current_hci_state.device_handle == hci_open_dev(current_hci_state.device_id)) < 0)
    {
        current_hci_state.has_error = 1;

        // Print error message and store it in error_message char[] 
        snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to open device: %s\n",strerror(errno));
        return current_hci_state;
    }

    // Set fd non-blocking
    int on = 1;
    if(ioctl(current_hci_state.device_handle,FIONBIO,(char*)&on)<0)
    {
        current_hci_state.has_error = 1;
        snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to set device to non-blocking: %s\n",strerror(errno));
        return current_hci_state;
    }
    current_hci_state.state = HCI_STATE_OPEN;

}

void start_hci_scan(){
   if(hci_le_set_scan_parameters())

    // Time out 1000ms
    if(hci_le_set_scan_enable(current_hci_state.device_handle,0x01,1,1000)<0)
    {
        current_hci_state.has_error = 1;
        snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to scan: %s\n",strerror(errno));
        return;
    }
    current_hci_state.state = HCI_STATE_SCANNING;

}

int main(int argc, char** argv)
{
    current_hci_state = open_default_hci_device();



    
    

    return 0;
}