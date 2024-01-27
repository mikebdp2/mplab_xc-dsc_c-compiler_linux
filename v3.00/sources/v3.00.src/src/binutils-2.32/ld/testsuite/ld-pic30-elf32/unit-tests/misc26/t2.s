;;
;; Testing BFINS, BFEXT on dsPIC33CK64MP508
;;
  .global _F0
_F0:
  mov #0,w0
  bfins #0,#4,#0xF,w0
  return

  .global _F4
_F4:
  mov #0,w0
  bfins #4,#4,#0xF,w0
  return

  .global _F8
_F8:
  mov #0,w0
  bfins #8,#4,#0xF,w0
  return

  .global _F12
_F12:
  mov #0,w0
  bfins #12,#4,#0xF,w0
  return

  .global _nib0
_nib0:
  bfext #0,#4,w0,w0
  return

  .global _nib4
_nib4:
  bfext #4,#4,w0,w0
  return

  .global _nib8
_nib8:
  bfext #8,#4,w0,w0
  return

  .global _nib12
_nib12:
  bfext #12,#4,w0,w0
  return

