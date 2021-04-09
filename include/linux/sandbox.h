#include <linux/list.h>

struct simple_proc_struct{
	pid_t pid;
	struct list_head proc_list;
};

static LIST_HEAD(simple_proc_struct_head);

static long unpermitted_syscall[] = {335, 338};
static int us = 2;
int is_in_proc(int pid);
int is_in_syscall(long syscall_number);

int regi_proc(int pid);
int clear_proc(int pid);
