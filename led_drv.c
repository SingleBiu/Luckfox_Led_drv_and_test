#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "led_dev"
#define LED_GPIO 32 // 对应 GPIOA0_D

static dev_t dev_num;
static struct cdev led_cdev;
static struct class *led_class;

static int led_open(struct inode *inode, struct file *file) { return 0; }

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char val;
    if (copy_from_user(&val, buf, 1)) return -EFAULT;
    gpio_set_value(LED_GPIO, val ? 1 : 0); // 根据写入值控制高低电平
    return count;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .write = led_write,
};

static int __init led_init(void) {
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&led_cdev, &led_fops);
    cdev_add(&led_cdev, dev_num, 1);
    led_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(led_class, NULL, dev_num, NULL, DEVICE_NAME);
    
    gpio_request(LED_GPIO, DEVICE_NAME);
    gpio_direction_output(LED_GPIO, 0); // 默认输出低电平（关灯）
    printk(KERN_INFO "LED driver loaded!\n");
    return 0;
}

static void __exit led_exit(void) {
    gpio_set_value(LED_GPIO, 0);
    gpio_free(LED_GPIO);
    device_destroy(led_class, dev_num);
    class_destroy(led_class);
    cdev_del(&led_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "LED driver unloaded!\n");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
