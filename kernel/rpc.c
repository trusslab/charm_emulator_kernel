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

#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/Charm/prints.h>
#include <linux/of.h>
#include <linux/pinctrl/consumer.h>
#include <linux/regulator/rpm-smd-regulator.h>
#include <linux/slab.h>
#include <linux/clk/msm-clk.h>
#include <linux/iommu.h>
#include <linux/msm_iommu_domains.h>
#include "../drivers/staging/android/ion/ion.h"

#define CLK_GET_RPC_CODE 1
#define CLK_ROUND_RATE_RPC_CODE 2
#define CLK_SET_RATE_RPC_CODE 3
#define CLK_GET_RATE_RPC_CODE 4
#define CLK_PREPARE_RPC_CODE 5
#define CLK_ENABLE_RPC_CODE 6
#define REGULATOR_GET_RPC_CODE 7
#define REGULATOR_ENABLE_RPC_CODE 8
#define REGULATOR_SET_VOLTAGE_RPC_CODE 9
#define REGULATOR_SET_OPT_RPC_CODE 10
#define REGULATOR_COUNT_VOLTAGES_RPC_CODE 11
#define PINCTRL_GET_RPC_CODE 12
#define PINCTRL_LOOKUP_STATE_RPC_CODE 13
#define PINCTRL_SELECT_STATE_RPC_CODE 14
#define GPIO_REQUEST_ONE_RPC_CODE 15
#define IS_ERR_RPC_CODE 16
#define PINCTRL_PUT_RPC_CODE 17
#define  IS_ERR_OR_NULL_RPC_CODE 18
#define PM_REQUEST_IDLE_RPC_CODE 19
#define PM_RUNTIME_BARRIER_RPC_CODE 20
#define OF_GET_GPIO_RPC_CODE 21
#define OF_GET_NAMED_GPIO_RPC_CODE 22

#define CLK_DISABLE_RPC_CODE 23
#define CLK_UNPREPARE_RPC_CODE 24
#define CLK_PUT_RPC_CODE 25

#define REGULATOR_DISABLE_RPC_CODE 26
#define REGULATOR_PUT_RPC_CODE 27

#define CLK_SET_PARENT_RPC_CODE 28

#define GPIO_SET_VALUE_CAN_SLEEP_RPC_CODE 29
#define RPM_REGULATOR_GET_RPC_CODE 30
#define RPM_REGULATOR_SET_MODE_RPC_CODE 31
#define RPM_REGULATOR_PUT_RPC_CODE 32
#define RPM_REGULATOR_ENABLE_RPC_CODE 33
#define RPM_REGULATOR_DISABLE_RPC_CODE 34
#define RPM_REGULATOR_SET_VOLTAGE_RPC_CODE 35
#define GPIO_REQUEST_RPC_CODE 36
#define GPIO_SET_VALUE_RPC_CODE 37
#define GPIO_GET_VALUE_RPC_CODE 38
#define GPIO_TO_IRQ_RPC_CODE 39
#define IRQ_TO_GPIO_RPC_CODE 40
#define GPIO_DIRECTION_OUTPUT_RPC_CODE 41
#define GPIO_DIRECTION_INPUT_RPC_CODE 42
#define CLK_RESET_RPC_CODE 43
#define MSM_IOMMU_GET_CTX_RPC_CODE 44
#define MSM_GET_IOMMU_DOMAIN_RPC_CODE 45
#define MSM_REGISTER_DOMAIN_RPC 46
#define IOMMU_ATTACH_DEVICE_RPC_CODE 47
#define IOMMU_DETACH_DEVICE_RPC_CODE 48
#define MSM_ION_CLIENT_CREATE_RPC_CODE 49
#define ION_CLIENT_DESTROY_RPC_CODE 50
#define ION_FREE_RPC_CODE 51
#define ION_MAP_IOMMU_RPC_CODE 52
#define ION_UNMAP_IOMMU_RPC_CODE 53
#define ION_IMPORT_DMA_RPC_CODE 54
#define GPIO_FREE_RPC_CODE 55
#define PTR_ERR_RPC_CODE 56

#define MSM_IOMMU_GROUP_FIND_RPC_CODE 57
#define POWER_SUPPLY_GET_BY_NAME_RPC_CODE 58
#define POWER_SUPPLY_SET_HI_POWER_STATE_RPC_CODE 59
#define POWER_SUPPLY_SET_LOW_POWER_STATE_RPC_CODE 60
#define CLK_OPS_LIST_RATE_RPC_CODE 61
#define REGULATOR_IS_ENABLED_RPC_CODE 62
#define CLK_GET_PARENT_RPC_CODE 63
#define CLK_GET_SYS_RPC_CODE 64
#define GPIO_EXPORT_RPC_CODE 65
#define GPIO_IS_VALID_RPC_CODE 66
/* Exynos */
#define EXYNOS_UPDATE_IP_IDLE_STATUS 67
#define EXYNOS_GET_IDLE_IP_INDEX 68
#define EXYNOS_SMC 69

#define REGULATOR_GET_VOLTAGE_RPC_CODE 70
//#undef PRINTKL
//#define PRINTKL(fmt,args...)

extern void * rpc_to_phone(void * msg, unsigned long int len);

struct clk *clk_get__remote(struct device *dev, const char *con_id)
{
        const char *dev_id = dev ? dev_name(dev) : NULL;
        struct clk *clk;
	
	//void * clk_ptr;
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = dev->of_node->name;
	compatible=of_get_property(dev->of_node,"compatible",&len_compatible);

