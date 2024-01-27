	.text
	.global __reset
__reset:
	.pbyte b1,b2,b3,b4

	.section foo,"r"
	.byte b1,b2,b3,b4

	.section dat,"d"
	.byte b1,b2,b3,b4

