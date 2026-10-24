#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Bohdan Korsun <korsun.bohdan_io42@edu.kpi.ua>");
MODULE_DESCRIPTION("Hello world module with lists and time");
MODULE_LICENSE("Dual BSD/GPL");

/* Кількість разів виконання циклу  */
static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of times to print Hello, world!");

/* Структура для зв'язного списку */
struct hello_data {
	struct list_head list;
	ktime_t time;
};

/* Оголошення голови списку */
static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	uint i;
	struct hello_data *data;

	/* Перевірка параметра */
	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("Warning: count is %u\n", count);
	} else if (count > 10) {
		pr_err("Error: count %u is too large\n", count);
		return -EINVAL;
	}

	/* Додавання елементів та вивід повідомлення */
	for (i = 0; i < count; i++) {
		data = kmalloc(sizeof(*data), GFP_KERNEL);
		if (!data) {
			pr_err("Memory allocation failed\n");
			return -ENOMEM;
		}

		data->time = ktime_get();
		list_add_tail(&data->list, &hello_list);

		pr_info("Hello, world!\n");
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