	PRINTKL("[1]dev_id=%s,con_id=%s , of_node_name=%s,compatible=%s",dev_id,con_id,of_node_name,compatible);
	offset=0;
	rpc_code=CLK_GET_RPC_CODE;
	msg_len= strlen(dev_id)+1 + strlen (con_id)+1 + strlen(of_node_name)+1  +3*sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(dev_id)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,dev_id, msg_len);
	offset+=msg_len;



	msg_len=strlen(con_id)+1;	
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, con_id, msg_len);
	offset+= msg_len;


	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&clk, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	return clk;
}
struct clk *clk_get_sys__remote(const char *dev_id, const char *con_id){

        struct clk *clk;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	PRINTKL("[1]dev_id=%s,con_id=%s",dev_id,con_id);
	offset=0;
	rpc_code=CLK_GET_SYS_RPC_CODE;
	msg_len= strlen(dev_id)+1 + strlen (con_id)+1 +2*sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(dev_id)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,dev_id, msg_len);
	offset+=msg_len;



	msg_len=strlen(con_id)+1;	
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, con_id, msg_len);
	offset+= msg_len;



 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&clk, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	return clk;

}
long clk_round_rate__remote(struct clk *clk, unsigned long rate)
{
        long rrate;	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	PRINTKL("");

	offset=0;
	rpc_code=CLK_ROUND_RATE_RPC_CODE;
	msg_len=  8+sizeof(unsigned long)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
	memcpy(rpc_buf+offset, &clk , 8 );
	offset+=8;
	memcpy(rpc_buf+offset, &rate , sizeof(unsigned long));
	offset+= sizeof(unsigned long);
	
 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&rrate,&return_data, sizeof(long));
	
	kfree(rpc_buf);
	 return rrate;

}
int clk_set_rate__remote(struct clk *clk, unsigned long rate)
{
        int rc = 0;
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code=CLK_SET_RATE_RPC_CODE;
        msg_len=  8+sizeof(unsigned long)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;
        memcpy(rpc_buf+offset, &rate , sizeof(unsigned long));
        offset+= sizeof(unsigned long);

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));

	kfree(rpc_buf);
	 return rc;
}

int clk_reset__remote(struct clk *clk, enum clk_reset_action action){

        int rc = 0;
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	unsigned long unl_action;
	unl_action = (unsigned long)action;
	PRINTKL("");
        offset=0;
        rpc_code=CLK_RESET_RPC_CODE;
        msg_len=  8+sizeof(unsigned long)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;
        memcpy(rpc_buf+offset, &unl_action , sizeof(unsigned long));
        offset+= sizeof(unsigned long);

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));

	kfree(rpc_buf);
	 return rc;



}


unsigned long clk_get_rate__remote(struct clk *clk)
{
	unsigned long rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code=CLK_GET_RATE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	kfree(rpc_buf);
	 return rc;

}

int clk_disable__remote(struct clk *clk)
{
	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= CLK_DISABLE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	if(rc!=999){
		PRINTKL("charm is disconnected or phone crashed");
		BUG();
	}
	kfree(rpc_buf);
	 return rc;
}


int clk_unprepare__remote(struct clk *clk)
{
	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= CLK_UNPREPARE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}


int clk_put__remote(struct clk *clk)
{
	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= CLK_PUT_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}
int clk_prepare__remote(struct clk *clk)
{
	int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= CLK_PREPARE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;

}



int clk_set_parent__remote(struct clk *clk, struct clk *parent)
{

	int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= CLK_SET_PARENT_RPC_CODE;
        msg_len=  8+8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;

        memcpy(rpc_buf+offset, &parent , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}


struct clk *clk_get_parent__remote(struct clk *clk)
{
        struct clk *parent_clk;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= CLK_GET_PARENT_RPC_CODE;
        msg_len=  8+8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;


 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&parent_clk, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	return parent_clk;
}

int clk_enable__remote(struct clk *clk)
{
	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= CLK_ENABLE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
	PRINTKL("msg_len=%d",msg_len);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &clk , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}

int clk_prepare_enable__remote(struct clk *clk)
{
         int ret;
 
         ret = clk_prepare__remote(clk);
         if (ret)
                 return ret;
         ret = clk_enable__remote(clk);
         if (ret)
                 clk_unprepare__remote(clk);
 
         return ret;
}
void clk_disable_unprepare__remote(struct clk *clk)
{
         clk_disable__remote(clk);
         clk_unprepare__remote(clk);
}
struct rpm_regulator *rpm_regulator_get__remote(struct device *dev, const char *con_id)
{


        const char *dev_id = dev ? dev_name(dev) : NULL;
        struct rpm_regulator *regul;
	
	//void * regul_ptr;
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = dev->of_node->name;
	compatible=of_get_property(dev->of_node,"compatible",&len_compatible);

	PRINTKL("[1]dev_id=%s,con_id=%s , of_node_name=%s,compatible=%s",dev_id,con_id,of_node_name,compatible);
	offset=0;
	rpc_code=RPM_REGULATOR_GET_RPC_CODE;
	msg_len= strlen(dev_id)+1 + strlen (con_id)+1 + strlen(of_node_name)+1  +2*sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(dev_id)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,dev_id, msg_len);
	offset+=msg_len;



	msg_len=strlen(con_id)+1;	
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, con_id, msg_len);
	offset+= msg_len;


	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&regul, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	 return regul;
}

int rpm_regulator_set_mode__remote(struct rpm_regulator *regulator,
                                 enum rpm_regulator_mode mode)
{
	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= RPM_REGULATOR_SET_MODE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code)+sizeof(mode);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regulator , 8 );
        offset+=8;
	
	
        memcpy(rpc_buf+offset, &mode , sizeof(mode) );
        offset+=sizeof(mode);

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;

}

