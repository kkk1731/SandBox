#include <linux/unistd.h>
#include <linux/entry-common.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/compat.h>
#include <asm/syscall.h>

SYSCALL_DEFINE1(ptr5000_test, int *, a){
	int tmp = 0;
	int send = 5000;

	if(copy_from_user(&tmp, a, sizeof(int)) != 0){
		return -EFAULT;
	}
	if(copy_to_user(a, &send, sizeof(int)) != 0){
		return -EFAULT;
	}
	return tmp;
}

