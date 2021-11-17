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

void plotData(double xs [], double ys [], int numItems){
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
	series->lineThickness = 2;
	series->color = GetGray(0.3);

	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 925;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
    settings->title = L"Altitude Data";
    settings->titleLength = wcslen(settings->title);
    settings->xLabel = L"time (s)";
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabel = L"Height (m)";
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
        WriteToFile(pngdata, length, "data1.png");
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

// OUTSOURCING DATA FILES FROM ROCKETRY COMMUNITY (trying to at least)
void toAccelStruct(FILE* fileName, MPU9250 acceleration[]){
	double value = 0.0;
	int count = 0;
	int i = 0;
	char line[MAX_LINE_LENGTH];
	
	while(!feof(fileName)){
		acceleration = (MPU9250*)realloc(acceleration, (count + 1) * sizeof(MPU9250));
		fscanf(fileName, "%lf,%lf,%lf,%lf,%lf,%lf,%lf", &acceleration[count].accelX, &acceleration[count].accelY, 
		&acceleration[count].accelZ, &acceleration[count].pitch, &acceleration[count].roll, &acceleration[count].yaw,
		&acceleration[count].time);
		count++;
	}
}