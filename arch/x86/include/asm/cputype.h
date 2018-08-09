//Charm start: from arch/arm64/include/asm/cputype.h
#ifndef __ASM_CPUTYPE_H
#define __ASM_CPUTYPE_H

#define INVALID_HWID		ULONG_MAX

#define MPIDR_LEVEL_BITS_SHIFT	3
#define MPIDR_LEVEL_BITS	(1 << MPIDR_LEVEL_BITS_SHIFT)
#define MPIDR_LEVEL_MASK	((1 << MPIDR_LEVEL_BITS) - 1)

#define MPIDR_LEVEL_SHIFT(level) \
	(((1 << level) >> 1) << MPIDR_LEVEL_BITS_SHIFT)

#define MPIDR_AFFINITY_LEVEL(mpidr, level) \
	((mpidr >> MPIDR_LEVEL_SHIFT(level)) & MPIDR_LEVEL_MASK)
#endif
//Charm end
