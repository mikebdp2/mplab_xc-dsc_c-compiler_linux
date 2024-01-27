device dsPIC33CK256MP508
hwtool sim
set uart1io.uartioenabled true
set uart1io.output file
set uart1io.outputfile "UartOut.txt"
program "t.exe"
run
wait 300000
quit
