
  ;; basic code alignment test
  ;;  for dsPIC33A
  .text
  mov w1,w2
  .align 4
here:
  add w1,w2,w3  ; with .align
  mov w1,w2
  add w1,w2,w3  ; without .align
  .align 8
  add w1,w2,w3  ; no padding needed
  
  
  ;; test alignment of data directives
  ;;  in code sections
  .text
  .word 0x2211
  .align 4
there:
  .long 0x44332211
  .word 0x2211
  .align 8
  .long 0x44332211
  
  
