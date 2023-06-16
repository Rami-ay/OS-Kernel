Programs Overview:
===================
ex6a:
----
in this program the father process initiate an array of size ARR_SIZE
the father want with the help of NUM_OF_GEN processes to fill the array with
prime numbers, in a way that the array is sorted from small number to large numbers(left to right)

the father forks NUM_OF_CHILDREN processes each child will gamble a random number between [0,1M) and check if it 
is a prime number, if so the client(forked child) will send to the server(filler-father) by writing the prime to a socket that the server opens and add it if it's larger than the maximum prime number, if the prime number is added the server send to the client 1 on the shared socket.

after the server is done filling the array the father will send -1 via socket for the client indicating it's done.
the server prints it's stats and the clients as well.

Compiling And Running:
======================
ex6a:
------------
compiling:
gcc -Wall ex6a1.c -o ex6a1
gcc -Wall ex6a1.c -o ex6a2

running:
./ex6a1 
======================================================================================================
ex6b:
----
this program contain two servers : prime server and palindrome server.

prime server:
--------------
this server create a socket which clienets can reach it with.
the server runs in an infinite loop in each iteration he run over the socket and find if there is any prime numbers if so the server writes to the clinet the prime numbers he found if none is found he writes -1.

palindrome server:
------------------
this server create a socket which clients can reach it with.
the server runs in an infinite loop in each iteration check if the array he read from the socket that the client wrote make a symetric number.if the array is symtric he puts the value 1 in socket and 0 otherwise.


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
ex6b:
------------
compiling:
gcc -Wall ex5b1.c -o ex5b1
gcc -Wall ex5b2.c -o ex5b2
gcc -Wall ex5b3.c -o ex5b3

running:
./ex6b3

==============================
KNOWN BUGS ON ex4c:
==============================
in ex6b: only the prime server and the palindrome server were wrote, 
therefore the program does not run as expected!

	

Author:Rami Ayoub
ID: 315478966