void rpm_regulator_put__remote(struct rpm_regulator *regulator)
{


	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= RPM_REGULATOR_PUT_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regulator , 8 );
        offset+=8;
	

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return;

}


int rpm_regulator_enable__remote(struct rpm_regulator *regulator)
{


	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= RPM_REGULATOR_ENABLE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regulator , 8 );
        offset+=8;
	

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;

}

int rpm_regulator_disable__remote(struct rpm_regulator *regulator)
{


	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= RPM_REGULATOR_DISABLE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regulator , 8 );
        offset+=8;
	

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;

}
int rpm_regulator_set_voltage__remote(struct rpm_regulator *regulator, int min_uV,
                               int max_uV)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= RPM_REGULATOR_SET_VOLTAGE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code)+sizeof(min_uV)+sizeof(max_uV);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);

        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regulator , 8 );
        offset+=8;
	
	
        memcpy(rpc_buf+offset, &min_uV , sizeof(min_uV) );
        offset+=sizeof(min_uV);

        memcpy(rpc_buf+offset, &max_uV , sizeof(max_uV) );
        offset+=sizeof(max_uV);

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;


}


struct regulator *regulator_get__remote(struct device * dev, const char *con_id)
{

        const char *dev_id = dev ? dev_name(dev) : NULL;
        struct regulator *regul;
	
	//void * regul_ptr;
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = dev->of_node->name;
	compatible=of_get_property(dev->of_node,"compatible",&len_compatible);

	PRINTKL("[1]dev_id=%s,con_id=%s , of_node_name=%s,compatible=%s",dev_id,con_id,of_node_name,compatible);
	offset=0;
	rpc_code=REGULATOR_GET_RPC_CODE;
	msg_len= strlen(dev_id)+1 + strlen (con_id)+1 + strlen(of_node_name)+1  +2*sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(dev_id)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,dev_id, msg_len);
	offset+=msg_len;



	msg_len=strlen(con_id)+1;	
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, con_id, msg_len);
	offset+= msg_len;


	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&regul, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	 return regul;
}
int regulator_enable__remote(struct regulator * regul)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_ENABLE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}


int regulator_disable__remote(struct regulator * regul)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_DISABLE_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}

int regulator_is_enabled__remote(struct regulator * regul)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_IS_ENABLED_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}
int regulator_put__remote(struct regulator * regul)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_PUT_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}
int regulator_set_voltage__remote(struct regulator *regul, int vmin, int vmax)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_SET_VOLTAGE_RPC_CODE;
        msg_len=  8+2*sizeof(int) +sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;
	memcpy(rpc_buf+offset, &vmin, sizeof(int));
	offset+=sizeof(int);
	memcpy(rpc_buf+offset, &vmax, sizeof(int));
	offset+=sizeof(int);

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}
int regulator_get_voltage__remote(struct regulator *regul)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_GET_VOLTAGE_RPC_CODE;
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	return rc;
}
int regulator_set_optimum_mode__remote(struct regulator * regul, int iop)
{


	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_SET_OPT_RPC_CODE;
        msg_len=  8+sizeof(int) +sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;
	memcpy(rpc_buf+offset, &iop, sizeof(int));
	offset+=sizeof(int);

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}
int regulator_count_voltages__remote(struct regulator *regul)
{

	int rc;
	
        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code; 
        unsigned long int offset;
	PRINTKL("");
        offset=0;
        rpc_code= REGULATOR_COUNT_VOLTAGES_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &regul , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

	memcpy( &rc, &return_data, sizeof(int));
	PRINTKL("rc=%d",rc);
	kfree(rpc_buf);
	 return rc;
}
struct pinctrl *pinctrl_get__remote(struct device *dev)
{

        const char *dev_id = dev ? dev_name(dev) : NULL;
        struct  pinctrl *pinctrl;
	
	//void * pinctrl_ptr;
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = dev->of_node->name;
	compatible=of_get_property(dev->of_node,"compatible",&len_compatible);

	PRINTKL("[1]dev_id=%s, of_node_name=%s,compatible=%s",dev_id,of_node_name,compatible);
	offset=0;
	rpc_code=PINCTRL_GET_RPC_CODE;
	msg_len= strlen(dev_id)+1  + strlen(of_node_name)+1  +2*sizeof(msg_len)+sizeof(rpc_code);
	PRINTKL("[2]: msg_len = %d", msg_len);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	PRINTKL("[3]");
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(dev_id)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,dev_id, msg_len);
	offset+=msg_len;


	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&pinctrl, &return_data, 8);

	PRINTKL("[2] fandogh rpc_buf=%lx",(unsigned long)rpc_buf);
        kfree(rpc_buf);
	 return pinctrl;

}

extern void devm_pinctrl_release(struct device *dev, void *res);
void pinctrl_put__remote(struct pinctrl *p)
{

      void * rpc_buf;
      void * return_data;
      int msg_len;
      uint64_t rpc_code;
      unsigned long int offset;

      PRINTKL("");

      offset=0;
      rpc_code= PINCTRL_PUT_RPC_CODE;
      msg_len=  8+sizeof(rpc_code);
      rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
      memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
      offset+=sizeof(rpc_code);

      memcpy(rpc_buf+offset, &p , 8 );
      offset+=8;




      return_data = rpc_to_phone(rpc_buf, offset);
      kfree(rpc_buf);	


}
void devm_pinctrl_put__remote(struct pinctrl *p)
{
	pinctrl_put__remote(p);
/*
      void * rpc_buf;
      void * return_data;
      int msg_len;
      uint64_t rpc_code;
      unsigned long int offset;

      PRINTKL("");

      offset=0;
      rpc_code= DEVM_PINCTRL_PUT_LOOKUP_STATE_RPC_CODE;
      msg_len=  8+sizeof(rpc_code);
      rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
      memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
      offset+=sizeof(rpc_code);

      memcpy(rpc_buf+offset, &p , 8 );
      offset+=8;




      return_data = rpc_to_phone(rpc_buf, offset);
      kfree(rpc_buf);	
*/

}


