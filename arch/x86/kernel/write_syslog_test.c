#include <linux/unistd.h>
#include <linux/entry-common.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/compat.h>
#include <asm/syscall.h>


SYSCALL_DEFINE1(write_syslog_test, char *, msg){
	char buf[256];
	long copied = strncpy_from_user(buf, msg, sizeof(buf));
	if (copied < 0 || copied == sizeof(buf))
		return -EFAULT; 
		
	printk(KERN_INFO "write_syslog called with \"%s\"\n", buf);
	return 0;
}
