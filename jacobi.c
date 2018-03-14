/* CSCI 347 jacobi method using n threads
 *
 * 10 MAR 2018, Zach Richardson and Daniel Wilson
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <>
#define EPSILON 0.00001

/*
 */
void changeChecker(double* maxChange, double CHALLENGER);

/*
 */
void matrixChanger(double* mtx1, double* mtx2, int threadNum, int NumOfThreads, double* maxChange);

/*fillMatrix
 * fills a 1024x1024 2d array with values from a file named "input"
 *  input: file to read values from.
 *  mtx: empty 2d array to be filled.
*/
void fillMatrix(FILE* input, double* mtx);

/* TO DO:
 * debug jacobi
 * mutex
 * find some way to time the program
 * add parallelization:
 *   mutex for maxChange
 *   make matrix changes loop according to threadNum and NumOfThreads
 */
int main(int argc, const char* argv[]) {
  char* fileName = "input.mtx";
  FILE* input = fopen(fileName, "r");
  double *mtx1 = (double *) malloc(1024*1024*sizeof(double));
  double *mtx2;
  double maxChange = 0;
  int threadNum = 1;
  int NumOfThreads = *argv[0];

  fillMatrix(input, mtx1);
  memcpy(mtx2, mtx1, 1024*1024*sizeof(double));
  for(;threadNum < NumOfThreads; threadNum++) {
    //new thread of matrixChanger
    matrixChanger(mtx1, mtx2, threadNum, NumOfThreads, &maxChange);
  }
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
 */
void matrixChanger(double* mtx1, double* mtx2, int threadNum, int NumOfThreads, double* maxChange) {

  while(*maxChange > EPSILON) {
    for(int row = threadNum; row < 1023; row+=NumOfThreads) {
      for(int col = 1; col < 1023; col++) {
        mtx2[(row*1024)+col] = mtx1[((row-1)*1024)+col] + mtx1[(row*1024)+(col-1)] +
                         mtx1[(row*1024)+(col+1)] + mtx1[((row+1)*1024)+col];
        mtx2[(row*1024)+col] = mtx2[(row*1024)+(col)]/4;
        if(mtx2[(row*1024)+col] > *maxChange) {
          changeChecker(maxChange, mtx2[(row*1024)+col]);
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
          if(mtx2[(row*1024)+col] > *maxChange) {
            changeChecker(maxChange, mtx2[(row*1024)+col]);
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
    fscanf(input, "%d", mtx[(row*1024)+0]);
    //mtx[col] = line[i];
    i++;
  }

  linelen = getline(&line, &bufsize, input);

  for(row = 1; row < 1023; row++) {
    fscanf(input, "%d", mtx[(row*1024)+0]);
    for(col = 0; col < 1023; col++){
      mtx[(row*1024)+col]= 0.0000000000;
    }
    mtx[(row*1024)+1023] = line[1023];
    linelen = getline(&line, &bufsize, input);
  }

  i = 0;
  for(col = 0; col < 1024; col++) {
    mtx[(1023*1024)+col] = line[i];
    i++;
  }
}

double gross(char* line, int i) {
  double retVal = 0;
  int decFound = 0;
  int decPoint = 0;
  int j = 0;

  while(i) {
    if(isspace(line[j])) {
      i--;
    }
    j++;
  }

  j--;

  while(!isspace(line[j])) {
    if(line[j] == 46) {
      decFound = 1;
    } else if(!decFound) {
      retVal = retVal + (line[j]- 48);
      retVal = retVal * 10;
      j++;
    } else {
      retVal = retVal + (line[j] * (10^decPoint));
      decPoint--;
    }
  }

  return retVal;
}
