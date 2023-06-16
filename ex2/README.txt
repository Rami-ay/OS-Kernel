Programs Overview:
===================

ex2a.c:
----------
this program runs in a loop 10 times
in every iteration it informs the user that it ready
wait to an input key and Enter pressed
then gives the user one second to press a natural number > 0.
after the 10 iterations the program shows the user how many numbers he
pressed and the maximum value of all the pressed numbers:

ex2b.c:
----------
in this program we fork a child process,
both the parent and the child process sleep for random time (2 secs max)
gambles number in range [0,100] the process who gets 1000 numbers greater than 95 first wins
the process that gets 60,000 numbers smaller than 55 first loses

Possible Running Situations:
1) infinite loop , it may happen that both processes run infintly,
in this case nither the son nor the father gets numbers that are >= 95 or 
smaller than 55.

2)we may get that the father process failed to fork/ kill returned error,
and the program will exit with failure.


Compiling And Running:
======================
Compiling ex2a.c:
------------------
gcc -Wall ex2a.c -o ex2a


Compiling ex2b.c:
------------------
gcc -Wall ex2b.c -o ex2b

Running:
---------
./ex2a
./ex2b

