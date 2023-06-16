Programs Overview:
===================
ex5a:
----
in this program the father process initiate an array of size ARR_SIZE
the father want with the help of NUM_OF_GEN processes to fill the array with
prime numbers, in a way that the array is sorted from small number to large numbers(left to right)

the father forks NUM_OF_CHILDREN processes each child will gamble a random number between [0,1M) and check if it 
is a prime number, if so the child will open shared memory by inserting 1 to sharedmemory[4] and add it if it's larger than the maximum prime number in the shared memory and close the shared memory after that by inserting 0 to sharedmemory[4] 
the father will go sleep until he gets a signal, and when he gets a signal he displays how many prime numbers in the array , the maximum and minimum number and closes the shared memory.
and then each child will print on the screen how many prime numbers they gambled and were added to the array.

Compiling And Running:
======================
ex5a:
------------
compiling:
gcc -Wall ex5a1.c -o ex5a1
gcc -Wall ex5a2.c -o ex5a2

running:
./ex5a1 
======================================================================================================
ex5b:
----
this program contain two servers : prime server and palindrome server.

prime server:
--------------
this server create a shared memory size of 102 ints which clienets can reach it with.
to the shared_memory[0] he enters his process id and in shared_memory[1] the client enters his process id.
the server runs in an infinite loop in each iteration he run over the shared memory and overwrite the non prime numbers with -1 when finish he send sigusr1 signal to the client.


palindrome server:
------------------
this server create a shared memory size of 22 int which clients can reach it with.
the server runs in an infinite loop in each iteration check if the array from index 2 to index 20 make a symetric number.if the array is symtric he puts the value 1 in shared_memory[21] and 0 otherwise and sends sigusr2 signal to the client.


in addition to these two servers there is the FRONT-END program which is the interface that 
the user uses to use the servers mentioned above.

the front-end program gets from the user the desired action : q-palindrome server , p - prime server.

if the useres chooses the prime server, the front-end program reads <= 100 ints until 0.
send it to the prime server and print the result on the screen.

if the user chooses the palindrome server, the front-end program reads <= 19 ints from the user and sends it
to the palindrome server.
if the array is palindrome it print "the array is symtric" on the screen.
if the array is not a palindrome it prints "the array is Not symtric" on the screen.


 Compiling And Running:
======================
ex5b:
------------
compiling:
gcc -Wall ex5b1.c -o ex5b1
gcc -Wall ex5b2.c -o ex5b2
gcc -Wall ex5b3.c -o ex5b3

running:
./ex5b3

==============================
KNOWN BUGS ON ex4c:
==============================
in ex5b : the program shoul exit when it gets SIGINT signal, i tried manually using the terminal to press Ctrl+C but it appears that the program do not responde to the command in shell.

	

Author:Rami Ayoub
ID: 315478966












