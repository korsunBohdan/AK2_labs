// SPDX-License-Identifier: GPL-2.0 OR BSD-2-Clause
/*
 * Copyright (c) 2026, Student
 * All rights reserved.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

#include "hello1.h"

MODULE_AUTHOR("Bohdan Korsun <korsun.bohdan_io42@edu.kpi.ua>");
MODULE_DESCRIPTION("Module hello1 exporting print_hello and managing linked list");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_data {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

static LIST_HEAD(hello_list);

int print_hello(void)
{
	struct hello_data *data;

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->time_before = ktime_get();
	pr_info("Hello, world!\n");
	data->time_after = ktime_get();

	list_add_tail(&data->list, &hello_list);

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: module loaded successfully\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_data *md, *tmp;

	list_for_each_entry_safe(md, tmp, &hello_list, list) {
		ktime_t duration = ktime_sub(md->time_after, md->time_before);

		pr_info("hello1: print duration = %lld ns\n",
			ktime_to_ns(duration));
		list_del(&md->list);
		kfree(md);
	}
	pr_info("hello1: module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

