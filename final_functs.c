/** final_functs.c
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT DATA FILES FROM A ROCKET/DRONE AND FIND/CALCULATE ESSENTIAL DATA AND GRAPH IT
 * =========================================================== */ 

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <unistd.h>
#include "pbPlots.h"
#include "supportLib.h"
#include "final_functs.h"

void plotData(char* title, char* xAxis, char* yAxis, char* fileName, double xs [], double ys [], int numItems){
	_Bool success;
    StringReference *errorMessage;

	ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
	series->xs = xs;
	series->xsLength = numItems;
	series->ys = ys;
	series->ysLength = numItems;
	series->linearInterpolation = true;
	series->lineType = L"solid";
	series->lineTypeLength = wcslen(series->lineType);
	series->lineThickness = 1;
	series->color = GetGray(0.3);
	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 700;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	
	// THERE IS NO EASY WAY TO CHANGE THE TITLES WITHOUT CHANGING THE SOURCE OF pbPlots 
	// SO I HAD TO DO ALL THE strcmp() TO SET THE TITLES 
	if(strcmp(title, "X - Acceleration") == 0){
		settings->title = L"X - Acceleration";
	}
	if(strcmp(title, "Y - Acceleration") == 0){
		settings->title = L"Y - Acceleration";
	}
	if(strcmp(title, "Z - Acceleration") == 0){
		settings->title = L"Z - Acceleration";
	}
	if(strcmp(title, "Barometric Altitude") == 0){
		settings->title = L"Barometric Altitude";
	}
	if(strcmp(title, "X - Velocity") == 0){
		settings->title = L"X - Velocity";
	}
	if(strcmp(title, "Y - Velocity") == 0){
		settings->title = L"Y - Velocity";
	}
    settings->titleLength = wcslen(settings->title);
    if(strcmp(xAxis, "time (s)") == 0){
		settings->xLabel = L"time (s)";
	}
    settings->xLabelLength = wcslen(settings->xLabel);
    if(strcmp(yAxis, "acceleration (m/s/s)") == 0){
		settings->yLabel = L"acceleration (m/s/s)";
	}
	if(strcmp(yAxis, "velocity (m/s)") == 0){
		settings->yLabel = L"velocity (m/s)";
	}
	if(strcmp(yAxis, "altitude (m)") == 0){
		settings->yLabel = L"altitude (m)";
	}
    settings->yLabelLength = wcslen(settings->yLabel);
	ScatterPlotSeries *s [] = {series};
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = 1;
	RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
	errorMessage = (StringReference *)malloc(sizeof(StringReference));
	success = DrawScatterPlotFromSettings(canvasReference, settings, errorMessage);

	if(success){
        size_t length;
        double *pngdata = ConvertToPNG(&length, canvasReference->image);
        WriteToFile(pngdata, length, fileName);
        DeleteImage(canvasReference->image);
	}else{
        fprintf(stderr, "Error: ");
        for(int i = 0; i < errorMessage->stringLength; i++){
            fprintf(stderr, "%c", errorMessage->string[i]);
        }
        fprintf(stderr, "\n");
		exit(-1);
    }
}

FILE* openFile(char* fileName){
	FILE* inFile = fopen(fileName, "r");
	if (inFile == NULL){
		fprintf(stderr, "Could not open file \"%s\"", fileName);  
        fprintf(stderr, " - terminating\n"); 
		exit(-1);
	}
	return inFile;
}

int countLines(FILE* fp){
	int count = 0;
	char line[MAX_LINE_LENGTH];

	// GET RID OF THE TITLE LINE
	fgets(line, MAX_LINE_LENGTH, fp);

	while(!feof(fp)){
		fgets(line, MAX_LINE_LENGTH, fp);
		count++;
	}
	return count;
}

