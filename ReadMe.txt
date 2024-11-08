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

This is the problem description:



Large-scale epidemic simulations have a great importance for public health. However, large-scale simulations  of realistic epidemic models require immense computing power and they are realised as parallel cloud distributed solutions.

In this assignment you implement and parallelize a highly simplified simulation model for the evolution of an infectious disease in a population.

A community of people inhabit a given area. Assume that the area is of a rectangular form and the position of each person is defined by his/her coordinates x,y.  The coordinates in this problem are discrete variables,  covering a  given fixed small area that is the area where contagion can occur. If 2 persons are in the contagion area, they are at the same coordinates.

Some persons are infected with a contagious disease. Do a simulation predicting how the community will be affected by the disease over a period of time. Use the following simplifying assumptions scenario:

Persons move around. We simulate discrete time. At every time moment, persons can make only one move (change their coordinates once). Each person has a very simplified movement pattern described by:

    Direction (N,S, E, W).  A person moves always in the same direction. If going in their moving direction the person arrives at the border of the rectangular simulation area,  the person starts “going back”  (reverses the movement direction, until the opposite border is reached).
     Amplitude: how far away is one movement (the current coordinate is incremented/decremented by the value of the amplitude).  If the amplitude of one movement spans over intermediate positions where other people are located, these people are NOT considered contacts thus they are not in danger of being infected. Only the people staying for at least one simulation time moment at the same location (the destination of the movement) can get infected.

A  Person can be: Infected (they have the disease), immune(they had the disease recently ), susceptible (they are not immune and not currently infected  – they get infected if they are in contact with an infected person). In this simplified model, you can consider that the duration of the sickness is a INFECTED_DURATION constant number of simulation time moments for every person (number of simulation time moments while a person stays infected).  After this time, the person recovers and gets immune for a  IMMUNE_DURATION constant number of time moments.

Every person has a current status and a future status. The status of a person changes at every simulation time moment. The future status can be determined by the passing of time (healing and getting immunity or losing his  immunity) or can be changed as a function of the current status of other persons at the same location (getting infected).

While (simulation time is not over)

     For all persons Update location

     For all persons compute future status

     Increment simulation time and make future status à  current

                                            

Requirements:

Read initial data:

Following parameters will be given as command line arguments:

TOTAL_SIMULATION_TIME,  InputFileName, ThreadNumber

Input files must contain:

·       Size of simulation area:  MAX_X_COORD, MAX_Y_COORD

·       Number N  of persons in the area.  For each person, on a new line:

o   PersonID

o   Initial coordinates x, y. they must be between 0..MAX_X_COORD, 0..MAX_Y_COORD

o   Initial status: (infected=0, susceptible = 1) Initially we consider that there are no immune persons. For the initially infected persons, we consider that they got infected at the moment zero of the simulation.

o   Movement pattern direction: (N=0, S=1, E=2, W=3)

o   Movement pattern amplitude: an integer number, smaller than the area dimension on the movement direction

PROVIDED INPUT FILES: see LINK HERE

Algorithms implementation:

Implement the algorithm that simulates the evolution of the epidemic both as a sequential and parallel version. The parallel version will take into account the number of threads specified as input argument.

The parallel version will use pthreads.  Divide the persons across the processing threads.  Make sure to have the needed synchronization points between threads! Before going on to update infection status, all persons must have updated locations.  Before going on to simulate next time moment,  all persons must have updated infection status.

Expected results:

The output: For each person:

     Final coordinates x, y
      Final status (infected, immune, susceptible)
     Infection counter: how many times during the simulation did the person become infected

The final output will be saved in files. If input file was  f.txt, then output files are following the name convention f_serial_out.txt and f_parallel_out.txt

Only the final result is saved – intermediate status at every simulation time moment is not saved in files.

Implement an automatic verification method to compare that the serial and parallel versions produce the same result.  

The program must provide 2 modes of running: the interactive (DEBUG) mode, when the evolution of the persons is printed after each generation, and the normal mode (without printing) for performance measurements.

Measure serial and parallel runtime and compute the speedup. The measured runtime does NOT include reading initial configuration from file and writing the final configuration in a file.

Repeat measurements for different sizes of the population, number of simulated time units, and different number of threads.

Population sizes:  10K, 20K, 50K, 100K, 500K

Simulation time: 50,  100, 150, 200, 500

 Provide a meaningful discussion of your experimental results.

Grading 

Implement serial version 5 points

Implement parallel version 10 points

A working program, reading input data in the required format  3 points

Implement comparison between serial and parallel result 0.5 point

Time Measurement, Speedup, Graphs, Discussion 1.5 point 

Your submitted code must at least compile. Code with compilation errors gets zero points.

