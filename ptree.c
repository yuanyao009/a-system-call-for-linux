#include<linux/syscalls.h>
#include<linux/list.h>
#include<linux/slab.h>
#include<asm-generic/errno-base.h>
#include<linux/sched.h>
#include<asm/uaccess.h>

SYSCALL_DEFINE2(ptree, struct prinfo, *buf, int, *nr)
{
	int result;
    
	int do_our_work(struct prinfo *b, int *n);
	struct prinfo *buf2 = kcalloc(*nr, sizeof(struct prinfo), GFP_KERNEL);
	int *nr2 = kcalloc(1, sizeof(int), GFP_KERNEL);
    
	if (buf == NULL || nr == NULL || *nr < 0)
		return -EINVAL;
	if (copy_from_user(buf2, buf, (*nr) * sizeof(struct prinfo)) != 0)
		return -EFAULT;
	if (copy_from_user(nr2, nr, sizeof(int)) != 0)
		return -EFAULT;
	read_lock(&tasklist_lock);
	result = do_our_work(buf2, nr2);
	read_unlock(&tasklist_lock);
	if (copy_to_user(buf, buf2, (*nr2) * sizeof(struct prinfo)) != 0)
		return -EFAULT;
	if (copy_to_user(nr, nr2, sizeof(int)) != 0)
		return -EFAULT;
	return result;
}

int do_our_work(struct prinfo *buf2, int *nr2)
{
	void do_dfs(struct prinfo *b, int *n, struct task_struct *task, int *n2);
	void store_process_to_buf(struct prinfo *b, int *n, struct task_struct *cr, int *c);
	int *count = kcalloc(1, sizeof(int), GFP_KERNEL);
	struct task_struct *first = &init_task;
	*count = 0;
	store_process_to_buf(buf2, nr2, first, count);
	do_dfs(buf2, nr2, first, count);
	return *count;
}

void do_dfs(struct prinfo *buf2, int *nr2, struct task_struct *cur, int *count)
{
	void store_process_to_buf(struct prinfo *b, int *n, struct task_struct *c, int *c1);
	int is_not_thread_leader(struct task_struct *t);
	struct list_head *child = NULL;
	list_for_each(child, &cur->children) {
		struct task_struct *tmp = list_entry(child, struct task_struct, sibling);
        
		if (!is_not_thread_leader(tmp))
			store_process_to_buf(buf2, nr2, tmp, count);
		if (*count >= *nr2)
			return;
		do_dfs(buf2, nr2, tmp, count);
	}
    
}

void store_process_to_buf(struct prinfo *buf2, int *nr2, struct task_struct *cur, int *count)
{
	int siblinglist_is_empty(struct task_struct *t);
	int is_not_thread_leader(struct task_struct *t);
	struct prinfo *tmp = kcalloc(1, sizeof(struct prinfo), GFP_KERNEL);
	char *str1;
	char *str2;
	int i;
    
	tmp->first_child_pid = 0;
	tmp->next_sibling_pid = 0;
	if (!list_empty(&cur->children) && cur != NULL) {
		struct task_struct *child = list_entry(cur->children.next, struct task_struct, sibling);
		if (!is_not_thread_leader(child))
			tmp->first_child_pid = child->pid;
	}
	if (!siblinglist_is_empty(cur)) {
		struct task_struct *sib = list_entry(cur->sibling.next, struct task_struct, sibling);
		if (!is_not_thread_leader(sib))
			tmp->next_sibling_pid = sib->pid;
	}
    
	tmp->parent_pid = cur->parent->pid;
	tmp->pid = cur->pid;
	tmp->state = cur->state;
	tmp->uid = task_uid(cur);
	str1 = (char *)&(tmp->comm);
	str2 = (char *)&(cur->comm);
	i = 0;
	for (i = 0; i < strlen(cur->comm); i++)
		str1[i] = str2[i];
	str1[i] = 0;
	buf2[*count] = *tmp;
	*count = *count + 1;
}

int siblinglist_is_empty(struct task_struct *task)
{
	struct list_head *list = &task->parent->children;
	struct list_head *head = &task->parent->children;
	struct task_struct *tmp = kcalloc(1, sizeof(struct prinfo), GFP_KERNEL);
	while (list->next != head)
		list = list->next;
	tmp = list_entry(list, struct task_struct, sibling);
	if (task == tmp)
		return 1;
	else
		return 0;
}

int is_not_thread_leader(struct task_struct *tmp)
{
	if (thread_group_empty(tmp))
		return 0;
	else if (thread_group_leader(tmp))
		return 0;
	else
		return 1;
}