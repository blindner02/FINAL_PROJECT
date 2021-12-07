/** final_functs.h
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT FILES FROM A ROCKET/DRONE AND FIND/CALCULATE ESSENTIAL DATA AND GRAPH IT
 * =========================================================== */ 

#ifndef FINAL_PROJECT_FUNCTS
#define FINAL_PROJECT_FUNCTS
#define MAX_LINE_LENGTH 1024
#define MAX_FILE_NAME_LENGTH 100
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

/**
* @brief the file will be opened
* @param fileName the reading file pointer
* @return FILE* pointer
* @pre none
* @post none
*/
FILE* openFile(char* fileName);

/**
* @brief counts the number of lines in the csv
* @param fileName file pointer to any csv
* @return int of number of lines
* @pre fileName is a valid file
* @post none
*/
int countLines(FILE* fileName);

/**
* @brief the structs and arrays of structs will be populated with data
* @param accelFile file pointer to acceleration file
* @param gyroFile file pointer to gyro file
* @param baroFile file pointer to baro file
* @param acceleration array of MPU9250 type structs
* @param gyroAll array of GYRO type structs
* @param baro array of BMP390 type structs
* @param numLines integer of number of lines in csv
* @return none
* @pre all inputs are valid 
* @post the structs will be populated
*/
void toStructs(FILE* accelFile, FILE* gryoFile, FILE* baroFile, MPU9250 acceleration[], GYRO gyroAll[], BMP390 baro[], int numLines);

/**
* @brief the arrays and 2d arrays ill be populated with data
* @param time pointer to array of time values
* @param baroAlt pointer array of altitude values
* @param accel pointer to array of acceleration structs
* @param gyro pointer to array of gyro structs
* @param baro pointer to array of baro structs
* @param allAccel 2d array of acceleration x, y, z, and time values
* @param allGyro 2d array of gyro x, y, z values
* @param numLines integer of number of lines in csv
* @return none
* @pre all inputs are valid 
* @post the arrays will be populated
*/
void toArrays(double time [], double baroAlt [], MPU9250* accel, GYRO* gyro, BMP390* baro, double* allAccel [], double* allGyro [], int numLines);

/**
* @brief data is taken in and the velocity and position of the object is calculated
* @param xyPos struct of x and y positions
* @param accel 2d array of acceleration x, y, z, and time values
* @param gyro 2d array of gyro x, y, z values
* @param baro array of altitude values
* @param numLines integer of number of lines in csv
* @return position type variable of the x and y displacement
* @pre all inputs are valid 
* @post none
*/
position findVelAndPos(position xyPos, double* accel[], double* gyro[], double* baro, int numLines);

/**
* @brief calculate the grid square based on the x and y positions 
* @param xyPos struct of x and y positions
* @param gridSquare pointer to a string to store the grid
* @return none
* @pre all inputs are valid 
* @post gridSquare is modified to have the designated gird square in it
*/
void findGridSquare(position xyPos, char* gridSquare);

#endif