struct pinctrl *devm_pinctrl_get__remote(struct device *dev)
{
	struct pinctrl *p;
/*
        struct pinctrl **ptr, *p;

        ptr = devres_alloc(devm_pinctrl_release, sizeof(*ptr), GFP_KERNEL);
        if (!ptr)
                return ERR_PTR(-ENOMEM);
*/
        p = pinctrl_get__remote(dev);
/*
        if (!IS_ERR(p)) {
                *ptr = p;
                devres_add(dev, ptr);
        } else {
                devres_free(ptr); 
        }
 */
        return p;
}
struct pinctrl_state *pinctrl_lookup_state__remote(struct pinctrl *p,const char *name)
{
	 struct pinctrl_state * my_state;
         void * rpc_buf;
         void * return_data;
         int msg_len;
         uint64_t rpc_code;
         unsigned long int offset;
 
         PRINTKL("");
 
         offset=0;
         rpc_code= PINCTRL_LOOKUP_STATE_RPC_CODE;
         msg_len=  8+strlen(name)+1+sizeof(rpc_code);
         rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
         memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
         offset+=sizeof(rpc_code);

         memcpy(rpc_buf+offset, &p , 8 );
         offset+=8;


         msg_len=strlen(name)+1;
         memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
         offset+= sizeof(msg_len);
         memcpy(rpc_buf+offset, name, msg_len);
         offset+= msg_len;

 
         return_data = rpc_to_phone(rpc_buf, offset);
 
         memcpy(&my_state,&return_data, 8);
 
         kfree(rpc_buf);
	 return my_state;
 



}

int pinctrl_select_state__remote(struct pinctrl *p, struct pinctrl_state *state)
{
        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= PINCTRL_SELECT_STATE_RPC_CODE;
        msg_len=  8+8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &p , 8 );
        offset+=8;

        memcpy(rpc_buf+offset, &state , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;
}
struct pinctrl * __must_check pinctrl_get_select__remote(
					struct device *dev, const char *name)
{
	struct pinctrl *p;
	struct pinctrl_state *s;
	int ret;

	p = pinctrl_get__remote(dev);
	if (IS_ERR(p))
		return p;

	s = pinctrl_lookup_state__remote(p, name);
	if (IS_ERR(s)) {
		pinctrl_put__remote(p);
		return ERR_PTR(PTR_ERR(s));
	}

	ret = pinctrl_select_state__remote(p, s);
	if (ret < 0) {
		pinctrl_put__remote(p);
		return ERR_PTR(ret);
	}

	return p;
}
struct pinctrl * __must_check pinctrl_get_select_default__remote(
					struct device *dev)
{
	return pinctrl_get_select__remote(dev, PINCTRL_STATE_DEFAULT);
}
void gpio_set_value_cansleep__remote(unsigned gpio, int value)
{


        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
//	gpio_set_value_cansleep(gpio,value);
        offset=0;
        rpc_code= GPIO_SET_VALUE_CAN_SLEEP_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(int)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);

        memcpy(rpc_buf+offset, &value , sizeof(int) );
        offset+=sizeof(int);



        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return;

}

void gpio_free__remote(unsigned gpio)
{
        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
	//gpio_free(gpio);
        offset=0;
        rpc_code= GPIO_FREE_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);


        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return;
}


void gpio_free_array__remote(const struct gpio *array, size_t num)
{
        while (num--)
                gpio_free__remote((array++)->gpio);
}
int gpio_direction_output__remote(unsigned gpio, int value)
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_DIRECTION_OUTPUT_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(int)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);

        memcpy(rpc_buf+offset, &value , sizeof(int) );
        offset+=sizeof(int);



        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;

}

int gpio_direction_input__remote(unsigned gpio)
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_DIRECTION_INPUT_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(int)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);



        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;

}
int gpio_export__remote(unsigned gpio, bool value)
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_EXPORT_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(bool)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);

        memcpy(rpc_buf+offset, &value , sizeof(bool) );
        offset+=sizeof(bool);



        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;

}
bool gpio_is_valid__remote(int number){

        bool rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_IS_VALID_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(bool)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &number , sizeof(int) );
        offset+=sizeof(unsigned);


        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(bool));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;
}
void gpio_set_value__remote(unsigned gpio, int value)
{


        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_SET_VALUE_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(int)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);

        memcpy(rpc_buf+offset, &value , sizeof(int) );
        offset+=sizeof(int);



        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return;
}

int gpio_get_value__remote(unsigned gpio)
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_GET_VALUE_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);




        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;

}
int gpio_to_irq__remote(unsigned gpio)
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_TO_IRQ_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);




        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;

}

//int irq_to_gpio__remote(unsigned irq)
int irq_to_gpio__remote(unsigned gpio )
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= IRQ_TO_GPIO_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);




        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;

}

