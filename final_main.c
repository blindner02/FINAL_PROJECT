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
	MPU9250* accel = (MPU9250*)malloc(sizeof(MPU9250) * numLines);
    toAccelStruct(test, accel, numLines);
	plotData(&accel->time, &accel->accelX, numLines);
}
