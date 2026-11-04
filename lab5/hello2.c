// SPDX-License-Identifier: GPL-2.0 OR BSD-2-Clause
/*
 * Copyright (c) 2026, Student
 * All rights reserved.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>

#include "hello1.h"

MODULE_AUTHOR("Bohdan Korsun <korsun.bohdan_io42@edu.kpi.ua>");
MODULE_DESCRIPTION("Module hello2 consuming print_hello");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of times to print Hello, world!");

static int __init hello2_init(void)
{
	uint i;
	int ret;

	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("hello2: Warning: count is %u\n", count);
	} else if (count > 10) {
		pr_err("hello2: Error: count %u is too large\n", count);
		return -EINVAL;
	}

	for (i = 0; i < count; i++) {
		ret = print_hello();
		if (ret) {
			pr_err("hello2: print_hello failed with code %d\n", ret);
			return ret;
		}
	}

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

