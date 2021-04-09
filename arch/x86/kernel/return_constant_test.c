#include <linux/unistd.h>
#include <linux/entry-common.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/compat.h>
#include <asm/syscall.h>

SYSCALL_DEFINE0(return_constant_test){
	return 1;
}

