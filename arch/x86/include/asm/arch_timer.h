//Charm start: from arch/arm64/include/asm/arch_timer.h
static inline u64 arch_counter_get_cntpct(void)
{
	/*
	 * AArch64 kernel and user space mandate the use of CNTVCT.
	 */
	BUG();
	return 0;
}
//Charm end
