#ifndef DASXUNYA_DEVICE_DRIVER_H
#define DASXUNYA_DEVICE_DRIVER_H

// Info
#define MODULE_NAME "dasxunya_device"

// Paths
#define PROCFS_DIR_NAME "OS2lab"
#define PROCFS_FILE_NAME "out"

//
#define GENERAL_ERROR_MESSAGE "E: Check kernel module or see more in dmesg"
#define BUFF 4096

struct ppp_channel_struct {
    int mtu; /* max transmit packet size */
    int hdrlen; /* amount of headroom channel needs */
    int speed; /* transfer rate (bytes/second) */
};

struct proc_pci_dev {
    unsigned int	devfn;		/* Encoded device & function index */
    unsigned short	vendor;
    unsigned short	device;
    unsigned short	subsystem_vendor;
    unsigned short	subsystem_device;
    unsigned int	class;
};
#endif //DASXUNYA_DEVICE_DRIVER_H