// OUTSOURCING DATA FILES FROM ROCKETRY COMMUNITY (trying to at least)
void toStructs(FILE* accelFile, FILE* gryoFile, FILE* baroFile, MPU9250 acceleration[], GYRO gyroAll[], BMP390 baro[], int numLines){
	int count = 0;
	char line[MAX_LINE_LENGTH];
	// get rid of the titles for each of the csv files so it starts parsing at the actual data
	fgets(line, MAX_LINE_LENGTH, accelFile);
	fgets(line, MAX_LINE_LENGTH, gryoFile);
	fgets(line, MAX_LINE_LENGTH, baroFile);
// Parse the data files and input the values into the structs
	while(count < numLines){
		fscanf(accelFile, "%lf,%lf,%lf,%lf", &acceleration[count].time, &acceleration[count].accelX, &acceleration[count].accelY, &acceleration[count].accelZ);
		fscanf(gryoFile, "%lf,%lf,%lf,%lf", &gyroAll[count].time, &gyroAll[count].angleRoll, &gyroAll[count].anglePitch, &gyroAll[count].angleYaw);
		fscanf(baroFile, "%lf,%lf", &baro[count].time, &baro[count].altitude);
		
		// print out every 10000th value to make sure arrays are being filled
		/*if(count % 10000 == 0){
			printf("%lf %lf %lf %lf\n", acceleration[count].time, (acceleration[count].accelX / 2023) * 9.80665, (acceleration[count].accelY / 2023) * 9.80665, (acceleration[count].accelZ / 2023) * 9.80665);
		}*/
		
		count++;
	}
}

void toArrays(double time [], double baroAlt [], MPU9250* accel, GYRO* gyro, BMP390* baro, double* allAccel [], double* allGyro [], int numLines){
	int g = 0;
	double delta_timeR;
	double delta_timeP;
	double delta_timeY;
	double delta_angleR;
	double delta_angleP;
	double delta_angleY;
	double* accelX = (double*)malloc(sizeof(double) * numLines);
	double* accelY = (double*)malloc(sizeof(double) * numLines);
	double* accelZ = (double*)malloc(sizeof(double) * numLines);
	double* gyroRoll = (double*)malloc(sizeof(double) * numLines);
	double* gyroPitch = (double*)malloc(sizeof(double) * numLines);
	double* gyroYaw = (double*)malloc(sizeof(double) * numLines);


	for(int i = 0; i < numLines; i++){
		time[i] = accel[i].time;

		// CONVERT THE DATA FROM G's TO (m/s/s)
		accelX[i] = (accel[i].accelX / 2023) * 9.80665;
		accel[i].accelX = accelX[i];
		
		accelY[i] = (accel[i].accelY / 2023) * 9.80665;
		accel[i].accelY = accelY[i];
	
		accelZ[i] = ((accel[i].accelZ / 2023) * 9.80665) - 9.80665;
		accel[i].accelZ = accelZ[i];

		baroAlt[i] = baro[i].altitude / 100;
		baro[i].altitude = baroAlt[i];

		if(i > 0){
			delta_timeR = time[i] - time[i - 1];
			delta_angleR = gyro[g].angleRoll * delta_timeR; 
			if(g == 0){
				gyroRoll[g] = delta_angleR;
			}
			gyroRoll[g] = delta_angleR + gyroRoll[g - 1];

			delta_timeP = time[i] - time[i - 1];
			delta_angleP = gyro[g].anglePitch * delta_timeP; 
			if(g == 0){
				gyroPitch[g] = delta_angleP;
			}
			gyroPitch[g] = delta_angleP + gyroPitch[g - 1];

			delta_timeY = time[i] - time[i - 1];
			delta_angleY = gyro[g].angleYaw * delta_timeY; 
			if(g == 0){
				gyroYaw[g] = delta_angleY;
			}
			gyroYaw[g] = delta_angleY + gyroYaw[g - 1];

			g++;
		}
	}
// Fill the large 2d arrays with the values to use in other functions
	allAccel[0] = time;
	allAccel[1] = accelX;
	allAccel[2] = accelY;
	allAccel[3] = accelZ;

	allGyro[0] = gyroRoll;
	allGyro[1] = gyroPitch;
	allGyro[2] = gyroYaw;

// My attempt at some sort of a filter (It kind of works!)
	double offsetR = gyroRoll[numLines - 2] / time[numLines - 1];
	double offsetP = gyroPitch[numLines - 2] / time[numLines - 1];
	double offsetY = gyroYaw[numLines - 2] / time[numLines - 1];

	for(int j = 0; j < numLines - 1; j++){
		gyroRoll[j] = gyroRoll[j] - (time[j] * offsetR);
		gyroPitch[j] = gyroPitch[j] - (time[j] * offsetP);
		gyroYaw[j] = gyroYaw[j] - (time[j] * offsetY);
	}

	allGyro[0] = gyroRoll;
	allGyro[1] = gyroPitch;
	allGyro[2] = gyroYaw;

	
//expected to have some deviation because of integration so print out the calculated offset (TEST)
	/*
	printf("Offsets: %lf %lf %lf\n", offsetR, offsetP, offsetY);
	*/

//comparing the calculated values to the EXPECTED data values to ensure continuity (TEST)
	/*
	printf("%lf %lf %lf %lf\n", allAccel[0][0], allAccel[1][0], allAccel[2][0], allAccel[3][0]);
	printf("%lf %lf %lf\n", allGyro[0][100], allGyro[1][100], allGyro[2][100]);
	*/
}

