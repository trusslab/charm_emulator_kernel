//Charm start: from arch/arm64/include/asm/smp_plat.h
/*
 * Logical CPU mapping.
 */
extern u64 __cpu_logical_map[NR_CPUS];
#define cpu_logical_map(cpu)    __cpu_logical_map[cpu]
//Charm end
