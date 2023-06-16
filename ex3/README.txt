Programs Overview:
===================
ex3a:
----
the program gets as input an int number to
pass to srand function,
the father declares an array of size 1000 , and initiat it sorted in the following manner:
the first element in the array will have random number in range [0,9] and from the second element forward the elemnt equals the value of previouse element + rand number in range [0,9]

the father forks two children, the first will will pick random number between [0,1M) and search it linearly million times.
the second child will do the same but uses the binary search.

in the end the children will write their stats on STDOUT using pipe.
the output will look like this in the end of the run
<s\b> <number_of_values_found> <execution_time>
.
.
x20
.
.
.
<avg_linear_search_time> <avg_binary_search_time>
<father_execution_time>

Compiling And Running:
======================
compiling:
gcc -Wall ex3a.c -o ex3a
running:
./ex0a <int>


NOTE: average time for ex1a.c is : 214.99 s
	  average time for ex3a.c is : 2179.88 s
============================================== 000 ==========================================
ex3b:
----
in this program the father process initiate an array of size ARR_SIZE
the father want with the help of NUM_OF_CHILDREN processes to fill the array with
prime numbers, in a way that the array is sorted from small number to large numbers(left to right)

the father forks NUM_OF_CHILDREN processes each child will gamble a random number between [0,1M) and check if it 
is a prime number, if so the child will send the prime number along with his process id to the father using general pipe
for all the children. 
the father will read these numbers from the general pipe and check if the gotten prime number is larger or equal to the maximum
number in the array, if so the father will add the prime number to the array and send 1 to the son using dedicated pipe for the relevant son.
otherwise the father won't add the prime number and write 0 to the dedicated pipe.

when the array is full the father will print the the following stats on the screen:
the number of the prime numbers in the array.
the maximum prime number in the array.
the minimum prime number in the array.

and then each child will print on the screen how many prime numbers they gambled and were added to the array.



KNOWN BUGS: did not succeed to comunicate between the father and the sons on the dedicated pipes.
			the program fails with certain amount of ARR_SIZE , it will exit(1).
			


Compiling And Running:
======================
ex3b:
------------
compiling:
gcc -Wall ex3b.c -o ex3b

running:
./ex3b