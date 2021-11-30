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
    FILE* accelFile;
	FILE* gyroFile;
	FILE* baroFile;
    accelFile = openFile("accel_test.csv");

	int numLines = countLines(accelFile);
	numLines = numLines - 1;
	printf("%d\n", numLines);
	fclose(accelFile);

	accelFile = openFile("accel_test.csv");
	gyroFile = openFile("gyro_test.csv");
	baroFile = openFile("baro_test.csv");

	MPU9250* bigAccel = (MPU9250*)malloc(sizeof(MPU9250) * numLines);
	GYRO* gyroAll = (GYRO*)malloc(sizeof(GYRO) * numLines);
	BMP390* baro = (BMP390*)malloc(sizeof(BMP390) * numLines);

	double* time = (double*)malloc(sizeof(double) * numLines);
	double* accelX = (double*)malloc(sizeof(double) * numLines);
	double* accelY = (double*)malloc(sizeof(double) * numLines);
	double* accelZ = (double*)malloc(sizeof(double) * numLines);
	double* veloX = (double*)malloc(sizeof(double) * numLines - 1);
	double* veloY = (double*)malloc(sizeof(double) * numLines - 1);
	double* veloZ = (double*)malloc(sizeof(double) * numLines - 1);
	double* posX = (double*)malloc(sizeof(double) * numLines - 2);
	double* posY = (double*)malloc(sizeof(double) * numLines - 2);
	double* posZ = (double*)malloc(sizeof(double) * numLines - 2);

	position xyPos;

    toStructs(accelFile, gyroFile, baroFile, bigAccel, gyroAll, baro, numLines);
	toArrays(time, accelX, accelY, accelZ, bigAccel, numLines);
	plotData("X - Acceleration", "time (s)", "acceleration (m/s/s)", "xAccel.png", time, accelX, numLines);
	plotData("Y - Acceleration", "time (s)", "acceleration (m/s/s)", "yAccel.png", time, accelY, numLines);
	plotData("Z - Acceleration", "time (s)", "acceleration (m/s/s)", "zAccel.png", time, accelZ, numLines);
	//xyPos = findVelAndPos(xyPos, time, accelX, accelY, bigAccel, numLines, veloX, veloY, posX, posY);
	//printf("X - Position (m): %lf\nY - Position (m): %lf\n", xyPos.xpos, xyPos.ypos);
	//plotData("X - Velocity", "time (s)", "velocity (m/s)", "test.png", time, veloX, numLines - 2);
}
