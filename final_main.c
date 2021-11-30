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
#include <unistd.h>
#include "final_functs.h"

int main(){
    FILE* test;
    test = openFile("latAccel2.csv");

	int numLines = countLines(test);
	printf("%d\n", numLines);
	fclose(test);
	test = openFile("latAccel2.csv");
	MPU9250* accel = (MPU9250*)malloc(sizeof(MPU9250) * numLines);

	double* time = (double*)malloc(sizeof(double) * numLines);
	double* accelX = (double*)malloc(sizeof(double) * numLines);
	double* accelY = (double*)malloc(sizeof(double) * numLines);
	double* veloX = (double*)malloc(sizeof(double) * numLines - 1);
	double* veloY = (double*)malloc(sizeof(double) * numLines - 1);
	double* posX = (double*)malloc(sizeof(double) * numLines - 2);
	double* posY = (double*)malloc(sizeof(double) * numLines - 2);

	position xyPos;

    toAccelStruct(test, accel, numLines);
	toArrays(time, accelX, accelY, accel, numLines);
	plotData("X - Acceleration", "time (s)", "acceleration (m/s/s)", "xAccel.png", time, accelX, numLines);
	plotData("Y - Acceleration", "time (s)", "acceleration (m/s/s)", "yAccel.png", time, accelY, numLines);
	xyPos = findVelAndPos(xyPos, time, accelX, accelY, accel, numLines, veloX, veloY, posX, posY);
	printf("X - Position (m): %lf\nY - Position (m): %lf\n", xyPos.xpos, xyPos.ypos);
	plotData("X - Velocity", "time (s)", "velocity (m/s)", "test.png", time, veloX, numLines - 2);
}