int gpio_request_one__remote(unsigned gpio,unsigned long flags, const char *label)
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_REQUEST_ONE_RPC_CODE;
        msg_len=  sizeof(unsigned)+sizeof(unsigned long)+strlen(label)+1+8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);

        memcpy(rpc_buf+offset, &flags , sizeof(unsigned long) );
        offset+=sizeof(unsigned long);

        msg_len=strlen(label)+1;
        memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
        offset+= sizeof(msg_len);
        memcpy(rpc_buf+offset, label, msg_len);
        offset+= msg_len;


        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;
}
int gpio_request__remote(unsigned gpio, const char *label)
{

        int rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= GPIO_REQUEST_RPC_CODE;
        msg_len=  sizeof(unsigned)+strlen(label)+1+8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &gpio , sizeof(unsigned) );
        offset+=sizeof(unsigned);


        msg_len=strlen(label)+1;
        memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
        offset+= sizeof(msg_len);
        memcpy(rpc_buf+offset, label, msg_len);
        offset+= msg_len;


        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	 return rc;

}
bool IS_ERR__remote(const void *ptr)
{
	bool  rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= IS_ERR_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &ptr , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(bool));
        PRINTKL("rc=%d",(int)rc);
        kfree(rpc_buf);
	 return rc;

}

bool PTR_ERR__remote(const void *ptr)
{
	bool  rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= PTR_ERR_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &ptr , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(bool));
        PRINTKL("rc=%d",(int)rc);
        kfree(rpc_buf);
	 return rc;

}
long IS_ERR_OR_NULL__remote(const void *ptr)
{


	long  rc;

        void * rpc_buf;
        void * return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= IS_ERR_OR_NULL_RPC_CODE;
        msg_len=  8+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);
        memcpy(rpc_buf+offset, &ptr , 8 );
        offset+=8;

        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(long));
        PRINTKL("rc=%ld",rc);
        kfree(rpc_buf);
	 return rc;
}

int pm_runtime_barrier__remote(struct device *dev)
{
	int rc;
	
        const char *dev_id = dev ? dev_name(dev) : NULL;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = dev->of_node->name;
	compatible=of_get_property(dev->of_node,"compatible",&len_compatible);

	PRINTKL("[1]dev_id=%s, of_node_name=%s,compatible=%s",dev_id,of_node_name,compatible);
	offset=0;
	rpc_code=PM_RUNTIME_BARRIER_RPC_CODE;
	msg_len= strlen(dev_id)+1  + strlen(of_node_name)+1  +2*sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(dev_id)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,dev_id, msg_len);
	offset+=msg_len;


	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&rc,&return_data, sizeof(int));

	PRINTKL("[2] rc =%d",rc);
        kfree(rpc_buf);
	 return rc;

}
int pm_request_idle__remote(struct device *dev)
{
	
	int rc;
	
        const char *dev_id = dev ? dev_name(dev) : NULL;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = dev->of_node->name;
	compatible=of_get_property(dev->of_node,"compatible",&len_compatible);

	PRINTKL("[1]dev_id=%s, of_node_name=%s,compatible=%s",dev_id,of_node_name,compatible);
	offset=0;
	PRINTKL("[2]");
	rpc_code=PM_REQUEST_IDLE_RPC_CODE;
	msg_len= strlen(dev_id)+1  + strlen(of_node_name)+1  +2*sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	PRINTKL("[3]");
	msg_len = strlen(dev_id)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,dev_id, msg_len);
	offset+=msg_len;

	PRINTKL("[4]");

	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

	PRINTKL("[5]");
 	return_data = rpc_to_phone(rpc_buf, offset);
	
	PRINTKL("[6]");
	memcpy(&rc,&return_data, sizeof(int));

	PRINTKL("[7]");
	PRINTKL("[2] rc =%d",rc);
        kfree(rpc_buf);
	 return rc;
	
	
}


	
int of_get_gpio__remote(struct device_node *np, int index)
{


	
	int rc;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = np->name;
	compatible=of_get_property(np,"compatible",&len_compatible);

	PRINTKL("[1] of_node_name=%s,compatible=%s",of_node_name,compatible);
	offset=0;
	rpc_code=OF_GET_GPIO_RPC_CODE;
	msg_len=  strlen(of_node_name)+1  +sizeof(msg_len)+sizeof(rpc_code)+sizeof(index);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

	
	memcpy(rpc_buf+offset, &index, sizeof(index));
	offset+=sizeof(index);

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&rc,&return_data, sizeof(int));

	PRINTKL("[2] rc =%d",rc);
        kfree(rpc_buf);
	 return rc;
	
	


}
int of_get_named_gpio__remote(struct device_node *np,const char *propname, int index)
{


	int rc;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;
	const char *  of_node_name;
	const char *  compatible;
	int len_compatible;
	of_node_name = np->name;
	compatible=of_get_property(np,"compatible",&len_compatible);

	PRINTKL("[1] of_node_name=%s,compatible=%s",of_node_name,compatible);
	offset=0;
	rpc_code=OF_GET_NAMED_GPIO_RPC_CODE;
	msg_len=  strlen(propname)+1+ strlen(of_node_name)+1  + 2*sizeof(msg_len)+sizeof(rpc_code)+sizeof(index);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(compatible)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, compatible, msg_len);
	offset+=msg_len;

	
	memcpy(rpc_buf+offset, &index, sizeof(index));
	offset+=sizeof(index);

	msg_len = strlen(propname)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,propname , msg_len);
	offset+=msg_len;
 	
	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&rc,&return_data, sizeof(int));

	PRINTKL("[2] rc =%d",rc);
        kfree(rpc_buf);
	 return rc;

}
     

int gpio_request_array__remote(const struct gpio *array, size_t num)
{
	int i, err;

	for (i = 0; i < num; i++, array++) {
		err = gpio_request_one__remote(array->gpio, array->flags, array->label);
		if (err)
			goto err_free;
	}
	return 0;

err_free:
	while (i--)
		gpio_free__remote((--array)->gpio);
	return err;
}


struct device *msm_iommu_get_ctx__remote(const char *ctx_name)
{

        struct device *dev;
	
