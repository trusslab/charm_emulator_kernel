obj-$(CONFIG_QCOM_GSBI)	+=	qcom_gsbi.o
#Charm
#obj-y			+=	mpm-of.o
#obj-y           += qdsp6v2/
#Charm start
ifdef CONFIG_MSM_SUBSYSTEM_RESTART
       obj-y += subsystem_notif.o
       obj-y += subsystem_restart.o
       obj-y += ramdump.o
endif

obj-$(CONFIG_MSM_QMI_INTERFACE) += qmi_interface.o
#Charm end
# Charm start
obj-$(CONFIG_MSM_OCMEM) += ocmem.o ocmem_allocator.o ocmem_notifier.o
obj-$(CONFIG_MSM_OCMEM) += ocmem_sched.o ocmem_api.o ocmem_rdm.o ocmem_core.o
# Charm end
