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
    double accelZ;
}MPU9250;

typedef struct GYRO{
    double time;
    double angleRoll;
    double anglePitch;
    double angleYaw;
}GYRO;

typedef struct BMP390{
    double altitude;
    double time;
}BMP390;

typedef struct position{
    double xpos;
    double ypos;
}position;

/**
* @brief receives an int from the user
* @param none
* @return num the safe int
* @pre user inputs a valid nt
* @post none
*/
void plotData(char* title, char* xAxis, char* yAxis, char* fileName, double xs [], double ys [], int numItems);

FILE* openFile(char* fileName);

int countLines(FILE* fileName);

void toStructs(FILE* accelFile, FILE* gryoFile, FILE* baroFile, MPU9250 acceleration[], GYRO gyroAll[], BMP390 baro[], int numLines);

void toArrays(double time [], double accelX [], double accelY [], double accelZ [], MPU9250* accel, int numLines);

position findVelAndPos(position xyPos, MPU9250* accel, GYRO* gyroMeasure, int numLines, double posX [], double posY []);

#endif