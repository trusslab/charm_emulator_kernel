/* Copyright (C) 2016-2018 University of California, Irvine
 * 
 * Authors:
 * Seyed Mohammadjavad Seyed Talebi <mjavad@uci.edu>
 * Ardalan Amiri Sani <arrdalan@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LINUX_CHARM_RPC_H__
#define __LINUX_CHARM_RPC_H__

#include <linux/device.h>
//#include <linux/pinctrl/core.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/rpm-smd-regulator.h>
#include <linux/clk/msm-clk.h>
#include <linux/iommu.h>
#include <linux/msm_iommu_domains.h>
#include <linux/power_supply.h>

#define clk_get(dev,con_id)					 	clk_get__remote(dev,con_id)					 
#define devm_clk_get(dev,con_id)					 clk_get__remote(dev,con_id)					 
#define clk_put(clk) 							clk_put__remote(clk) 						
#define devm_clk_put(clk) 							clk_put__remote(clk) 						
#define clk_unprepare(clk) 						clk_unprepare__remote(clk) 					
#define clk_disable(clk) 						clk_disable__remote(clk) 					
#define clk_round_rate(clk,rate) 					clk_round_rate__remote(clk,rate) 				
#define clk_set_rate(clk,rate) 						clk_set_rate__remote(clk,rate) 				
#define clk_get_rate(clk) 						clk_get_rate__remote(clk) 					
#define clk_prepare(clk) 						clk_prepare__remote(clk) 						
#define clk_prepare_enable(clk) 					clk_prepare_enable__remote(clk) 						
#define clk_disable_unprepare(clk) 					clk_disable_unprepare__remote(clk) 						
#define clk_enable(clk) 						clk_enable__remote(clk) 						
#define clk_set_parent(clk,parent) 					clk_set_parent__remote(clk,parent) 					
#define clk_reset(clk,action) 						clk_reset__remote(clk,action) 					

#define regulator_get(dev,con_id) 					regulator_get__remote(dev,con_id) 						
#define devm_regulator_get(dev,con_id) 					regulator_get__remote(dev,con_id) 						
#define regulator_enable(regul) 					regulator_enable__remote(regul) 					
#define regulator_set_voltage(regul,vmin,vmax) 				regulator_set_voltage__remote(regul,vmin,vmax) 				
#define regulator_set_optimum_mode(regul,iop) 				regulator_set_optimum_mode__remote(regul,iop) 						
#define regulator_count_voltages(regul) 				regulator_count_voltages__remote(regul) 					
#define regulator_disable(regul) 					regulator_disable__remote(regul) 					
#define regulator_put(regul) 						regulator_put__remote(regul) 							
#define devm_regulator_put(regul) 						regulator_put__remote(regul) 							
#define regulator_is_enabled(regul) 					regulator_is_enabled__remote(regul) 							
#define regulator_bulk_get(dev,num_consumers,consumers)			regulator_bulk_get__remote(dev,num_consumers,consumers)	

#define rpm_regulator_get(dev,supply)					rpm_regulator_get__remote(dev,supply)						 		
#define rpm_regulator_put(regulator) 					rpm_regulator_put__remote(regulator) 								
#define rpm_regulator_enable(regulator) 				rpm_regulator_enable__remote(regulator) 					
#define rpm_regulator_disable(regulator) 				rpm_regulator_disable__remote(regulator) 						
#define rpm_regulator_set_voltage(regulator,min_uV,max_uV) 		rpm_regulator_set_voltage__remote(regulator,min_uV,max_uV) 					
#define rpm_regulator_set_mode(regulator,mode) 				rpm_regulator_set_mode__remote(regulator,mode) 				

#define pinctrl_get(dev) 						pinctrl_get__remote(dev) 							
#define devm_pinctrl_put(p) 						devm_pinctrl_put__remote(p) 								
#define devm_pinctrl_get(dev) 						devm_pinctrl_get__remote(dev) 									
#define pinctrl_lookup_state(p,name) 					pinctrl_lookup_state__remote(p,name) 						
#define pinctrl_select_state(p,state) 					pinctrl_select_state__remote(p,state) 						
        							       							
#define gpio_request_one(gpio, flags, label) 				gpio_request_one__remote(gpio, flags, label) 						
#define gpio_request(gpio,label)					gpio_request__remote(gpio,label)
#define of_get_gpio(np,index) 						of_get_gpio__remote(np,index) 							
#define of_get_named_gpio(np,propname,index) 				of_get_named_gpio__remote(np,propname,index) 						
#define gpio_set_value_cansleep(gpio,value) 				gpio_set_value_cansleep__remote(gpio,value) 					

#define gpio_get_value(gpio)						gpio_get_value__remote(gpio)				
#define gpio_to_irq(gpio)			                        gpio_to_irq__remote(gpio)		
#define irq_to_gpio(irq)			                        irq_to_gpio__remote(irq)		
#define gpio_set_value(gpio,value)		                        gpio_set_value__remote(gpio,value)	
#define gpio_direction_input(gpio)					gpio_direction_input__remote(gpio)
#define gpio_direction_output(gpio,value)				gpio_direction_output__remote(gpio,value)	
#define gpio_free(gpio) 						gpio_free__remote(gpio)
#define gpio_free_array(array,num)					gpio_free_array__remote(array,num) 
 

#define IS_ERR(ptr)							IS_ERR__remote(ptr)					 
#define PTR_ERR(ptr)							PTR_ERR__remote(ptr)					 
#define IS_ERR_OR_NULL(ptr) 						IS_ERR_OR_NULL__remote(ptr) 							

#define pm_runtime_barrier(dev) 					pm_runtime_barrier__remote(dev) 						
#define pm_request_idle(dev) 						pm_request_idle__remote(dev) 							

#define msm_iommu_get_ctx(ctx_name)					msm_iommu_get_ctx__remote(ctx_name)
#define msm_get_iommu_domain(domain_num)				msm_get_iommu_domain__remote(domain_num)
#define msm_register_domain(layout)					msm_register_domain__remote(layout)
#define iommu_attach_device(domain,dev)					iommu_attach_device__remote(domain,dev)
#define iommu_detach_device(domain,dev)					iommu_detach_device__remote(domain,dev)

#define msm_ion_client_create(name)					msm_ion_client_create__remote(name)
#define ion_client_destroy(client)					ion_client_destroy__remote(client)
#define ion_free(client,handle)						ion_free__remote(client, handle)

#define ion_map_iommu(client,handle,domain_num,partition_num,align,iova_length,iova,buffer_size,flags,iommu_flags)	ion_map_iommu__remote(client,handle,domain_num,partition_num,align,iova_length,iova,buffer_size,flags,iommu_flags)

#define ion_unmap_iommu(client,handle, domain_num,partition_num)	ion_unmap_iommu(client,handle, domain_num,partition_num)
#define ion_import_dma_buf(client,fd)					ion_import_dma_buf__remote(client,fd)
/* FIXME implemet these later*/
#define iommu_group_find(name)						iommu_group_find__remote(name)
#define power_supply_get_by_name(name)					power_supply_get_by_name__remote(name)
#define power_supply_set_hi_power_state(psy,value)			power_supply_set_hi_power_state__remote(psy,value)
#define power_supply_set_low_power_state(psy,value)			power_supply_set_low_power_state__remote(psy,value)



