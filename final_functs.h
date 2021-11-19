/** final_functs.h
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT FILES FROM A ROCKET AND FIND ESSENTIAL DATA AND GRAPH IT
 * =========================================================== */ 

#ifndef FINAL_PROJECT_FUNCTS
#define FINAL_PROJECT_FUNCTS
#define MAX_LINE_LENGTH 1024
typedef struct MPU9250{
    double time;
    double accelX;
    double accelY;
}MPU9250;

typedef struct BMP390{
    double altitude;
    double time;
}BMP390;

/**
* @brief receives an int from the user
* @param none
* @return num the safe int
* @pre user inputs a valid nt
* @post none
*/
void plotData(double xs [], double ys [], int numItems);

FILE* openFile(char* fileName);

void toAccelStruct(FILE* fileName, MPU9250 acceleration[], int numLines);

int countLines(FILE* fileName);

#endif