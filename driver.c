#include <linux/kernel.h> // To use the different Kernel Constants viz. KERN_DEBUG, KERN_INFO, KERN_ALERT ...
#include <linux/module.h> // Required for creating a Loadable Kernel Module
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/export.h>  // For file operation
#include <linux/proc_fs.h>
#include <linux/string.h>

#include "driver.h"

// Add file or directory entries to the /proc file system
static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static ssize_t node_write(struct file *file, const char __user *buffer, size_t length, loff_t *ptr_offset){
    char umsg[BUFF];
    printk(KERN_INFO "Обработка аргументов\n");
    return strlen(umsg);
}

static const struct file_operations fops = {
        .owner = THIS_MODULE,
        .write  = node_write
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
MODULE_DESCRIPTION("Module featuring procfs: pci_dev, ppp_channel");
MODULE_VERSION("1.0");