//----------------------------------------------------------------------------------------
struct clk *clk_get__remote(struct device *dev, const char *con_id);
int clk_put__remote(struct clk *clk);
int clk_unprepare__remote(struct clk *clk);
int clk_disable__remote(struct clk *clk);
long clk_round_rate__remote(struct clk *clk, unsigned long rate);
int clk_set_rate__remote(struct clk *clk, unsigned long rate);
unsigned long clk_get_rate__remote(struct clk *clk);
int clk_prepare__remote(struct clk *clk);
int clk_prepare_enable__remote(struct clk *clk);
void clk_disable_unprepare__remote(struct clk *clk);
int clk_enable__remote(struct clk *clk);
int clk_set_parent__remote(struct clk *clk, struct clk *parent);
int clk_reset__remote(struct clk *clk, enum clk_reset_action action);


struct regulator *regulator_get__remote(struct device * dev, const char *con_id);
int regulator_enable__remote(struct regulator * regul);
int regulator_set_voltage__remote(struct regulator *regul, int vmin, int vmax);
int regulator_set_optimum_mode__remote(struct regulator * regul, int iop);
int regulator_count_voltages__remote(struct regulator *regul);
int regulator_disable__remote(struct regulator * regul);
int regulator_put__remote(struct regulator * regul);
int regulator_is_enabled__remote(struct regulator *regulator);
int regulator_bulk_get__remote(struct device *dev, int num_consumers, struct regulator_bulk_data *consumers);


