	.file "/home/c65161/dev/builds/build_20210426/src/acme/ld/testsuite/ld-pic30-elf32/unit-tests/foo.c"
	.global	_external_array	; export
	.section	*_0x7f9a5135d0a060a82d0f,memory(_external_memory),noload
	.align	2
	.type	_external_array,@object
	.size	_external_array, 512
_external_array:
	.skip	512
	.section	.text,code
	.align	2
	.global	_main	; export
	.type	_main,@function
_main:
	.set ___PA___,1
	lnk	#0
	ulnk	
	return	
	.set ___PA___,0
	.size	_main, .-_main



	.section __c30_info, info, bss
__large_data_scalar:

	.section __c30_signature, info, data
	.word 0x0001
	.word 0x0000
	.word 0x0000

	.memory _external_memory, size(1024), origin(0)
; MCHP configuration words

	.set ___PA___,0
	.end
