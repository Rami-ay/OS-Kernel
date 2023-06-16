Programs Overview:
===================
=============================================
!! This program runs in Windows Enviroment !!
=============================================

ex1ba1.c:
----------
this program is the father program.
it gets as an input a name of EXISTING AND EMPTY input file and int for srand
50 times , the father fork 2 children , the first which build an array and
fill it randomly and sort it using bubble sort algorithm.
the second do the same but using a quick sort algo.
when the children is done, the father gather the stats from the input file.
show his stats on screen and remove the input file when done.

example of the stats : 
	<Average_bubbleSort_time> <Average_quickSort_time>
	<Minimum_bubbleSort_time> <Minimum_quickSort_time>
	<Maximum_bubbleSort_time> <Maximum_quickSort_time>
	<Father_Total_Running_time>

ex1ba2.c:
----------
this program is the first child, gets as input a file name and int for srand.
declare an array of size 50,000 and fill it randomly and sort it using bubble sort algorithm.

example of the stats : b <running time> 
explaining : the b indicates that the child sorted using bubble sort.

ex1ba3.c:
----------
this program is the second child, gets as input a file name and int for srand.
declare an array of size 50,000 and fill it randomly and sort it using quick sort algorithm.

example of the stats : q <running time> 
explaining : the q indicates that the child sorted using quick sort.


Compiling And Running:
======================
Compiling:
-----------
Since this is a Windows enviroment program, you can compile using windows IDE such as VS,
or using POSIX-compatible programming and runtime environment such as Cygwin.
and run it using cmd ,

!!MAKE SURE TO HAVE THE .exe FILES AND THE INPUT FILE IN SAME FOLDER!!

Assuming you complied using the VS all three programs ( make sure to have the following names: [ex1ba1.exe,ex1ba2.exe,ex1ba3.exe] )

using POSIX-compatible programming and runtime environment :
-------------------------------------------------------------
gcc -Wall ex1ba1.c -o ex1ba1
gcc -Wall ex1ba2.c -o ex1ba2
gcc -Wall ex1ba3.c -o ex1ba3

*Note: after running the 3 commands mentioned above you will get .exe executable files

Running:
---------
using cmd - Windows Command Prompt:
ex1ba1.exe <input_file> <number>