struct rpm_regulator *rpm_regulator_get__remote(struct device *dev, const char *supply);
void rpm_regulator_put__remote(struct rpm_regulator *regulator);
int rpm_regulator_enable__remote(struct rpm_regulator *regulator);
int rpm_regulator_disable__remote(struct rpm_regulator *regulator);
int rpm_regulator_set_voltage__remote(struct rpm_regulator *regulator, int min_uV,
                              int max_uV);
int rpm_regulator_set_mode__remote(struct rpm_regulator *regulator,
                                enum rpm_regulator_mode mode);



struct pinctrl *pinctrl_get__remote(struct device *dev);
void devm_pinctrl_put__remote(struct pinctrl *p);
struct pinctrl *devm_pinctrl_get__remote(struct device *dev);
struct pinctrl_state *pinctrl_lookup_state__remote(struct pinctrl *p,const char *name);
int pinctrl_select_state__remote(struct pinctrl *p, struct pinctrl_state *state);

int gpio_request_one__remote(unsigned gpio,unsigned long flags, const char *label);
int gpio_request__remote(unsigned gpio, const char *label);
int of_get_gpio__remote(struct device_node *np, int index);
int of_get_named_gpio__remote(struct device_node *np,const char *propname, int index);
void gpio_set_value_cansleep__remote(unsigned gpio, int value);

int gpio_get_value__remote(unsigned gpio);
int gpio_to_irq__remote(unsigned gpio);
int irq_to_gpio__remote(unsigned irq);
void gpio_set_value__remote(unsigned gpio, int value);
int gpio_direction_input__remote(unsigned gpio);
int gpio_direction_output__remote(unsigned gpio, int value);
void gpio_free__remote(unsigned gpio);
void gpio_free_array__remote(const struct gpio *array, size_t num);

bool IS_ERR__remote(const void *ptr);
long IS_ERR_OR_NULL__remote(const void *ptr);
bool PTR_ERR__remote(const void *ptr);

int pm_runtime_barrier__remote(struct device *dev);
int pm_request_idle__remote(struct device *dev);


struct device *msm_iommu_get_ctx__remote(const char *ctx_name);
struct iommu_domain *msm_get_iommu_domain__remote(int domain_num);
int msm_register_domain__remote(struct msm_iova_layout *layout);
int iommu_attach_device__remote(struct iommu_domain *domain, struct device *dev);
void iommu_detach_device__remote(struct iommu_domain *domain, struct device *dev);
struct ion_client *msm_ion_client_create__remote(const char *name);
void ion_client_destroy__remote(struct ion_client *client);
void ion_free__remote(struct ion_client *client, struct ion_handle *handle);
int ion_map_iommu__remote(struct ion_client *client, struct ion_handle *handle,
                        int domain_num, int partition_num, unsigned long align,
                        unsigned long iova_length, ion_phys_addr_t *iova,
                        unsigned long *buffer_size,
                        unsigned long flags, unsigned long iommu_flags);
void ion_unmap_iommu__remote(struct ion_client *client, struct ion_handle *handle,
                        int domain_num, int partition_num);
struct ion_handle *ion_import_dma_buf__remote(struct ion_client *client, int fd);

signed long clk_ops_list_rate__remote( struct clk *clk , uint32_t i);
/* FIXME implement these rpcs later */
struct iommu_group *iommu_group_find__remote(const char *name);
struct power_supply *power_supply_get_by_name__remote(const char *name);
int power_supply_set_hi_power_state__remote(struct power_supply *psy, int value);
int power_supply_set_low_power_state__remote(struct power_supply *psy, int value);

#endif /* __LINUX_CHARM_RPC_H__ */
