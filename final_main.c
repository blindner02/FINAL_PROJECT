/** final_main.c
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT DATA FILES FROM A ROCKET AND FIND ESSENTIAL DATA AND GRAPH IT
 * Documentation: NONE
 * =========================================================== */ 

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "final_functs.h"

int main(){
    FILE* test;
    test = openFile("latAccel2.csv");

	int numLines = countLines(test);
	printf("%d\n", numLines);
	fclose(test);
	test = openFile("latAccel2.csv");
	MPU9250* accel = (MPU9250*)malloc(sizeof(MPU9250) * numLines);

	double time[numLines];
	double accelX[numLines];
	double accelY[numLines];

    toAccelStruct(test, accel, numLines);
	toArrays(time, accelX, accelY, accel, numLines);
	plotData("X - Acceleration", "time (s)", "acceleration (m/s/s)", "xAccel.png", time, accelX, numLines);
}
