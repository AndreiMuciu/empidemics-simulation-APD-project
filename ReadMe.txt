Here are some measurements:
10k persons && 50 Time simulation: time serial - 5,15; time parallel - 1,64; speedup - 3,12
10k persons && 100 Time simulation: time serial - 9,04; time parallel - 3,28; speedup - 2,75
10k persons && 150 Time simulation: time serial - 13,32; time parallel - 4,74; speedup - 2,8
10k persons && 200 Time simulation: time serial - 17,78; time parallel - 6,22; speedup - 2.85
10k persons && 500 Time simulation: time serial - 43,52; time parallel - 15,4; speedup - 2,82
20k persons && 50 Time simulation: time serial - 19,43; time parallel - 6,99; speedup - 2,77
20k persons && 100 Time simulation: time serial - 41,17; time parallel - 13,41; speedup - 3,06
20k persons && 150 Time simulation: time serial - 56,61; time parallel - 19,94; speedup - 2,83
20k persons && 200 Time simulation: time serial - 76,96; time parallel - 26,08; speedup - 2,95
20k persons && 500 Time simulation: time serial - 192,35; time parallel - 64,95; speedup - 2,96
50k persons && 50 Time simulation: time serial - 102,19; time parallel - 34,66; speedup - 2,94
50k persons && 100 Time simulation: time serial - 197,64; time parallel - 68,11; speedup - 2,9
50k persons && 150 Time simulation: time serial - 289,75; time parallel - 98,37; speedup - 2,94
50k persons && 200 Time simulation: time serial - 381,65; time parallel - 126,79; speedup - 3,01
50k persons && 500 Time simulation: time serial - 962,5; time parallel - 330,75; speedup - 2,91

I created the sequential logic in the first phase, and then based on that logic described in 
the statement I transformed the sequentiality into parallel logic, based on the measurements 
the speedup value obtained is generally between 2.8 and 3, so the improvement is a very good one 
for the parallel part . the program is called in the command line as follows: 
./executable_name TOTAL_SIMULATION_TIME INPUT_FILE_NAME NUMBER_OF_THREADS. the program checks 
if the number of threads is divisible by the number of people taken from the input file, 
also if the number of arguments in the command line is not correct, the program stops. 
for the sequential part I had initially created several variables in the hand, 
then to be able to adapt to the parallel I created other local variables with the values 
​​of the ones in the hand to be able to do the parallel logic. the logic followed in the algorithm 
is exactly the same in sequential and parallel. it can be seen that for small data there is no 
point in paralyzing such as a number of 10 people, however, for a larger number, 
the parallel version feels good and helps a lot.

