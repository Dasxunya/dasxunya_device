#include <linux/kernel.h> // To use the different Kernel Constants viz. KERN_DEBUG, KERN_INFO, KERN_ALERT ...
#include <linux/module.h> // Required for creating a Loadable Kernel Module
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <linux/export.h>  // For file operation
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h> // User copy

#include "driver.h"

static char kbuf[BUFF];

// Add file or directory entries to the /proc file system
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static void get_pci_dev (unsigned int vendor_id, unsigned int device_id){
    struct proc_pci_dev gotten_pci_dev;
    struct pci_dev *dev = pci_get_device(vendor_id, device_id, NULL);
    if (dev == NULL) {
        printk(KERN_INFO "Такого девайса не существует");
        return -EFAULT;
    }
    gotten_pci_dev.devfn = dev->devfn;
    gotten_pci_dev.vendor = dev->vendor;
    gotten_pci_dev.device = dev->device;
    gotten_pci_dev.subsystem_vendor = dev->subsystem_vendor;
    gotten_pci_dev.subsystem_device = dev->subsystem_device;
    gotten_pci_dev.class = dev->class;
    //send to user this struct
}

static ssize_t node_read(struct file *file, char __user *buffer, size_t length, loff_t *ptr_offset){
    int vendorId, deviceId;
    if (BUFF < length) {
        printk(KERN_WARNING "Запись не помещается в буфер");
        sprintf(kbuf, GENERAL_ERROR_MESSAGE);
        copy_to_user(buffer, kbuf, length);
        return -EFAULT;
    }
    if (*ptr_offset > 0){
        return -EFAULT;
    }
    printk(KERN_INFO "Получаю айди (\"vid did\") от юзера");
    copy_from_user(kbuf, buffer, length);

    if ((sscanf(kbuf, "%d %d", &vendorId, &deviceId)) == 2){
        get_pci_dev(vendorId, deviceId);
    } else {printk("ойей");}
        /*else if () {

    } else {

    }*/

    *ptr_offset = strlen(kbuf);
    return *ptr_offset;
}

static const struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = node_read
};

static int __init dasxunya_device_init(void)
{
    printk(KERN_INFO "Инициализация модуля\n");
    proc_dir = proc_mkdir(PROCFS_DIR_NAME, NULL);
    if (proc_dir == NULL)
    {
        printk("Не удалось создать директорию");
        return -1;
    }
    proc_file = proc_create(PROCFS_FILE_NAME, 0777, proc_dir, &fops);
    if (proc_file == NULL)
    {
        proc_remove(proc_dir);
        printk("Не удалось создать файл");
        return -1;
    }
    printk(KERN_INFO "Модуль успешно внедрен\n");
    return 0;
}

static void __exit dasxunya_device_exit(void)
{
    proc_remove(proc_dir);
    printk(KERN_INFO "Модуль успешно извлечен\n");
}

// Registering module init and exit functions
module_init(dasxunya_device_init);
module_exit(dasxunya_device_exit);

// Module descriptions
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module featuring procfs: proc_pci_dev, ppp_channel");
MODULE_VERSION("1.0");