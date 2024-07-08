#include <linux/kernel.h>
#include <linux/math64.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/pwm.h>

struct pwm_clk_chip {
	struct pwm_chip chip;
	struct clk *clk;
	bool clk_enabled;
};

static int pwm_clk_probe(struct platform_device *pdev)
{

    pr_err("DEVICE TREE UNDERSTANDING: FETCH NODE\n");
	struct device_node *np;
    np = of_find_node_by_name(NULL, "pwm1");
    if (!np) {
        pr_err("Failed to find node\n");
        return -ENODEV;
    }

    u32 prop_value;
    int ret;

    ret = of_property_read_u32(np, "status", &prop_value);
    if (ret) {
        pr_err("Failed to read property\n");
        of_node_put(np);
        return ret;
    }



	return 0;
}

static void pwm_clk_remove(struct platform_device *pdev)
{

}

static struct platform_driver pwm_clk_driver = {
	.driver = {
		.name = "pwm-clk",
	},
	.probe = pwm_clk_probe,
	.remove_new = pwm_clk_remove,
};

module_platform_driver(pwm-clk);

MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A Simple pwm test as a kernel module");
MODULE_LICENSE("GPL");
