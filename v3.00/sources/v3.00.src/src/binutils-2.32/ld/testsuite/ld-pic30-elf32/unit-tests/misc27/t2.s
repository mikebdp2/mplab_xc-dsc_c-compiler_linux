;;
;; Testing BFINSF, BFEXTF on dsPIC33CK64MP508
;;
  .bss
foo:
  .space 2
  
  .text
  .global _F0
_F0:
  mov #0,w0
  mov #0xF,w1
  mov w0,foo
  bfins #0,#4,w1,foo
  mov foo,w0
  return

  .global _F4
_F4:
  mov #0,w0
  mov #0xF,w1
  mov w0,foo
  bfins #4,#4,w1,foo
  mov foo,w0
  return

  .global _F8
_F8:
  mov #0,w0
  mov #0xF,w1
  mov w0,foo
  bfins #8,#4,w1,foo
  mov foo,w0
  return

  .global _F12
_F12:
  mov #0,w0
  mov #0xF,w1
  mov w0,foo
  bfins #12,#4,w1,foo
  mov foo,w0
  return

  .global _nib0
_nib0:
  mov w0,foo
  bfext #0,#4,foo,w0
  return

  .global _nib4
_nib4:
  mov w0,foo
  bfext #4,#4,foo,w0
  return

  .global _nib8
_nib8:
  mov w0,foo
  bfext #8,#4,foo,w0
  return

  .global _nib12
_nib12:
  mov w0,foo
  bfext #12,#4,foo,w0
  return

