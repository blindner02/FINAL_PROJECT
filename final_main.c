/** final_main.c
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT DATA FILES FROM A ROCKET/DRONE AND FIND/CALCULATE ESSENTIAL DATA AND GRAPH IT
 * Documentation: C3C Ameen Khan assisted me in passing the two-dimensional arrays into the plotData function
 * 				  C3C Luke Ringe assisted me in parsing my csv files and the proper syntax for the fscanf 
 * 				  Maj Birrer assisted in trying to figure out why the end of my findGridSquare function was 
 * 				  giving a *** stack smashing detected *** error
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

/* Test code */	
	// char* inAccel = "accel_test.csv";
	// char* inGyro = "gyro_test.csv";
	// char* inBaro = "baro_test.csv";


// comment out this block ***
	char inAccel[MAX_FILE_NAME_LENGTH];
	char inGyro[MAX_FILE_NAME_LENGTH];
	char inBaro[MAX_FILE_NAME_LENGTH];
	
	printf("Enter the name of a properly formatted acceleration data .csv : ");
	scanf("%s", inAccel);

// leave this	
	accelFile = openFile(inAccel);

	printf("Enter the name of a properly formatted gyroscopic data .csv : ");
	scanf("%s", inGyro);

// leave this	
	gyroFile = openFile(inGyro);

	printf("Enter the name of a properly formatted barometric pressure .csv : ");
	scanf("%s", inBaro);

// leave this
	baroFile = openFile(inBaro);

/// *** to test

	int numLines = countLines(accelFile);

// Test code	
	// int numLines = 0;
	numLines = numLines - 1;
	printf("Number of items: %d\n", numLines);
	if(numLines < 0){
		fprintf(stderr, "Number of items must be a positive integer. Fill the csv with data.\n");
		exit(-1);
	}
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
	plotData("Barometric Altitude", "time (s)", "altitude (m)", "baroAlt.png", time, baroAlt, numLines);

	printf("\n");

	xyPos = findVelAndPos(xyPos, allAccel, allGyro, baroAlt, numLines);
	printf("X - Position (m): %lf\nY - Position (m): %lf\n", xyPos.xpos, xyPos.ypos);
	findGridSquare(xyPos, gridSquare);
	printf("Grid Square: %s\n", gridSquare);
	
	free(bigAccel);
	free(gyroAll);
	free(baro);
	free(time);
	free(baroAlt);
	free(allAccel);
	free(allGyro);

	exit(0);
}