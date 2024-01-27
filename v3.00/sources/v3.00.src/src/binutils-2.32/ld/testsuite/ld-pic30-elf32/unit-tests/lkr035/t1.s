        ;;
        ;;  
        ;; 
        .text
        .extern _data_start
        .extern _bss_start
        .global __reset
__reset:        
        .pword 1,2,3,4

        .bss
        .space 0x8

		.section .foo,"x"
		.pword 5,6,7,8

	    .data
        .space 0x8       
        .word _data_start
        .word _bss_start
        
        
        
        
        
