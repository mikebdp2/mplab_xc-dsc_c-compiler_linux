device PIC24FJ256DA210
hwtool sim
set uart1io.uartioenabled true
set uart1io.output file
set uart1io.outputfile "UartOut.txt"
program "t.exe"
run
wait 300000
quit
