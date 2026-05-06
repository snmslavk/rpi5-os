.section ".text.boot"
.globl _start

_start:
    // Read CPU ID
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    
    // Only CPU 0 continues, others enter WFE loop
    cbz x0, setup_stack
    b halt_cpu

setup_stack:
    // Set stack pointer for CPU 0
    ldr x0, =_stack_end
    mov sp, x0

    // Clear BSS section
    ldr x0, =__bss_start
    ldr x1, =__bss_end
    sub x1, x1, x0
    cbz x1, skip_bss_clear

clear_bss:
    str xzr, [x0], #8
    subs x1, x1, #8
    b.gt clear_bss

skip_bss_clear:
    // Jump to kernel main
    bl main
    b halt

halt_cpu:
    wfe
    b halt_cpu

halt:
    wfe
    b halt