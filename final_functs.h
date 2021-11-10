/** final_functs.h
 * =========================================================== 
 * Name: Brandon Lindner
 * Section: T1B-T2B
 * Project: FINAL PROJECT
 * Purpose: PARSE FLIGHT DATA FILES FROM A ROCKET AND FIND ESSENTIAL DATA AND GRAPH IT
 * =========================================================== */ 

/**
* @brief receives an int from the user
* @param none
* @return num the safe int
* @pre user inputs a valid nt
* @post none
*/
void drawPlot();

FILE* openFile(char* fileName);