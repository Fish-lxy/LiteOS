#include "idt.h"
#include "string.h"
#include "types.h"
#include "cpu.h"
#include "console.h"
#include "debug.h"

// 中断描述符表
IDT_Entry_t idt_entries[256];

// IDTR
IDT_Ptr_t idt_ptr;

// 中断处理函数的指针数组
InterruptHandler_t interruptHandlers[256];

// 声明加载 IDTR 的函数，汇编实现(/krnl/idt_s.s)
extern void flushIDT_s(uint32_t);

// 初始化中断描述符表
void initIDT() {
	//consoleWriteColor("Init Interrupt Descriptor Table...", TC_black, TC_light_blue);
	// 重新映射 IRQ 表
    // 两片级联的 Intel 8259A 芯片
    // 主片端口 0x20 0x21
    // 从片端口 0xA0 0xA1
        
    // 初始化主片、从片
    // 0001 0001
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // 设置主片 IRQ 从 0x20(32) 号中断开始
    outb(0x21, 0x20);

    // 设置从片 IRQ 从 0x28(40) 号中断开始
    outb(0xA1, 0x28);
    
    // 设置主片 IR2 引脚连接从片
    outb(0x21, 0x04);

    // 告诉从片输出引脚和主片 IR2 号相连
    outb(0xA1, 0x02);
    
    // 设置主片和从片按照 8086 的方式工作
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    // 设置主从片允许中断
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

	bzero((uint8_t*) &interruptHandlers, sizeof(InterruptHandler_t) * 256);

	//设定IDTR
	idt_ptr.limit = sizeof(IDT_Entry_t) * 256 - 1;
	idt_ptr.base = (uint32_t) &idt_entries;

	bzero((uint8_t*) &idt_entries, sizeof(IDT_Entry_t) * 256);

	// 0-32:  用于 CPU 的中断处理
	IDT_setGate(0, (uint32_t) isr0, 0x08, 0x8E);
	IDT_setGate(1, (uint32_t) isr1, 0x08, 0x8E);
	IDT_setGate(2, (uint32_t) isr2, 0x08, 0x8E);
	IDT_setGate(3, (uint32_t) isr3, 0x08, 0x8E);
	IDT_setGate(4, (uint32_t) isr4, 0x08, 0x8E);
	IDT_setGate(5, (uint32_t) isr5, 0x08, 0x8E);
	IDT_setGate(6, (uint32_t) isr6, 0x08, 0x8E);
	IDT_setGate(7, (uint32_t) isr7, 0x08, 0x8E);
	IDT_setGate(8, (uint32_t) isr8, 0x08, 0x8E);
	IDT_setGate(9, (uint32_t) isr9, 0x08, 0x8E);
	IDT_setGate(10, (uint32_t) isr10, 0x08, 0x8E);
	IDT_setGate(11, (uint32_t) isr11, 0x08, 0x8E);
	IDT_setGate(12, (uint32_t) isr12, 0x08, 0x8E);
	IDT_setGate(13, (uint32_t) isr13, 0x08, 0x8E);
	IDT_setGate(14, (uint32_t) isr14, 0x08, 0x8E);
	IDT_setGate(15, (uint32_t) isr15, 0x08, 0x8E);
	IDT_setGate(16, (uint32_t) isr16, 0x08, 0x8E);
	IDT_setGate(17, (uint32_t) isr17, 0x08, 0x8E);
	IDT_setGate(18, (uint32_t) isr18, 0x08, 0x8E);
	IDT_setGate(19, (uint32_t) isr19, 0x08, 0x8E);
	IDT_setGate(20, (uint32_t) isr20, 0x08, 0x8E);
	IDT_setGate(21, (uint32_t) isr21, 0x08, 0x8E);
	IDT_setGate(22, (uint32_t) isr22, 0x08, 0x8E);
	IDT_setGate(23, (uint32_t) isr23, 0x08, 0x8E);
	IDT_setGate(24, (uint32_t) isr24, 0x08, 0x8E);
	IDT_setGate(25, (uint32_t) isr25, 0x08, 0x8E);
	IDT_setGate(26, (uint32_t) isr26, 0x08, 0x8E);
	IDT_setGate(27, (uint32_t) isr27, 0x08, 0x8E);
	IDT_setGate(28, (uint32_t) isr28, 0x08, 0x8E);
	IDT_setGate(29, (uint32_t) isr29, 0x08, 0x8E);
	IDT_setGate(30, (uint32_t) isr30, 0x08, 0x8E);
	IDT_setGate(31, (uint32_t) isr31, 0x08, 0x8E);

	// 31-47：用于外设的中断请求
    IDT_setGate(32, (uint32_t)irq0, 0x08, 0x8E);
    IDT_setGate(33, (uint32_t)irq1, 0x08, 0x8E);
    IDT_setGate(34, (uint32_t)irq2, 0x08, 0x8E);
    IDT_setGate(35, (uint32_t)irq3, 0x08, 0x8E);
    IDT_setGate(36, (uint32_t)irq4, 0x08, 0x8E);
    IDT_setGate(37, (uint32_t)irq5, 0x08, 0x8E);
    IDT_setGate(38, (uint32_t)irq6, 0x08, 0x8E);
    IDT_setGate(39, (uint32_t)irq7, 0x08, 0x8E);
    IDT_setGate(40, (uint32_t)irq8, 0x08, 0x8E);
    IDT_setGate(41, (uint32_t)irq9, 0x08, 0x8E);
    IDT_setGate(42, (uint32_t)irq10, 0x08, 0x8E);
    IDT_setGate(43, (uint32_t)irq11, 0x08, 0x8E);
    IDT_setGate(44, (uint32_t)irq12, 0x08, 0x8E);
    IDT_setGate(45, (uint32_t)irq13, 0x08, 0x8E);
    IDT_setGate(46, (uint32_t)irq14, 0x08, 0x8E);
    IDT_setGate(47, (uint32_t)irq15, 0x08, 0x8E);

	// 170 将来用于实现系统调用
	IDT_setGate(170, (uint32_t) isr170, 0x08, 0x8E);

	// 更新设置中断描述符表
	flushIDT_s((uint32_t) &idt_ptr);
	//consoleWriteColor("OK\n", TC_black, TC_yellow);
}

