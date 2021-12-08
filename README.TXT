
Name: Brandon Lindner, Fall 2021
Section: T1B-T2B
Project: Final Project - Dead Reckoning
Description: PARSE FLIGHT DATA FILES FROM A ROCKET/DRONE AND FIND/CALCULATE ESSENTIAL DATA AND GRAPH IT
Documentation Statement: None
==============================================================
UserManual/Instructions:
   Replace this paragraph with instructions on how to build, run,
   and use your program if different than the typical process.  
   If your program has "built-in" instructions or a tutorial you 
   should state that here and forgo a long description.

==============================================================
Required Programming Skills (must have 5 of 6):
  -- Leave blank if the requirement isn't one that you implemented.

   1) Dynamic Memory Allocation
          What you did to meet this req: Dynamically allocated all arrays used
              File: final_main.c
		    final_functs.c
              Line#: 75-85
		     152-157, 243-247

   2) Pointers
          What you did to meet this req: Pointed to arrays and string pointers
              File: final_main.c
              Line#: 106-107 (all arrays passed to functions)

   3) File I/O
          What you did to meet this req: All data came from 3 different files
              File: final_functs.c
		    final_main.c
              Line#: 97-105
		     42, 48, 54, 70-72
   4) Structs
          What you did to meet this req: Created structs for the readings of each of the sensors
              File: final_functs.h
		    final_functs.c
              Line#: 13-35
		     too many lines to count in .c

   5) String Manipulation
          What you did to meet this req: Concatinated strings to print the grid square
              File: final_functs.c
              Line#: 298, 305-206

   6) Processing of Array Data
          What you did to meet this req: Stored all data within arrays and accessed when calculating
              File: final_functs.c
              Line#: toArray function and findVelAndPos

  Required Advanced Programming Skills (1 of the below):
   1) Recursion
          What you did to meet this req: xxx
              File: (file where example is located)
              Line#: (line in file where it is located)

   2) 2D Dynamic Memory
          What you did to meet this req: used to store arrays of x, y, and z acceleration
              File: final_main.c
              Line#: 84-85


==============================================================
List the requirements from your design document (indicate any changes
or omissions from stated shall requirements from initial design) 
also describe any functionality that isn't working.  You must also
list the percentage that was allocated from the design.  If you
are proposing a change include that as well as a justification
for the change in percentage.

      Requirements from Design:
      -- Indicate changes to requirment and/or percentage with
         an asterisk.  Put errors or nonfuctional implementation
         below the requirment it applies to.

        SHALL 1. Prompt the user for the acceleration, barometer, and gyroscope data files (5%)
	SHALL 2. Read the files inputted by the user (5%)
	SHALL 3. Put the data from the files into arrays for printing to the graphs (7%)
	SHALL 4. Put the data from the files into structs with each reading combined with the other readings from that same time (10%)
	MAY 5. Implement a Kalman filter of some kind to smooth the data inconstancies and noise (15%)
	SHALL 6. Read the gyroscopic and lateral acceleration data and find the position of the rocket based on that (30%)
	SHALL 7. Take the position of the rocket after landing and output the appropriate grid square it would be located in (13%)
	SHALL 8. Graph the data from the files with pbplot (10%)
	SHALL 9. Output the grid square, and distance from the launch pad (5%)

	COMPLETED ALL BUT THE KALMAN FILTER WHICH WAS A "MAY"


==============================================================
  How many hours did you spend on this project: 35 hours
  How many lines of code did you write in total: 546