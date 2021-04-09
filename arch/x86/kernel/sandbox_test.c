#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sandbox.h>

int is_in_proc(int pid){
	struct list_head *itr;
	struct simple_proc_struct *tmp;
	list_for_each(itr, &simple_proc_struct_head){
		tmp = list_entry(itr, struct simple_proc_struct, proc_list);
		if (tmp->pid == pid){
                        printk(KERN_INFO "process is in sandbox");
			return 1;
                }
        }	
	return 0;
}

int is_in_syscall(long syscall_number){
	int i;
        for(i=0; i < us; i++){
                if(syscall_number == unpermitted_syscall[i]){
                        //printk(KERN_INFO "process is in sandbox");
                        return 1;
		}
        }
	return 0;
}

int regi_proc(int pid){
	struct simple_proc_struct *data;
	data = kmalloc(sizeof(struct simple_proc_struct), GFP_KERNEL);
	data->pid = pid;
	list_add(&data->proc_list, &simple_proc_struct_head);
	return 0;
}	

int clear_proc(int pid){
	struct list_head *itr;
	list_for_each(itr, &simple_proc_struct_head){

                if(pid ==list_entry(itr, struct simple_proc_struct, proc_list)->pid){
                        list_del(itr);
                        kfree(itr);
                        return 0;
                }
        }
        return 0;
}

SYSCALL_DEFINE1(register_sandbox, int, pid){
	struct simple_proc_struct *data;
	data = kmalloc(sizeof(struct simple_proc_struct), GFP_KERNEL);
	data->pid = pid;
	list_add(&data->proc_list, &simple_proc_struct_head);
	return 0;
}	

			
SYSCALL_DEFINE1(clear_sandlist, int, pid){
	struct list_head *itr;
	list_for_each(itr, &simple_proc_struct_head){
		
		if(pid == list_entry(itr, struct simple_proc_struct, proc_list)->pid){
			list_del(itr);
			kfree(itr);
			return 0;
		}
	}
	return 0;
}

/*
void del_PandC(int pid){
	struct list_head *itr;
	struct simple_proc_struct *data;
	list_for_each(itr, &simple_proc_struct_head){
		data = list_entry(itr, struct simple_proc_struct, list);
		if(pid == data->ppid){
			del_PandC(data->ppid);
		}
		if(pid == data->pid){
			data = list_entry(itr, struct simple_proc_struct, list);
			itr = itr->prev;
			list_del(&data->list);
			kfree(data);
		}
	}
}
			

SYSCALL_DEFINE1(clear_sandlist, int, pid){
	del_PandC(pid);
	return 0;
}
*/
