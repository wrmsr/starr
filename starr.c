/*
 * @author  Derek Molloy
 * http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
*/

#include <linux/init.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("starr");
MODULE_VERSION("0.1");

static char *name = "starr";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

static long starr_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
  struct pt_regs *regs;
  printk("%s: The calling process is \"%s\" (pid %i)\n", __FUNCTION__, current->comm, current->pid);
  if (cmd == 0) {
    regs = task_pt_regs(current);
    printk("%s: cs=%lx\n", __FUNCTION__, regs->cs);
    regs->cs &= ~3;
    printk("%s: cs=%lx\n", __FUNCTION__, regs->cs);
    return 0;
  }
  return -EINVAL;
}

const struct file_operations starr_fops = {
  .owner = THIS_MODULE,
  .unlocked_ioctl = starr_ioctl,
};

struct miscdevice starr_dev = {
  MISC_DYNAMIC_MINOR,
  "starr",
  &starr_fops
};

static int __init starr_init(void){
  int rv;
  printk(KERN_INFO "starr: Hello %s from the starr LKM!\n", name);
  rv = misc_register(&starr_dev);
  if (rv)
    return rv;
  return 0;
}

static void __exit starr_exit(void){
  printk(KERN_INFO "starr: Goodbye %s from the starr LKM!\n", name);
  misc_deregister(&starr_dev);
}

module_init(starr_init);
module_exit(starr_exit);
