/** final_functs.h
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT FILES FROM A ROCKET AND FIND ESSENTIAL DATA AND GRAPH IT
 * =========================================================== */ 

#ifndef FINAL_PROJECT_FUNCTS
#define FINAL_PROJECT_FUNCTS
typedef struct MPU9250{
    double accelX;
    double accelY;
    double accelZ;
    double pitch;
    double roll;
    double yaw;
    double time;
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

#endif