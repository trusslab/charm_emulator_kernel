/*this program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef MSM_GPIO_IRQ_H
#define MSM_GPIO_IRQ_H
//configs from bullhead

#undef config_spmi			        
#undef config_spmi_msm_pmic_arb		
#undef config_msm_qpnp_int							
#undef config_pm					
#undef config_pm_runtime				
#undef config_of_spmi					
#undef config_msm_show_resume_irq			
#undef config_hardirqs_sw_resend			
#undef config_smp					
#undef config_gpiolib					
#undef config_pm_sleep					
#undef config_debug_fs				
#undef config_use_pinctrl_irq				
#undef config_of					
#undef config_pinctrl					
#undef config_of_gpio					
#undef config_pm_sleep					
#undef config_gpio_qpnp_pin				
#undef config_pinctrl_msm_tlmm			
//-------------------------------------------------
#define config_spmi			        1
#define config_spmi_msm_pmic_arb		1
#define config_msm_qpnp_int			1				
#define config_pm				1	
#define config_pm_runtime			1	
#define config_of_spmi				1	
#define config_msm_show_resume_irq		1	
#define config_hardirqs_sw_resend		1	
#define config_smp				1	
#define config_gpiolib				1	
#define config_pm_sleep				1	
#define config_debug_fs				1
#define config_use_pinctrl_irq			1	
#define config_of				1	
#define config_pinctrl				1	
#define config_of_gpio				1	
#define config_pm_sleep				1	
#define config_gpio_qpnp_pin			1	
#define config_pinctrl_msm_tlmm			1
//---------------------------------------------------
#include <linux/irq.h>

#if (defined(CONFIG_GPIO_MSM_V1) || defined(CONFIG_GPIO_MSM_V2) \
	|| defined(CONFIG_GPIO_MSM_V3) && !defined(CONFIG_USE_PINCTRL_IRQ))
int __init msm_gpio_of_init(struct device_node *node,
					struct device_node *parent);
extern struct irq_chip msm_gpio_irq_extn;
static inline int __init msm_tlmm_of_irq_init(struct device_node *node,
					struct device_node *parent)
{
	return 0;
}
#elif defined(CONFIG_PINCTRL_MSM_TLMM)
int __init msm_tlmm_of_irq_init(struct device_node *node,
					struct device_node *parent);
extern struct irq_chip mpm_tlmm_irq_extn;
static inline int __init msm_gpio_of_init(struct device_node *node,
					struct device_node *parent)
{
	return 0;
}
#else
static inline int __init msm_tlmm_of_irq_init(struct device_node *node,
					      struct device_node *parent)
{
	return 0;
}

static inline int __init msm_gpio_of_init(struct device_node *node,
					struct device_node *parent)
{
	return 0;
}
#endif
#endif