	//void * regul_ptr;
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	PRINTKL("[1]ctx_name=%s",ctx_name);
	offset=0;
	rpc_code=MSM_IOMMU_GET_CTX_RPC_CODE;
	msg_len= strlen(ctx_name)+1  +sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(ctx_name)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,ctx_name, msg_len);
	offset+=msg_len;


 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&dev, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	 return dev;

}



struct iommu_domain *msm_get_iommu_domain__remote(int domain_num)
{

        struct iommu_domain *iommu_domain;
	
	//void * regul_ptr;
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	PRINTKL("[1]domian_num=%d",domain_num);
	offset=0;
	rpc_code=MSM_GET_IOMMU_DOMAIN_RPC_CODE;
	msg_len= sizeof(domain_num) +sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	memcpy(rpc_buf+offset,&domain_num, sizeof(domain_num));
	offset+=sizeof(domain_num);


 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&iommu_domain, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	 return iommu_domain;

}
int msm_register_domain__remote(struct msm_iova_layout *layout)
{


        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code=MSM_REGISTER_DOMAIN_RPC;
	msg_len= sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);



 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ret, &return_data, sizeof(int));

	PRINTKL("[2]ret=%d",ret);
        kfree(rpc_buf);
	 return ret;

}
int iommu_attach_device__remote(struct iommu_domain *domain, struct device *dev)
{

	uint64_t ptr1=(uint64_t)domain;
	uint64_t ptr2=(uint64_t)dev;
	
        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code = IOMMU_ATTACH_DEVICE_RPC_CODE;
	msg_len= sizeof(rpc_code)+sizeof(ptr1)+sizeof(ptr2);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);
	
	
	memcpy(rpc_buf+offset, &ptr1 , sizeof(ptr1));
	offset+=sizeof(ptr1);

	memcpy(rpc_buf+offset, &ptr2 , sizeof(ptr2));
	offset+=sizeof(ptr2);

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ret, &return_data, sizeof(int));

	PRINTKL("[2]ret=%d",ret);
        kfree(rpc_buf);
	 return ret;

}


void iommu_detach_device__remote(struct iommu_domain *domain, struct device *dev)
{

	uint64_t ptr1=(uint64_t)domain;
	uint64_t ptr2=(uint64_t)dev;
	
        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code = IOMMU_DETACH_DEVICE_RPC_CODE;
	msg_len= sizeof(rpc_code)+sizeof(ptr1)+sizeof(ptr2);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);
	
	
	memcpy(rpc_buf+offset, &ptr1 , sizeof(ptr1));
	offset+=sizeof(ptr1);

	memcpy(rpc_buf+offset, &ptr2 , sizeof(ptr2));
	offset+=sizeof(ptr2);

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ret, &return_data, sizeof(int));

	PRINTKL("[2]ret=%d",ret);
        kfree(rpc_buf);
	 return ;

}
struct ion_client *msm_ion_client_create__remote(const char *name)
{

        struct ion_client * ionc;
	
	//void * regul_ptr;
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	PRINTKL("[1]name=%s",name);
	offset=0;
	rpc_code=MSM_ION_CLIENT_CREATE_RPC_CODE;
	msg_len= strlen(name)+1  +sizeof(msg_len)+sizeof(rpc_code);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len = strlen(name)+1;
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset,name, msg_len);
	offset+=msg_len;


 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ionc, &return_data, 8);

	PRINTKL("[2]");
        kfree(rpc_buf);
	 return ionc;

}


void ion_client_destroy__remote(struct ion_client *client)
{

	uint64_t ptr1=(uint64_t)client;
	
        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code = ION_CLIENT_DESTROY_RPC_CODE;
	msg_len= sizeof(rpc_code)+sizeof(ptr1);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);
	
	
	memcpy(rpc_buf+offset, &ptr1 , sizeof(ptr1));
	offset+=sizeof(ptr1);


 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ret, &return_data, sizeof(int));

	PRINTKL("[2]ret=%d",ret);
        kfree(rpc_buf);
	 return ;

}
void ion_free__remote(struct ion_client *client, struct ion_handle *handle)
{

	uint64_t ptr1=(uint64_t)client;
	uint64_t ptr2=(uint64_t)handle;
	
        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code = ION_FREE_RPC_CODE;
	msg_len= sizeof(rpc_code)+sizeof(ptr1)+sizeof(ptr2);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);
	
	
	memcpy(rpc_buf+offset, &ptr1 , sizeof(ptr1));
	offset+=sizeof(ptr1);


	memcpy(rpc_buf+offset, &ptr2 , sizeof(ptr1));
	offset+=sizeof(ptr2);

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ret, &return_data, sizeof(int));

	PRINTKL("[2]ret=%d",ret);
        kfree(rpc_buf);
	 return ;



}
int ion_map_iommu__remote(struct ion_client *client, struct ion_handle *handle,
                        int domain_num, int partition_num, unsigned long align,
                        unsigned long iova_length, ion_phys_addr_t *iova,
                        unsigned long *buffer_size,
                        unsigned long flags, unsigned long iommu_flags)
{

	uint64_t ptr1=(uint64_t)client;
	uint64_t ptr2=(uint64_t)handle;
	uint64_t ptr3=(uint64_t)iova;
	uint64_t ptr4=(uint64_t)buffer_size;
	
        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code = ION_MAP_IOMMU_RPC_CODE;
	msg_len= sizeof(rpc_code)+4*sizeof(ptr1)+ 2*sizeof(int)+4*sizeof(unsigned long);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);
	
	
	memcpy(rpc_buf+offset, &ptr1 , sizeof(ptr1));
	offset+=sizeof(ptr1);


	memcpy(rpc_buf+offset, &ptr2 , sizeof(ptr1));
	offset+=sizeof(ptr2);

	
	memcpy(rpc_buf+offset, &domain_num , sizeof(int));
	offset+=sizeof(int);

	memcpy(rpc_buf+offset, &partition_num, sizeof(int));
	offset+=sizeof(int);

	memcpy(rpc_buf+offset, &align, sizeof(unsigned long));
	offset+=sizeof(unsigned long);

	memcpy(rpc_buf+offset, &iova_length, sizeof(unsigned long));
	offset+=sizeof(unsigned long);

	memcpy(rpc_buf+offset, &ptr3, sizeof(uint64_t));
	offset+=sizeof(uint64_t);

	memcpy(rpc_buf+offset, &ptr4, sizeof(uint64_t));
	offset+=sizeof(uint64_t);

	memcpy(rpc_buf+offset, &flags, sizeof(unsigned long));
	offset+=sizeof(unsigned long);

	memcpy(rpc_buf+offset, &iommu_flags, sizeof(unsigned long));
	offset+=sizeof(unsigned long);


 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ret, &return_data, sizeof(int));

	PRINTKL("[2]ret=%d",ret);
        kfree(rpc_buf);
	 return ret ;

}
void ion_unmap_iommu__remote(struct ion_client *client, struct ion_handle *handle,
                        int domain_num, int partition_num)
{

	uint64_t ptr1=(uint64_t)client;
	uint64_t ptr2=(uint64_t)handle;
	
        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code = ION_UNMAP_IOMMU_RPC_CODE;
	msg_len= sizeof(rpc_code)+sizeof(ptr1)+ 2*sizeof(int);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);
	
	
	memcpy(rpc_buf+offset, &ptr1 , sizeof(ptr1));
	offset+=sizeof(ptr1);


	memcpy(rpc_buf+offset, &ptr2 , sizeof(ptr1));
	offset+=sizeof(ptr2);

	
	memcpy(rpc_buf+offset, &domain_num , sizeof(int));
	offset+=sizeof(int);

	memcpy(rpc_buf+offset, &partition_num, sizeof(int));
	offset+=sizeof(int);

 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ret, &return_data, sizeof(int));

	PRINTKL("[2]ret=%d",ret);
        kfree(rpc_buf);
	 return ;

}
struct ion_handle *ion_import_dma_buf__remote(struct ion_client *client, int fd)
{


