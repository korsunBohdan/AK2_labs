// SPDX-License-Identifier: GPL-2.0 OR BSD-2-Clause
/*
 * Copyright (c) 2026, Student
 * All rights reserved.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/bug.h>

MODULE_AUTHOR("Bohdan Korsun <korsun.bohdan_io42@edu.kpi.ua>");
MODULE_DESCRIPTION("Hello world module with lists and time");
MODULE_LICENSE("Dual BSD/GPL");

/* Кількість разів виконання циклу */
static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of times to print Hello, world!");

/* Структура для зв'язного списку */
struct hello_data {
	ktime_t time;
	struct list_head list;
};

/* Оголошення голови списку */
static LIST_HEAD(hello_list);

/* Функція додавання вузла у список */
__attribute__((__noinline__))
static void add_to_list(struct list_head *node)
{
	BUG_ON(!node);
	list_add_tail(node, &hello_list);
}

static int __init hello_init(void)
{
	uint i;
	struct hello_data *data;

	/* Перевірка параметра */
	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("Warning: count is %u\n", count);

	/* Аварійна зупинка через BUG_ON */
	BUG_ON(count > 10);

	/* Додавання елементів та вивід повідомлення */
	for (i = 0; i < count; i++) {
		data = kmalloc(sizeof(*data), GFP_KERNEL);

		/* Симуляція відмови kmalloc() на 5-му повідомленні */
		if (i == 4) {
			pr_warn("Simulating kmalloc failure for 5th message\n");
			kfree(data);
			data = NULL;
		}

		if (data)
			data->time = ktime_get();

		pr_info("Hello, world!\n");

		/* Передача елемента у список */
		add_to_list(&data->list);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_data *md, *tmp;

	/* Прохід по списку з видаленням елементів */
	list_for_each_entry_safe(md, tmp, &hello_list, list) {
		pr_info("Time: %lld ns\n", ktime_to_ns(md->time));
		list_del(&md->list);
		kfree(md);
	}
}

module_init(hello_init);
module_exit(hello_exit);
