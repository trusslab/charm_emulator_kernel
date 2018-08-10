obj-$(CONFIG_PM_DEVFREQ)	+= devfreq.o
obj-$(CONFIG_DEVFREQ_GOV_SIMPLE_ONDEMAND)	+= governor_simpleondemand.o
obj-$(CONFIG_DEVFREQ_GOV_PERFORMANCE)	+= governor_performance.o
obj-$(CONFIG_DEVFREQ_GOV_POWERSAVE)	+= governor_powersave.o
obj-$(CONFIG_DEVFREQ_GOV_USERSPACE)	+= governor_userspace.o

# DEVFREQ Drivers
obj-$(CONFIG_ARM_EXYNOS4_BUS_DEVFREQ)	+= exynos/
obj-$(CONFIG_ARM_EXYNOS5_BUS_DEVFREQ)	+= exynos/
# Charm start
#obj-$(CONFIG_PM_DEVFREQ)        += devfreq_trace.o
obj-$(CONFIG_MSM_BIMC_BWMON)            += bimc-bwmon.o
obj-$(CONFIG_MSM_DEVFREQ_DEVBW)         += devfreq_devbw.o
obj-$(CONFIG_DEVFREQ_GOV_MSM_BW_HWMON)  += governor_bw_hwmon.o
obj-$(CONFIG_DEVFREQ_GOV_MSM_GPUBW_MON) += governor_gpubw_mon.o
obj-$(CONFIG_DEVFREQ_GOV_MSM_GPUBW_MON) += governor_bw_vbif.o
# Charm end
