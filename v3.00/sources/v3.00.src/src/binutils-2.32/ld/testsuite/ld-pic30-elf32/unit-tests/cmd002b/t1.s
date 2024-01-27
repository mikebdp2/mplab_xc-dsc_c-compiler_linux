        .text
        .global __reset
__reset:        
        .pword 1,2,3,4
        .bss
        .space 0x20
        
        .section userheap,heap
        .space 0x100
        .ifdef HEAP_ERROR
        .space 0x100
        .endif
        
        
        
