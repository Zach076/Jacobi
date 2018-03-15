/* CSCI 347 jacobi method using n threads
 *
 * 10 MAR 2018, Zach Richardson and Daniel Wilson
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define EPSILON .00001

/*ithCharToDouble
 * converts the ith token in a line to a double
 *  line: pointer to string containing tokens to convert
 *  i: index of the token to convert
 * returns the converted value as a double
*/
double ithCharToDouble(char* line, int i);

/*changeChecker
 * updates current maxChange value using a mutex.
 *  maxChange: pointer to current value of the largest change in the matrix.
 *  CHALLENGER: new value to compare against maxChange.
*/
void changeChecker(double* maxChange, double CHALLENGER);

/*matrixChanger
 * implements the jacobi equation using two matrices.
 *  mtx1: original matrix containing initial values
 *  mtx2: copy of mtx1 to use in algorithm
 *  threadnum: number specifying the current thread executing the function
 *  NumOfThreads: total number of threads executing the function
 *  maxChange: pointer to double specifying the maximum change of a value in the
 *           matrix.
 */
void matrixChanger(double* mtx1, double* mtx2, int threadNum, int NumOfThreads, double* maxChange);

/*fillMatrix
 * fills a 1024x1024 2d array with values from a file named "input"
 *  input: file to read values from.
 *  mtx: empty 2d array to be filled.
*/
void fillMatrix(FILE* input, double* mtx);

/* TO DO:
 * print output to file depending on where the jcobi method ends (mtx1 and mtx2)
 * clean up the damn warnings
 * find some way to time the program
 * add parallelization:
 *   mutex for maxChange
 *   make matrix changes loop according to threadNum and NumOfThreads
 */
int main(int argc, const char* argv[]) {
  char* fileName = "input.mtx";
  FILE* input = fopen(fileName, "r");
  double *mtx1 = (double *) malloc(1024*1024*sizeof(double));
  double *mtx2 = (double *) malloc(1024*1024*sizeof(double));
  double maxChange = 1;
  //int threadNum = 1;
  //int NumOfThreads = *argv[0];
  int NumOfThreads = 1;
  int x;

  fillMatrix(input, mtx1);
  memcpy(mtx2, mtx1, 1024*1024*sizeof(double));
  for(int threadNum = 1;threadNum <= NumOfThreads; threadNum++) {
    //new thread of matrixChanger
    matrixChanger(mtx1, mtx2, threadNum, NumOfThreads, &maxChange);
  }
  x = 1;
}

/*
 */
void changeChecker(double* maxChange, double CHALLENGER) {
  //lock here while in use
  if(CHALLENGER > *maxChange) {
    *maxChange = CHALLENGER;
  }
  //unlock
}

/*
 *
 *
 *
 *
 *
 *
 */
void matrixChanger(double* mtx1, double* mtx2, int threadNum, int NumOfThreads, double* maxChange) {

  while(*maxChange > EPSILON) {
    *maxChange = 0;
    for(int row = threadNum; row < 1023; row+=NumOfThreads) {
      for(int col = 1; col < 1023; col++) {
        mtx2[(row*1024)+col] = mtx1[((row-1)*1024)+col] + mtx1[(row*1024)+(col-1)] +
                         mtx1[(row*1024)+(col+1)] + mtx1[((row+1)*1024)+col];
        mtx2[(row*1024)+col] = mtx2[(row*1024)+(col)]/4;
        if((mtx2[(row*1024)+col]- mtx1[(row*1024)+col]) > *maxChange) {
          changeChecker(maxChange, (mtx2[(row*1024)+col]- mtx1[(row*1024)+col]));
        }
      }
    }
    //lock until all threads are here

    if(*maxChange > EPSILON) {
      for(int row = threadNum; row < 1023; row+=NumOfThreads) {
        for(int col = 1; col < 1023; col++) {
          mtx1[(row*1024)+col] = mtx2[((row-1)*1024)+col] + mtx2[(row*1024)+(col-1)] +
                           mtx2[(row*1024)+col+1] + mtx2[((row+1)*1024)+col];
          mtx1[(row*1024)+col] = mtx1[(row*1024)+col]/4;
          if((mtx1[(row*1024)+col]- mtx2[(row*1024)+col]) > *maxChange) {
            changeChecker(maxChange, (mtx1[(row*1024)+col]- mtx2[(row*1024)+col]));
          }
        }
      }
    }
    //lock until all threads are here
  }
}

/*fillMatrix
 * fills a 1024x1024 2d array with values from a file named "input"
 * does so using three for loops
 *
 * The first for loop fills row 0, where every value must be read
 *  from the file
 * The second for loop fills rows 1-1022, where only columns 0 and 1023 must
 *  be read from the file, and all other values can be set to 0
 * The final for loop fills row 1023, where every value must be read
 *  from the file
 */
void fillMatrix(FILE* input, double* mtx){
  int i = 0;
  int row = 0;
  int col = 0;
  char* line = NULL;
  size_t  bufsize = 0;
  ssize_t linelen = getline(&line, &bufsize, input);

  for(col = 0; col < 1024; col++) {
    mtx[col] = ithCharToDouble(line, i);
    i++;
  }

  linelen = getline(&line, &bufsize, input);

  for(row = 1; row < 1023; row++) {
    mtx[(row*1024)+0] = ithCharToDouble(line, 0);
    for(col = 1; col < 1023; col++){
      mtx[(row*1024)+col]= 0.0000000000;
    }
    mtx[(row*1024)+1023] = ithCharToDouble(line, 1023);
    linelen = getline(&line, &bufsize, input);
  }

  i = 0;
  for(col = 0; col < 1024; col++) {
    mtx[(1023*1024)+col] = ithCharToDouble(line, i);
    i++;
  }
}

double ithCharToDouble(char* line, int i) {
  double retVal = 0;
  int decFound = 0;
  double decPoint = 1;

  while(i) {
    if(isspace(line[j])) {
      i--;
    }
    j++;
  }

  while(!isspace(line[j])) {
    if(line[j] == 46) {
      decFound = 1;
    } else if(!decFound) {
      retVal = retVal * 10;
      retVal = retVal + (line[j]- 48);
    } else {
      decPoint /= 10;
      retVal += ((line[j]-48) * decPoint);
    }
    j++;
  }

  //retVal = fmod(retVal, 0.0000000001);
  return retVal;
}