// 设置中断描述符
void IDT_setGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt_entries[num].base_low = base & 0xFFFF;
	idt_entries[num].base_high = (base >> 16) & 0xFFFF;

	idt_entries[num].seg_selector = sel;
	idt_entries[num].always0 = 0;

	// 先留下 0x60 这个魔数，以后实现用户态时候
	// 这个与运算可以设置中断门的特权级别为 3
	idt_entries[num].flags = flags;  // | 0x60
}

// 调用 ISR 中断处理函数，由汇编调用
void ISR_handlerCall(InterruptFrame_t* pr) {
	//由 InterruptFrame_t 中的中断号选择中断处理函数
	if (interruptHandlers[pr->int_no]) { 
		interruptHandlers[pr->int_no](pr);
	}
	else {
		printkColor("Interrupt unhandled! intcode : %d\n", TC_black, TC_red, pr->int_no);
		while(1){}
	}
}

// 调用 IRQ 中断服务程序，由汇编调用
void IRQ_handlerCall(InterruptFrame_t* pr){
// 发送中断结束信号给 PICs
    // 按照我们的设置，从 32 号中断起为用户自定义中断
    // 因为单片的 Intel 8259A 芯片只能处理 8 级中断
    // 故大于等于 40 的中断号是由从片处理的
    if (pr->int_no >= 40) {
        // 发送重设信号给从片
        outb(0xA0, 0x20);
    }
    // 发送重设信号给主片
    outb(0x20, 0x20);

    if (interruptHandlers[pr->int_no]) {
        interruptHandlers[pr->int_no](pr);
    }
}

// 注册一个中断处理函数
void interruptHandlerRegister(uint8_t n, InterruptHandler_t h) {
	interruptHandlers[n] = h;
}