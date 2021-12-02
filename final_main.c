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

	char inAccel[MAX_FILE_NAME_LENGTH];
	char inGyro[MAX_FILE_NAME_LENGTH];
	char inBaro[MAX_FILE_NAME_LENGTH];
	
	printf("Enter the name of a properly formatted acceleration data .csv : ");
	scanf("%s", inAccel);
	printf("Enter the name of a properly formatted gyroscopic data .csv : ");
	scanf("%s", inGyro);
	printf("Enter the name of a properly formatted barometric pressure .csv : ");
	scanf("%s", inBaro);
	/*
	char* inAccel = "accel_test.csv";
	char* inGyro = "gyro_test.csv";
	char* inBaro = "baro_test.csv";
	*/
    accelFile = openFile(inAccel);

	int numLines = countLines(accelFile);
	// int numLines = 100000;
	numLines = numLines - 1;
	printf("Number of items: %d\n", numLines);
	fclose(accelFile);

	

	accelFile = openFile(inAccel);
	gyroFile = openFile(inGyro);
	baroFile = openFile(inBaro);


	MPU9250* bigAccel = (MPU9250*)malloc(sizeof(MPU9250) * numLines);
	GYRO* gyroAll = (GYRO*)malloc(sizeof(GYRO) * numLines);
	BMP390* baro = (BMP390*)malloc(sizeof(BMP390) * numLines);

	double* time = (double*)malloc(sizeof(double) * numLines);
	double* baroAlt = (double*)malloc(sizeof(double) * numLines);

	// Usable array instead of MPU9250 struct to pass into functions
	// Passing an array of structs and looping through those structs was funky
	double** allAccel = (double**)malloc(sizeof(double*) * 4);
	double** allGyro = (double**)malloc(sizeof(double*) * 3);

	position xyPos;

	char gridSquare[3];

	

    toStructs(accelFile, gyroFile, baroFile, bigAccel, gyroAll, baro, numLines);

	toArrays(time, baroAlt, bigAccel, gyroAll, baro, allAccel, allGyro, numLines);
	

	plotData("X - Acceleration", "time (s)", "acceleration (m/s/s)", "xAccel.png", time, allAccel[1], numLines);
	plotData("Y - Acceleration", "time (s)", "acceleration (m/s/s)", "yAccel.png", time, allAccel[2], numLines);
	plotData("Z - Acceleration", "time (s)", "acceleration (m/s/s)", "zAccel.png", time, allAccel[3], numLines);
	plotData("Barometric Altitde", "time (s)", "altitude (m)", "baroAlt.png", time, baroAlt, numLines);
	plotData("Barometric Altitde", "time (s)", "altitude (m)", "gyroRoll.png", time, allGyro[0], numLines - 1);
	xyPos = findVelAndPos(xyPos, allAccel, allGyro, baroAlt, numLines);
	printf("\n");
	printf("X - Position (m): %lf\nY - Position (m): %lf\n", xyPos.xpos, xyPos.ypos);
	findGridSquare(xyPos, gridSquare);
	printf("Grid Square: %s\n", gridSquare);
	//plotData("X - Velocity", "time (s)", "velocity (m/s)", "test.png", time, veloX, numLines - 2);
}