	uint64_t ptr1=(uint64_t)client;
	uint64_t ptr2;
	
        int ret;
	
	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code = ION_IMPORT_DMA_RPC_CODE;
	msg_len= sizeof(rpc_code)+sizeof(ptr1)+ sizeof(int);
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);
	
	
	memcpy(rpc_buf+offset, &ptr1 , sizeof(ptr1));
	offset+=sizeof(ptr1);
	
	memcpy(rpc_buf+offset, &fd , sizeof(int));
	offset+=sizeof(int);


 	return_data = rpc_to_phone(rpc_buf, offset);
	
	memcpy(&ptr2, &return_data, sizeof(uint64_t));

	PRINTKL("[2]ret=%d",ptr2);
        kfree(rpc_buf);
	 return (struct ion_handle *)ptr2 ;

}
struct iommu_group *iommu_group_find__remote(const char *name)
{

         struct iommu_group * iommug;
 
         //void * regul_ptr;
         void * rpc_buf;
         void * return_data;
         int msg_len;
         uint64_t rpc_code;
         unsigned long int offset;
 
         PRINTKL("[1]name=%s",name);
         offset=0;
         rpc_code=MSM_IOMMU_GROUP_FIND_RPC_CODE;
         msg_len= strlen(name)+1  +sizeof(msg_len)+sizeof(rpc_code);
         rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
         memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
         offset+=sizeof(rpc_code);
 
         msg_len = strlen(name)+1;
         memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
         offset+= sizeof(msg_len);       
         memcpy(rpc_buf+offset,name, msg_len);
         offset+=msg_len;
 
 
         return_data = rpc_to_phone(rpc_buf, offset);
 
         memcpy(&iommug, &return_data, 8);
 
         PRINTKL("[2]");
         kfree(rpc_buf);
          return iommug;




}

struct power_supply *power_supply_get_by_name__remote(const char *name)
{

         struct power_supply * psup;
 
         //void * regul_ptr;
         void * rpc_buf;
         void * return_data;
         int msg_len;
         uint64_t rpc_code;
         unsigned long int offset;
 
         PRINTKL("[1]name=%s",name);
         offset=0;
         rpc_code=POWER_SUPPLY_GET_BY_NAME_RPC_CODE;
         msg_len= strlen(name)+1  +sizeof(msg_len)+sizeof(rpc_code);
         rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
         memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
         offset+=sizeof(rpc_code);
 
         msg_len = strlen(name)+1;
         memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
         offset+= sizeof(msg_len);       
         memcpy(rpc_buf+offset,name, msg_len);
         offset+=msg_len;
 
 
         return_data = rpc_to_phone(rpc_buf, offset);
 
         memcpy(&psup, &return_data, 8);
 
         PRINTKL("[2]");
         kfree(rpc_buf);
          return psup;

}
int power_supply_set_hi_power_state__remote(struct power_supply *psy, int value)
{
         void * rpc_buf;
	 int ret;
         void * return_data;
         int msg_len;
         uint64_t rpc_code;
         unsigned long int offset;
 
         PRINTKL("");
 
         offset=0;
         rpc_code= POWER_SUPPLY_SET_HI_POWER_STATE_RPC_CODE;
         msg_len=  8+sizeof(unsigned long)+sizeof(rpc_code);
         rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
         memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
         offset+=sizeof(rpc_code);
         memcpy(rpc_buf+offset, &psy , 8 );
         offset+=8;
         memcpy(rpc_buf+offset, &value , sizeof(int));
         offset+= sizeof(int);
 
         return_data = rpc_to_phone(rpc_buf, offset);
 
         memcpy(&ret,&return_data, sizeof(int));
 
         kfree(rpc_buf);
          return ret;
 


}

