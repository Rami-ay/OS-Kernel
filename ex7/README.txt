Programs Overview:
===================
ex7a:
----
in this program the main process initiate an global array of size=101 the first element in the array will act as a lock,the main
process fills the array with zeros then creates 3 threads that responsable of gambling a prime number and it in the array in such manner
that the values in the arrays are sorted from the small(left) to the bigger(right) prime numbers.
after creating the threads the main process waits untill there are done and prints the arrays stats.
each thread in an infinite loop gambles prime number and tries to add it to the array and count how much prime he had enter to the array.
in the end of the mentioned process which means that the array is full (or failed to add a prime for 100 times) the thread prints the number 
of prime he had added on the screen and exit.
 

Compiling And Running:
======================
ex7a:
------------
compiling:
gcc -Wall -lpthread ex7a.c -o ex7a
running:
./ex7a 

race cases: it may happen that the threads race to unlock the array and and add primes.
======================================================================================================
ex7b:
----
this program contain two servers : prime server and palindrome server.

prime server:

--------------
this server is a thread,
that runs in an infinite loop in each iteration he run over an input array which is globaly defined.the array contains number in each
cell, if the value in the cell is prime the thread writes 1 in the output array in the same exact index as the values exists in the input
array, 0 otherwise.

palindrome server:
------------------
this server is a thread,
that runs on an array (which is globaly defined) if the array from array[1:99] if the number in the array is palindrome it writes
1 in array[0], 0 otherwise.

in addition to these two servers there is the FRONT-END thread which is the interface that 
the user uses to use the servers mentioned above.

the front-end program gets from the user the desired action : q-palindrome server , p - prime server.

if the useres chooses the prime server, the front-end program reads <= 100 ints until 0.
send it to the prime server and print the result on the screen.

if the user chooses the palindrome server, the front-end program reads <= 100 ints from the user and sends it
to the palindrome server.
if the array is palindrome it print "the array is symtric" on the screen.
if the array is not a palindrome it prints "the array is Not symtric" on the screen.


 Compiling And Running:
======================
ex7b:
------------
compiling:
gcc -Wall -lpthread ex7b.c -o ex7b

running:
./ex7b

race cases: I believe there is no thread race cases since every thread wait for signal from the front end and vice versa.
	

Author:Rami Ayoub
ID: 315478966












