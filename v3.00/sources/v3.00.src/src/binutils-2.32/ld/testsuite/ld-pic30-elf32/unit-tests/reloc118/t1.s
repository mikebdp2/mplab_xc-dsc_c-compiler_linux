	.section	.nbss,bss,near
	.align	4
_buf:	.space	2048

	.section	.text,code
      .global __reset
__reset:

	mov.w	#_buf,w0
	mov.w	#_buf+8,w0
	mov.w	#_buf-8,w0
	
	mov.l	#_buf,w0
	mov.l	#_buf+8,w0
	mov.l	#_buf-8,w0
	
	mov.w #addr_lo(_buf),w0
	mov.w #addr_hi(_buf),w0
	mov.w #addr_lo(_buf+8),w0
	mov.w #addr_hi(_buf+8),w0
	mov.w #addr_lo(_buf-8),w0
	mov.w #addr_hi(_buf-8),w0
	bra __reset
	
