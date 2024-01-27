device dsPIC30F6014
set breakoptions.wdtwarnings Break
set warningmessagebreakoptions.W0004_CORE_WDT_RESET Break
hwtool sim
set uart1io.uartioenabled true
set uart1io.output file
set uart1io.outputfile "uartout.txt"
program "t.exe"
run
wait 300000
quit