position findVelAndPos(position xyPos, double* accel[], double* gyro[], double* baro, int numLines){
	double dispX = 0.0;
	double dispY = 0.0;

	double* xAccel = (double*)malloc(sizeof(double) * numLines - 2);
	double* yAccel = (double*)malloc(sizeof(double) * numLines - 2);

	double* veloX = (double*)malloc(sizeof(double) * numLines - 3);
	double* veloY = (double*)malloc(sizeof(double) * numLines - 3);
// CALCULATING THE POSITION BY TAKING THE TRAPEZOIDAL RIEMANN SUM AS AN INTEGRAL
	for(int i = 0; i < numLines - 2; i++){
		xAccel[i] = (accel[3][i] * sin(gyro[0][i] + 180)) + (accel[1][i] * cos(gyro[2][i])) + (accel[2][i] * sin(gyro[2][i] + 180));
		yAccel[i] = (accel[3][i] * sin(gyro[1][i] + 180)) + (accel[2][i] * cos(gyro[2][i])) + (accel[1][i] * sin(gyro[2][i]));
	}
	for(int i = 0; i < numLines - 3; i++){
		veloX[i] = 0.5 * (accel[0][i + 1] - accel[0][i]) * (xAccel[i + 1] + xAccel[i]);
	}
	for(int i = 0; i < numLines - 3; i++){
		veloY[i] = 0.5 * (accel[0][i + 1] - accel[0][i]) * (yAccel[i + 1] + yAccel[i]);
	}
	for(int i = 0; i < numLines - 4; i++){
		dispX = dispX + (0.5 * (accel[0][i + 1] - accel[0][i]) * (veloX[i + 1] + veloX[i]));
	}
	for(int i = 0; i < numLines - 4; i++){
		dispY = dispY + (0.5 * (accel[0][i + 1] - accel[0][i]) * (veloY[i + 1] + veloY[i]));
	}
	xyPos.xpos = dispX * 100;
	xyPos.ypos = dispY * 100;

	free(xAccel);
	free(yAccel);
	free(veloX);
	free(veloY);

	return (xyPos);
}

void findGridSquare(position xyPos, char* gridSquare){
	char letter = 'A';
	char num[10];
	int intNum = 20;
	
// test code	
	// xyPos.xpos = 1.200;
	// xyPos.ypos = 10.056;

// Failsafe	
	if(xyPos.xpos > 10.0 || xyPos.xpos < -10.0){
		fprintf(stderr, "X displacement is out of grid range.\n");
		exit(-1);
	}
// Failsafe	
	if(xyPos.ypos > 10.0 || xyPos.ypos < -10.0){
		fprintf(stderr, "Y displacement is out of grid range.\n");
		exit(-1);
	}
// Check the x position for grid square
	for(int i = -10; i < 10; i++){
		if(xyPos.xpos > (double)i && xyPos.xpos < (double)i + 1.0){
			strcat(gridSquare, &letter);
		}
		letter++;
	}
// Check the y position for grid square
	for(int k = -10; k < 10; k++){
		if(xyPos.ypos > (double)k && xyPos.ypos < (double)k + 1.0){
			sprintf(num, "%d", intNum);
			strcat(gridSquare, num);
		}
		intNum--;
	}
}