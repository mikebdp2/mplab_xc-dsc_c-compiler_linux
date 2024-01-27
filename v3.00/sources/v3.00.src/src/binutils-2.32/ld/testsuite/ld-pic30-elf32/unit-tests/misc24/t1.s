        ;;
        ;;
        
        .equ COUNT,4
        
        .global LC0,LC1 
       	.section	.const,"r"
LC0:
  .ascii "this line should appear 12 times\12\0"
	
  .data
_line:
  .word	LC0
        
  .text
  .global	_main	; export
_main:
  lnk	#0


  do  #3,loop
  mov _line,w0
  call _print_string
loop:

  nop
  do  #(COUNT-1),loop2
  mov _line,w0
  call _print_string
loop2:

  nop
  mov #3,w2
  do  w2,loop3
  mov _line,w0
  call _print_string
loop3:

  nop
  ulnk
  return

	.end
	
