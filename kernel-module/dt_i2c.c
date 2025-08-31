// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>

static struct i2c_client *oled_client;

/* Declare the probe and remove function */

static int my_oled_probe(struct i2c_client *client, const struct i2c_device_id *id);
static void my_oled_remove(struct i2c_client *client);

static struct of_device_id my_driver_ids[] =
{
	{
		.compatible = "brightlight,myoled",
	}, { /* sentinal */}
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct i2c_device_id my_oled[] = {
	{"my_oled", 0},
	{ },
};

MODULE_DEVICE_TABLE(i2c, my_oled);

static struct i2c_driver my_driver = 
{
	.probe = my_oled_probe,
	.remove = my_oled_remove,
	.id_table = my_oled,
	.driver = {
		.name = "my_oled",
		.of_match_table = my_driver_ids,
	},
};


static struct proc_dir_entry *proc_file;

/**
 * @brief send message to oled
 */
static ssize_t my_write(struct file *file, const char *user_buffer, size_t count, loff_t *offs)
{
	pr_info("dt_i2c - In write function!\n");
	pr_info("dt_i2c - Input %d to procfs\n", user_buffer[0]);
	int val;
	const int *result = (void*)kstrtouint(user_buffer, 16, &val);
	if(IS_ERR(result))
	{
		pr_err("dt_i2c - Error : %d converting string!\n", kstrtouint(user_buffer, 0, &val));
	}
	
	pr_info("dt_i2c - Writing: %x to i2c device to i2c device.\n",(u8) val);
	i2c_smbus_write_byte_data(oled_client, 0, (u8) val);
	/*
	long val;
	if(kstrtol(user_buffer, 0, &val) == 0)
	{
		pr_info("dt_i2c - Writing: %x to i2c device to i2c device.\n",(u8) val);
		i2c_smbus_write_byte_data(oled_client, 0, (u8) val);
		i2c_smbus_write_byte(oled_client, (u8) val);
	} else
	{
		pr_err("dt_i2c - Error : %d converting string!\n", kstrtol(user_buffer, 0, &val));
	}
	*/
	return count;
}

/**
 * @brief Read Oled Value
 */
static ssize_t my_read(struct file *file, char *user_buffer, size_t count, loff_t *offs)
{
	u8 oled;
	oled = i2c_smbus_read_byte(oled_client);
	return sprintf(user_buffer, "%d\n", oled);
}


static struct proc_ops fops = {
	.proc_write = my_write,
	.proc_read = my_read,
};

/**
 * @brief This function is called on loading the driver
 */
static int my_oled_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	pr_info("dt_i2c - Now I'm in the Probe Function!\n");

	if(client->addr != 0x3c)
	{
		pr_err("dt_i2c - Wrong i2c address!\n");
		return -1;
	}

	oled_client = client;

	/* Creating procfs file */
	proc_file = proc_create("my-oled", 0666, NULL, &fops);
	if(proc_file == NULL)
	{
		pr_err("dt_i2c - Error creating /proc/my-oled\n");
		return -ENOMEM;
	}
	pr_info("dt_i2c - Created /proc/my-oled\n");
	return 0;
}

/**
 * @brief This function is called on unloading the driver
 */
static void my_oled_remove(struct i2c_client *client)
{
	pr_info("dt_i2c - Now I'm in the remove function!\n");
	proc_remove(proc_file);
	return;
}

/* This will create init and exit function automatically */
module_i2c_driver (my_driver);

/* Meta Information */
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("David D'Ulisse");
MODULE_DESCRIPTION("Read a device tree");