int power_supply_set_low_power_state__remote(struct power_supply *psy, int value)
{
         void * rpc_buf;
	 int ret;
         void * return_data;
         int msg_len;
         uint64_t rpc_code;
         unsigned long int offset;
 
         PRINTKL("");
 
         offset=0;
         rpc_code= POWER_SUPPLY_SET_LOW_POWER_STATE_RPC_CODE;
         msg_len=  8+sizeof(unsigned long)+sizeof(rpc_code);
         rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
         memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
         offset+=sizeof(rpc_code);
         memcpy(rpc_buf+offset, &psy , 8 );
         offset+=8;
         memcpy(rpc_buf+offset, &value , sizeof(int));
         offset+= sizeof(int);
 
         return_data = rpc_to_phone(rpc_buf, offset);
 
         memcpy(&ret,&return_data, sizeof(int));
 
         kfree(rpc_buf);
          return ret;
 


}
int regulator_bulk_get__remote(struct device *dev, int num_consumers,
                       struct regulator_bulk_data *consumers)
{
        int i;
        int ret;

        for (i = 0; i < num_consumers; i++)
                consumers[i].consumer = NULL;

        for (i = 0; i < num_consumers; i++) {
                consumers[i].consumer = regulator_get__remote(dev,
                                                      consumers[i].supply);
                if (IS_ERR__remote(consumers[i].consumer)) {
                        ret = PTR_ERR__remote(consumers[i].consumer);
                        dev_err(dev, "Failed to get supply '%s': %d\n",
                                consumers[i].supply, ret);
                        consumers[i].consumer = NULL;
                        goto err;
                }
        }

        return 0;

err:
        while (--i >= 0)
                regulator_put__remote(consumers[i].consumer);

        return ret;
}

signed long clk_ops_list_rate__remote( struct clk *clk , uint32_t i)
{
         signed long rrate;
         void * rpc_buf;
         void * return_data;
         int msg_len;
         uint64_t rpc_code;
         unsigned long int offset;
 
         PRINTKL("");
 
         offset=0;
         rpc_code=CLK_OPS_LIST_RATE_RPC_CODE;
         msg_len=  8+sizeof(unsigned long)+sizeof(rpc_code);
         rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
         memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
         offset+=sizeof(rpc_code);
         memcpy(rpc_buf+offset, &clk , 8 );
         offset+=8;
         memcpy(rpc_buf+offset, &i , sizeof(uint32_t));
         offset+= sizeof(uint32_t);
 
         return_data = rpc_to_phone(rpc_buf, offset);
 
         memcpy(&rrate,&return_data, sizeof(long));
 
         kfree(rpc_buf);
          return rrate;

}
/* Exynos */
void exynos_update_ip_idle_status__remote(int ip_index, int idle)
{
	int rc;

        void *rpc_buf;
        void *return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= EXYNOS_UPDATE_IP_IDLE_STATUS;
        msg_len=  sizeof(int)+sizeof(int)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &ip_index , sizeof(int) );
        offset+=sizeof(int);

        memcpy(rpc_buf+offset, &idle, sizeof(int) );
        offset+=sizeof(int);



        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	return;
}

int exynos_get_idle_ip_index__remote(const char *ip_name)
{
	int rc;

	void * rpc_buf;
	void * return_data;
	int msg_len;
	uint64_t rpc_code;
	unsigned long int offset;

	offset=0;
	rpc_code=EXYNOS_GET_IDLE_IP_INDEX;
	rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
	memcpy(rpc_buf+offset, &rpc_code , sizeof(rpc_code));
	offset+=sizeof(rpc_code);

	msg_len=strlen(ip_name)+1;	
	memcpy(rpc_buf+offset, &msg_len , sizeof(msg_len));
	offset+= sizeof(msg_len);	
	memcpy(rpc_buf+offset, ip_name, msg_len);
	offset+= msg_len;

 	return_data = rpc_to_phone(rpc_buf, offset);
	
        memcpy( &rc, &return_data, sizeof(int));

	PRINTKL("[2]");
        kfree(rpc_buf);
	return rc;
}

int exynos_smc__remote(unsigned long cmd, unsigned long arg1, unsigned long arg2, unsigned long arg3)
{
	int rc;

        void *rpc_buf;
        void *return_data;
        int msg_len;
        uint64_t rpc_code;
        unsigned long int offset;
        PRINTKL("");
        offset=0;
        rpc_code= EXYNOS_SMC;
        msg_len=  sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(unsigned long)+sizeof(rpc_code);
        rpc_buf =  kmalloc(PAGE_SIZE, GFP_KERNEL);
        memcpy(rpc_buf+offset,&rpc_code , sizeof(rpc_code));
        offset+=sizeof(rpc_code);

        memcpy(rpc_buf+offset, &cmd , sizeof(unsigned long) );
        offset+=sizeof(unsigned long);

        memcpy(rpc_buf+offset, &arg1, sizeof(unsigned long) );
        offset+=sizeof(unsigned long);

        memcpy(rpc_buf+offset, &arg2 , sizeof(unsigned long) );
        offset+=sizeof(unsigned long);

        memcpy(rpc_buf+offset, &arg3, sizeof(unsigned long) );
        offset+=sizeof(unsigned long);

        return_data = rpc_to_phone(rpc_buf, offset);

        memcpy( &rc, &return_data, sizeof(int));
        PRINTKL("rc=%d",rc);
        kfree(rpc_buf);
	return;
}
