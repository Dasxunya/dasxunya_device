#include <linux/kernel.h> // To use the different Kernel Constants viz. KERN_DEBUG, KERN_INFO, KERN_ALERT ...
#include <linux/module.h> // Required for creating a Loadable Kernel Module
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h> //for task struct
#include <linux/pci.h>
#include <linux/sched/mm.h>
#include <linux/pid.h>
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
static struct proc_pci_dev gotten_pci_dev;
static struct proc_vma_area gotten_vma_area;

static int get_pci_dev (unsigned int vendor_id, unsigned int device_id){
    struct pci_dev *dev = pci_get_device(vendor_id, device_id, NULL);
    if (dev == NULL) {
        printk(KERN_INFO "Такого девайса не существует");
        return -1;
    }
    gotten_pci_dev.devfn = dev->devfn;
    gotten_pci_dev.vendor = dev->vendor;
    gotten_pci_dev.device = dev->device;
    gotten_pci_dev.subsystem_vendor = dev->subsystem_vendor;
    gotten_pci_dev.subsystem_device = dev->subsystem_device;
    gotten_pci_dev.class = dev->class;
    return 0;
}

static int get_vma_area (pid_t pid){
    struct task_struct *task = get_pid_task(find_get_pid(pid), PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_INFO "Не удалось получить task");
        return -1;
    }
    struct mm_struct *mm = get_task_mm(task);
    struct vm_area_struct *vma = mm->mmap;

    gotten_vma_area.vm_start = vma->vm_start;
    gotten_vma_area.vm_end = vma->vm_end;
    gotten_vma_area.vm_flags = vma->vm_flags;
    gotten_vma_area.vm_pgoff = vma->vm_pgoff;

    return 0;
}

static ssize_t node_read(struct file *file, char __user *buffer, size_t length, loff_t *ptr_offset){
    int vendorId, deviceId;
    unsigned int pid;
    if (BUFF < length) {
        printk(KERN_WARNING "Запись не помещается в буфер");
        sprintf(kbuf, GENERAL_ERROR_MESSAGE);
        copy_to_user(buffer, kbuf, length);
        return -EFAULT;
    }
    if (*ptr_offset > 0){
        return -EFAULT;
    }

    copy_from_user(kbuf, buffer, length);

    if ((sscanf(kbuf, "%x %x", &vendorId, &deviceId)) == 2){
        printk(KERN_INFO "Получаю айди (\"vid did\") от юзера");
        if ((get_pci_dev(vendorId, deviceId)) == 0){
            sprintf(kbuf, "\nEncoded device & function index: %u\nVendor: %u\nDevice: %u\nSubsystem vendor: %u\nSubsystem device: %u\nClass: %u\n", gotten_pci_dev.devfn, gotten_pci_dev.vendor, gotten_pci_dev.device, gotten_pci_dev.subsystem_vendor, gotten_pci_dev.subsystem_device, gotten_pci_dev.class);
            copy_to_user(buffer, kbuf, sizeof(kbuf));
        }   else {printk("Запись структуры в буфер пользователя не произошла");}
    } else if ((sscanf(kbuf, "%u", &pid)) == 1) {
        if ((get_vma_area(pid)) == 0){
            sprintf(kbuf, "\nStart address: %lu\nEnd address: %lu\nFlags: %lu\nOffset (within vm_file): %lu\n", gotten_vma_area.vm_start, gotten_vma_area.vm_end, gotten_vma_area.vm_flags, gotten_vma_area.vm_pgoff);
            copy_to_user(buffer, kbuf, sizeof(kbuf));
}   else {printk("Запись структуры в буфер пользователя не произошла");}
    } else {
        printk("Не удалось получить данные для определения структуры");
    }
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