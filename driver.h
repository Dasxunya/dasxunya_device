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

struct proc_vma_area {
    unsigned long vm_start;		/* Our start address within vm_mm. */
    unsigned long vm_end;		/* The first byte after our end address within vm_mm. */
    unsigned long vm_flags;     /* Flags, see mm.h. */
    unsigned long vm_pgoff;		/* Offset (within vm_file) in PAGE_SIZE units */
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