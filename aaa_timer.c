void set_timer(int timeout_ms)
{
    unsigned long value, freq, cnt, cmp;

    value = 0;
	asm volatile("msr CNTP_CTL_EL0, %0" : : "r" (value));
    asm volatile("mrs %0, CNTFRQ_EL0" : "=r" (freq));
    asm volatile("mrs %0, CNTPCT_EL0" : "=r" (cnt));
    cmp = cnt + (freq/1000)*timeout_ms;
    asm volatile("msr CNTP_CVAL_EL0, %0" :  :"r" (cmp));
    value = 1;
    asm volatile("msr CNTP_CTL_EL0, %0" : : "r" (value));
}

void close_timer()
{
    unsigned long value;
    value = 0;
	asm volatile("msr CNTP_CTL_EL0, %0" : : "r" (value));
}