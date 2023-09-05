.global _Reset
_Reset:

ldr x1,=arm_vector_table
msr vbar_el1,x1
msr vbar_el2,x1
adrp x19, stack_top_el2
mov sp,x19
MSR SPSEL, #1                                   // switch to SP_ELn
ldr x6,=_el1_enter
msr elr_el2,x6                                  // 设置el2的异常返回地址为_el1_enter
MOV   X0, XZR
ORR   X0, X0, #(7 << 6)                         // 禁止IRQ,FIQ,SError位
bic   X0, X0, #(1 << 4)                         // M[4]=0代表为AArch64
ORR   X0, X0, #(1 << 2)                         
ORR   X0, X0, #(1 << 0)                         // M[0：3] = 0b0101，表示挑选栈顶指针为EL1h，即选择了SP_EL1
MSR   SPSR_EL2, X0  
mov   x0,xzr
orr   x0,x0,#(1<<31)
MSR   hCR_EL2, X0                               // hcr的bit31为RW位，对较低异常级别的执行状态控制。0：低层的特权等级全部都是AArch32；1：低一层的特权等级是 AArch64
mrs x5,sctlr_el1
orr x5,x5,#(1<<9)                               // enable el0 daifconfig
msr sctlr_el1,x5                                // EL0可以访问cpu状态寄存器的PSTATE.{D,A, I, F}比特
eret


.global _el1_enter
_el1_enter:

adrp x19, stack_top_el1
mov sp,x19                  // 设置栈顶指针为el1的栈顶指针地址
msr daifclr, #1             // f位置0->FIQ异常不会被屏蔽
msr daifclr, #2             // i位置0->IRQ异常不会被屏蔽
msr daifclr, #4             // a位置0->SError异常不会被屏蔽
msr daifclr, #8             // d位置0->当前异常级别的观察点、断点和软件步骤异常不会被屏蔽。
BL el1_entry                // el1_entry中清零了bss段，初始化了gic时钟
ldr x6,=_el0_enter
msr elr_el1,x6              // 要从el1返回时的地址为_el0_enter
MOV   X0, XZR
bic   X0, X0, #(1 << 4)
MSR   SPSR_EL1, X0          // 存入bit4为0的数，表明运行在AArch64执行状态，M[0：3] = 0b0，表示挑选栈顶指针为EL0t，即选择了SP_EL0
eret

.global _el0_enter
_el0_enter:
adrp x19, stack_top_el0
mov sp,x19                  // 设置栈指针指向el0的栈顶
bl el0_entry                // el0_entry中ivc通信
b .

.global get_reg
get_reg:   
mrs x0,currentel 
ret

.global get_spsel
get_spsel:
mrs x0,spsel 
ret