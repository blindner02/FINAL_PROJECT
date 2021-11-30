/** final_functs.c
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT DATA FILES FROM A ROCKET AND FIND ESSENTIAL DATA AND GRAPH IT
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
    }
}

FILE* openFile(char* fileName){
	FILE* inFile = fopen(fileName, "r");
	if (fileName == NULL){
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
	fgets(line, MAX_LINE_LENGTH, accelFile);
	fgets(line, MAX_LINE_LENGTH, gryoFile);
	fgets(line, MAX_LINE_LENGTH, baroFile);
	printf("%8s %8s %8s %8s\n", "time (s)", "x-accel", "y-accel", "z-accel");
	while(count < numLines){
		fscanf(accelFile, "%lf,%lf,%lf,%lf", &acceleration[count].time, &acceleration[count].accelX, &acceleration[count].accelY, &acceleration[count].accelZ);
		fscanf(gryoFile, "%lf,%lf,%lf,%lf", &gyroAll[count].time, &gyroAll[count].angleRoll, &gyroAll[count].anglePitch, &gyroAll[count].angleYaw);
		fscanf(baroFile, "%lf,%lf", &baro[count].time, &baro[count].altitude);
		if(count % 10000 == 0){
			printf("%lf %lf %lf %lf\n", acceleration[count].time, (acceleration[count].accelX / 2023) * 9.80665, (acceleration[count].accelY / 2023) * 9.80665, (acceleration[count].accelZ / 2023) * 9.80665);
		}
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

		// FILTER OUT THE OUTLIERS TO AN EXTENT
		// CONVERT THE DATA FROM G's TO (m/s/s)
		//if(9.80665 * accel[i].accelX < 150.0 && 9.80665 * accel[i].accelX > -150.0){
			accelX[i] = (accel[i].accelX / 2023) * 9.80665;
			accel[i].accelX = accelX[i];
		//}
		/*else{
			accelX[i] = 0.0;
		}*/
		//if(9.80665 * accel[i].accelY < 150.0 && 9.80665 * accel[i].accelY > -150.0){
			accelY[i] = (accel[i].accelY / 2023) * 9.80665;
			accel[i].accelY = accelY[i];
		//}
		/*else{
			accelY[i] = 0.0;
		}*/
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

	allAccel[0] = time;
	allAccel[1] = accelX;
	allAccel[2] = accelY;
	allAccel[3] = accelZ;

	allGyro[0] = gyroRoll;
	allGyro[1] = gyroPitch;
	allGyro[2] = gyroYaw;

	printf("%lf %lf %lf %lf\n", allAccel[0][0], allAccel[1][0], allAccel[2][0], allAccel[3][0]);
}
position findVelAndPos(position xyPos, double* accel[], double* gyro[], double* baro, int numLines, double posX [], double posY []){
	//double dispX = 0.0;
	//double dispY = 0.0;






	/*
	for(int i = 0; i < numLines - 1; i++){
		veloX[i] = 0.5 * (time[i + 1] - time[i]) * (accelX[i + 1] + accelX[i]);
	}
	for(int i = 0; i < numLines - 1; i++){
		veloY[i] = 0.5 * (time[i + 1] - time[i]) * (accelY[i + 1] + accelY[i]);
	}
	for(int i = 0; i < numLines - 2; i++){
		posX[i] = 0.5 * (time[i + 1] - time[i]) * (veloX[i + 1] + veloX[i]);
		dispX = dispX + (0.5 * (time[i + 1] - time[i]) * (veloX[i + 1] + veloX[i]));
	}
	for(int i = 0; i < numLines - 2; i++){
		posY[i] = 0.5 * (time[i + 1] - time[i]) * (veloY[i + 1] + veloY[i]);
		dispY = dispY + (0.5 * (time[i + 1] - time[i]) * (veloY[i + 1] + veloY[i]));
	}*/
	//xyPos.xpos = dispX;
	//xyPos.ypos = dispY;
	return (xyPos);
}