/*
 * Driver core interface to the pinctrl subsystem.
 *
 * Copyright (C) 2012 ST-Ericsson SA
 * Written on behalf of Linaro for ST-Ericsson
 * Based on bits of regulator core, gpio core and clk core
 *
 * Author: Linus Walleij <linus.walleij@linaro.org>
 *
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/device.h>
#include <linux/pinctrl/devinfo.h>
#include <linux/pinctrl/consumer.h>
#include <linux/slab.h>

//Charm start
#include <linux/of.h>
#define CONFIG_PM 1
extern struct pinctrl *devm_pinctrl_get__remote(struct device *dev);
extern struct pinctrl_state *pinctrl_lookup_state__remote(struct pinctrl *p,const char *name);
extern int pinctrl_select_state__remote(struct pinctrl *p, struct pinctrl_state *state);
extern bool IS_ERR__remote(const void *ptr);
extern bool PTR_ERR__remote(const void *ptr);
extern void devm_pinctrl_put__remote(struct pinctrl *p);
//Charm end
/**
 * pinctrl_bind_pins() - called by the device core before probe
 * @dev: the device that is just about to probe
 */
int pinctrl_bind_pins(struct device *dev)
{
	int ret;
//Charm start

        char * compatible;
        int len_compatible;
	int rpc_do_flag=0;
	compatible=of_get_property(dev->of_node,"compatible",&len_compatible);
	if(compatible){	
		if(
		strcmp(compatible,"qcom,camera-flash")&&
		strcmp(compatible,"st,stmvl6180"     )&&
		strcmp(compatible,"qcom,gdsc"        )&&
		strcmp(compatible,"qcom,mmsscc-8992" )&&
		strcmp(compatible,"qcom,msm-cam"     )&&
		strcmp(compatible,"qcom,csiphy-v3.1.1")&&
		strcmp(compatible,"qcom,csiphy"      )&&
		strcmp(compatible,"qcom,csid"        )&&
		strcmp(compatible,"qcom,ispif-v3.0"  )&&
		strcmp(compatible,"qcom,ispif"       )&&
		strcmp(compatible,"qcom,vfe44"       )&&
		strcmp(compatible,"qcom,vfe44"       )&&
		strcmp(compatible,"qcom,jpeg"        )&&
		strcmp(compatible,"qcom,jpeg_dma"    )&&
		strcmp(compatible,"qcom,irqrouter"   )&&
		strcmp(compatible,"qcom,cpp"         )&&
		strcmp(compatible,"qcom,cci"	)&&
		strcmp(compatible,"qcom,csid-v3.1"	)&&
		strcmp(compatible,"qcom,actuator"	)&&
		strcmp(compatible,"qcom,eeprom")&&
		strcmp(compatible,"qcom,camera") 
		)
		{
			rpc_do_flag=0;
		}
		else
		{
			rpc_do_flag=1;

		}
	}
//Charm end	
	dev->pins = devm_kzalloc(dev, sizeof(*(dev->pins)), GFP_KERNEL);
		if (!dev->pins)
			return -ENOMEM;
	if(rpc_do_flag){

		dev->pins->p = devm_pinctrl_get__remote(dev);
		if (IS_ERR__remote(dev->pins->p)) {
			dev_dbg(dev, "no pinctrl handle\n");
			ret = PTR_ERR__remote(dev->pins->p);
			goto cleanup_alloc__remote;
		}

		dev->pins->default_state = pinctrl_lookup_state__remote(dev->pins->p,
						PINCTRL_STATE_DEFAULT);
		if (IS_ERR__remote(dev->pins->default_state)) {
			dev_dbg(dev, "no default pinctrl state\n");
			ret = 0;
			goto cleanup_get__remote;
		}

		ret = pinctrl_select_state__remote(dev->pins->p, dev->pins->default_state);
		if (ret) {
			dev_dbg(dev, "failed to activate default pinctrl state\n");
			goto cleanup_get__remote;
		}

#ifdef CONFIG_PM
		/*
		 * If power management is enabled, we also look for the optional
		 * sleep and idle pin states, with semantics as defined in
		 * <linux/pinctrl/pinctrl-state.h>
		 */
		dev->pins->sleep_state = pinctrl_lookup_state__remote(dev->pins->p,
						PINCTRL_STATE_SLEEP);
		if (IS_ERR__remote(dev->pins->sleep_state))
			/* Not supplying this state is perfectly legal */
			dev_dbg(dev, "no sleep pinctrl state\n");

		dev->pins->idle_state = pinctrl_lookup_state__remote(dev->pins->p,
						PINCTRL_STATE_IDLE);
		if (IS_ERR__remote(dev->pins->idle_state))
			/* Not supplying this state is perfectly legal */
			dev_dbg(dev, "no idle pinctrl state\n");
#endif

		return 0;
	}else{	
		dev->pins = devm_kzalloc(dev, sizeof(*(dev->pins)), GFP_KERNEL);
		if (!dev->pins)
			return -ENOMEM;

		dev->pins->p = devm_pinctrl_get(dev);
		if (IS_ERR(dev->pins->p)) {
			dev_dbg(dev, "no pinctrl handle\n");
			ret = PTR_ERR(dev->pins->p);
			goto cleanup_alloc;
		}

		dev->pins->default_state = pinctrl_lookup_state(dev->pins->p,
						PINCTRL_STATE_DEFAULT);
		if (IS_ERR(dev->pins->default_state)) {
			dev_dbg(dev, "no default pinctrl state\n");
			ret = 0;
			goto cleanup_get;
		}

		ret = pinctrl_select_state(dev->pins->p, dev->pins->default_state);
		if (ret) {
			dev_dbg(dev, "failed to activate default pinctrl state\n");
			goto cleanup_get;
		}

#ifdef CONFIG_PM
		/*
		 * If power management is enabled, we also look for the optional
		 * sleep and idle pin states, with semantics as defined in
		 * <linux/pinctrl/pinctrl-state.h>
		 */
		dev->pins->sleep_state = pinctrl_lookup_state(dev->pins->p,
						PINCTRL_STATE_SLEEP);
		if (IS_ERR(dev->pins->sleep_state))
			/* Not supplying this state is perfectly legal */
			dev_dbg(dev, "no sleep pinctrl state\n");

		dev->pins->idle_state = pinctrl_lookup_state(dev->pins->p,
						PINCTRL_STATE_IDLE);
		if (IS_ERR(dev->pins->idle_state))
			/* Not supplying this state is perfectly legal */
			dev_dbg(dev, "no idle pinctrl state\n");
#endif

		return 0;
	}
	/*
	 * If no pinctrl handle or default state was found for this device,
	 * let's explicitly free the pin container in the device, there is
	 * no point in keeping it around.
	 */
cleanup_get:
	devm_pinctrl_put(dev->pins->p);
cleanup_alloc:
	devm_kfree(dev, dev->pins);
	dev->pins = NULL;

	/* Only return deferrals */
	if (ret != -EPROBE_DEFER)
		ret = 0;

	return ret;
cleanup_get__remote:
	devm_pinctrl_put__remote(dev->pins->p);
cleanup_alloc__remote:
	dev->pins = NULL;

	/* Only return deferrals */
	if (ret != -EPROBE_DEFER)
		ret = 0;

	return ret;
}
