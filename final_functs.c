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
#include "pbPlots.h"
#include "supportLib.h"
#include "final_functs.h"

void plotData(char* title, char* xAxis, char* yAxis, double xs [], double ys [], int numItems){
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
	if(strcmp(title, "X - Acceleration") == 0){
		settings->title = L"X - Acceleration";
	}
	if(strcmp(title, "Y - Acceleration") == 0){
		settings->title = L"Y - Acceleration";
	}
	if(strcmp(title, "Barometric Altitude") == 0){
		settings->title = L"Barometric Altitude";
	}
    settings->titleLength = wcslen(settings->title);
    if(strcmp(xAxis, "time (s)") == 0){
		settings->xLabel = L"time (s)";
	}
    settings->xLabelLength = wcslen(settings->xLabel);
    if(strcmp(yAxis, "acceleration (m/s/s)") == 0){
		settings->yLabel = L"acceleration (m/s/s)";
	}
	if(strcmp(yAxis, "altitude (ft)") == 0){
		settings->yLabel = L"altitude (ft)";
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
        WriteToFile(pngdata, length, "dataNew.png");
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
	// for(int c = getc(fp); c != EOF; c = getc(fp)){
    //     if(c == '\n'){
    //     	count += 1;
	// 	}
	// }
	char line[MAX_LINE_LENGTH];

	// Get rid of the title row of information
	fgets(line, MAX_LINE_LENGTH, fp);

	// Iterate through the whole file and count number of lines
	while(!feof(fp)){
		fgets(line, MAX_LINE_LENGTH, fp);
		count++;
	}

	return count;
}

// OUTSOURCING DATA FILES FROM ROCKETRY COMMUNITY (trying to at least)
void toAccelStruct(FILE* inFile, MPU9250 acceleration[], int numLines){
	double value = 0.0;
	int count = 0;
	int i = 0;
	char line[MAX_LINE_LENGTH];
	fgets(line, MAX_LINE_LENGTH, inFile);

	while(count < numLines){
		fscanf(inFile, "%lf,%lf,%lf", &acceleration[count].time, &acceleration[count].accelX, &acceleration[count].accelY);
		if(count % 1000 == 0){
			printf("%lf %lf %lf \n", acceleration[count].time, acceleration[count].accelX, acceleration[count].accelY);
		}
		count++;
	}
}

void toArrays(double time [], double accelX [], double accelY [], MPU9250* accel, int numLines){
	for(int i = 0; i < numLines; i++){
		time[i] = accel[i].time;
		if(9.80665 * accel[i].accelX < 150.0 && 9.80665 * accel[i].accelX > -150.0){
			accelX[i] = 9.80665 * accel[i].accelX;
		}
		else{
			accelX[i] = 0.0;
		}
		if(9.80665 * accel[i].accelY < 150.0 && 9.80665 * accel[i].accelY > -150.0){
			accelY[i] = 9.80665 * accel[i].accelY;
		}
		else{
			accelY[i] = 0.0;
		}
	}
}
