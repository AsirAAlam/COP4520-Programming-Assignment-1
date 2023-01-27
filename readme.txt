Name: Asir Alam
COP4520 Programming Assignment 1

Instructions:
- Compile: g++ primes.cpp -pthread
- Execute: ./a.out

The program uses an O(sqrt(n)) algorithm for testing the primality of each integer. To check the
primality of some integer n, it checks integers upto sqrt(n) for divisibility since any existing
factors would be found by then. Furthermore, it checks for divisibility by 2 and 3 beforehand so it
can skip numbers for a notable constant factor improvement. 

Since each integer can be checked for primality independently of the other integers, this process 
can be done concurrently. The distribution works as follows to ensure equal workload. Each thread 
t_i loops over integers starting at 2 * i + 1 with a step size of 16. For example, thread t_1 loops 
over integers 1, 17, 33, etc. This is a better way of splitting the workload compared to
8 contiguous regions since it ensures that each thread has equally frequent encounters of numbers
of all magnitudes. The program considers all odd numbers, so no numbers are being skipped. The 
prime number 2 is hard coded since none of the threads test even numbers. 

Experimental evaluation was done as follows. I timed each thread separately and observed the order 
in which the threads finished their work. The first thread and last thread only have an execution 
time difference of a couple hundred milliseconds. The orderings of the threads finishing varies 
between each execution, which indicates that no thread is consistently doing more work than another 
and the workload has been split evenly. Furthermore, I timed the program running on different 
numbers of threads by changing the "threads" integer variable. I started with 1 thread and doubled 
the count repeatedly until 8. The recorded execution times given in order were approximately 21s, 
12s, 6s, and 4s for 1 thread, 2 threads, 4 threads, and 8 threads, respectively. The times 
approximately cut in half upon doubling thread count and show signs of Amdahl's law in effect. 
