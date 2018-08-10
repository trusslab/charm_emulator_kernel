dtc -I dts -O dtb charm.dts > charm.dtb
xxd -i charm.dtb > charm_dtb.h
