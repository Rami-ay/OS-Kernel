Programs Overview:
===================
ex4a:
----
in this program the father process initiate an array of size ARR_SIZE
the father want with the help of NUM_OF_GEN processes to fill the array with
prime numbers, in a way that the array is sorted from small number to large numbers(left to right)

the father forks NUM_OF_CHILDREN processes each child will gamble a random number between [0,1M) and check if it 
is a prime number, if so the child will send the prime number along with his process id to the father using general named pipe(fifo)
for all the children. 
the father will read these numbers from the general pipe and check if the gotten prime number is larger or equal to the maximum
number in the array, if so the father will add the prime number to the array and send 1 to the son using dedicated named pipe for the relevant son.
otherwise the father won't add the prime number and write 0 to the dedicated pipe.

when the array is full the father will print the the following stats on the screen:
the number of the prime numbers in the array.
the maximum prime number in the array.
the minimum prime number in the array.

and then each child will print on the screen how many prime numbers they gambled and were added to the array.

			


Compiling And Running:
======================
ex4a:
------------
compiling:
gcc -Wall ex4a1.c -o ex4a1
gcc -Wall ex4a2.c -o ex4a2
mkfifo fifo0
mkfifo fifo1
mkfifo fifo2
mkfifo fifo3

running:
./ex4a1 fifo0 fifo1 fifo2 fifo3

======================================================================================================
ex4b:
----
the program do the same tasks as ex4a but instead of using 4 fifos, it uses one queue,
that the filler and the generators read an write using different message types:

message_type=1 : filler-generator1 
message_type=2 : filler-generator2
message_type=3 : filler-generator3
message_type=4 : filler-ALL generators

 Compiling And Running:
======================
ex4b:
------------
compiling:
gcc -Wall ex4b1.c -o ex4b1
gcc -Wall ex4b2.c -o ex4b2

running:
./ex4b1

======================================================================================================
ex4c:
----
this program contain two servers : prime server and palindrome server.

prime server:
--------------
this server create a queue which clienets can reach it with.
the server runs in an infinite loop in each iteration reads a message from the queue.
the message contains numbers, he run over these numbers and locate the primes of them,
gathering them in one string and writes it back to the client using the mentioned queue.


palindrome server:
------------------
the server creates a queue which clients can reach it with.
the server runs in an infinite liip in each iteration reads a message from the queue.
the message contains a string, the server check if the string is a palindrome and write 
back to the queue the relevant message
"yes" if the string is palindrome.
"no" if the string is NOT a palindrome.


in addition to these two servers there is the FRONT-END program which is the interface that 
the user uses to use the servers mentioned above.

the front-end program the desired action the user want to execute: q-palindrome server , p - prime server.

if the useres chooses the prime server, the front-end program reads a string of numbers until 0.
send it to the prime server and print the result on the screen.

if the user chooses the palindrome server, the front-end program reads a string from the user and sends it
to the palindrome server.
if the string is palindrome it print + on the screen.
if the string is not a palindrome it prints - on the screen.


 Compiling And Running:
======================
ex4b:
------------
compiling:
gcc -Wall ex4c1.c -o ex4c1
gcc -Wall ex4c2.c -o ex4c2
gcc -Wall ex4c3.c -o ex4c3

running:
./ex4c3

==============================
KNOWN BUGS ON ex4c:
==============================
- the program ran with me only with debug mode.
- the is no handling to the SIGINT signal,
	therfore the queues are not freed, and needs to be freed manually using 
	ipcrm command on the shell.
	

Author:Rami Ayoub
ID: 315478966












