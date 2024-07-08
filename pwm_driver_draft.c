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
	struct pwm_clk_chip *pcchip;
	int ret;

	pcchip = devm_kzalloc(&pdev->dev, sizeof(*pcchip), GFP_KERNEL);
	if (!pcchip)
		return -ENOMEM;

	pcchip->clk = devm_clk_get_prepared(&pdev->dev, NULL);
	if (IS_ERR(pcchip->clk))
		return dev_err_probe(&pdev->dev, PTR_ERR(pcchip->clk),
				     "Failed to get clock\n");
	return 0;
}

static void pwm_clk_remove(struct platform_device *pdev)
{
	struct pwm_clk_chip *pcchip = platform_get_drvdata(pdev);

	pwmchip_remove(&pcchip->chip);

	if (pcchip->clk_enabled)
		clk_disable(pcchip->clk);
}

static struct platform_driver pwm_clk_driver = {
	.driver = {
		.name = "pwm-clk",
	},
	.probe = pwm_clk_probe,
	.remove_new = pwm_clk_remove,
};
module_platform_driver(pwm_clk_driver);

MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A Simple pwm test as a kernel module");
MODULE_LICENSE("GPL");
