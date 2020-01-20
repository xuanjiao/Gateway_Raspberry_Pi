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

void start_hci_scan(struct hci_state current_hci_state){
    if(hci_le_set_scan_parameters(
            current_hci_state.device_handle,
            0x01,
            htobs(0x0010),
            htobs(0x0010),
            0x00,
            0x00,
            1000)<0)
            {
                current_hci_state.has_error = 1;
                snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to set scan parameters: %s\n",strerror(errno));
                return;
            }

    // Time out 1000ms
    if(hci_le_set_scan_enable(current_hci_state.device_handle,0x01,1,1000)<0)
    {
        current_hci_state.has_error = 1;
        snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to scan: %s\n",strerror(errno));
        return;
    }
    current_hci_state.state = HCI_STATE_SCANNING;

    // Save the current HCI filter
    socklen_t olen = sizeof(current_hci_state.original_filter);

    // Get socket option and store in original_filter
    if(getsockopt(current_hci_state.device_handle,SOL_HCI,HCI_FILTER, &current_hci_state.original_filter, &olen)<0)
    {
        current_hci_state.has_error = 1;
        snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to get socket options: %s\n",strerror(errno));
        return;
    }

    // Creat and set new filter
    struct hci_filter new_filter;

    hci_filter_clear(&new_filter);
    hci_filter_set_ptype(HCI_EVENT_PKT,&new_filter);
    hci_filter_set_event(EVT_LE_META_EVENT,&new_filter);

    if(setsockopt(current_hci_state.device_handle,SOL_HCI,HCI_FILTER,&new_filter,sizeof(new_filter)) < 0)
    {
        current_hci_state.has_error = 1;
        snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to set socket option: %s\n",strerror(errno));
        return;
    }
       
    current_hci_state.state = HCI_STATE_FILTERING;
}

void stop_hci_scan(struct hci_state current_hci_state)
{
    if(current_hci_state.state == HCI_STATE_FILTERING)
    {
        current_hci_state.state = HCI_STATE_SCANNING;
        setsockopt(current_hci_state.device_handle,SOL_HCI,HCI_FILTER,&current_hci_state.original_filter,sizeof(current_hci_state.original_filter));
    }

    if(hci_le_set_scan_enable(current_hci_state.device_handle,0x00,1,1000)<0)
    {
        current_hci_state.has_error = 1;
        snprintf(current_hci_state.error_message,sizeof(current_hci_state.error_message),"Failed to disable scan: %s\n",strerror(errno));
    }

    current_hci_state.state = HCI_STATE_OPEN;
}

void close_hci_device(struct hci_state current_hci_state){
    if(current_hci_state.state == HCI_STATE_OPEN){
        hci_close_dev(current_hci_state.device_handle);
    }
}

void error_check_and_exit(struct hci_state current_hci_state)
{
    if(current_hci_state.has_error)
    {
        printf("ERROR: %s\n",current_hci_state.error_message);
        exit(1);
    }
}

int main(int argc, char** argv)
{
    current_hci_state = open_default_hci_device();
    
    error_check_and_exit(current_hci_state);

    stop_hci_scan(current_hci_state);

    start_hci_scan(current_hci_state);

    error_check_and_exit(current_hci_state);

    printf("Scanning\n");


    stop_hci_scan(current_hci_state);

    error_check_and_exit(current_hci_state);

    close_hci_device(current_hci_state);
    

    return 0;
}