/* CSCI 347 jacobi method using n threads
 *
 * 10 MAR 2018, Zach Richardson and Daniel Wilson
 */

#include <stdlib.h>

void fillMatrix(FILE* input, double[][] mtx);

/* TO DO:
 * code jacobi
 * find some way to time the program
 * add parallelization
 */
int main(int argc, const char* argv[]) {
  char* fileName = "jacobi";
  FILE* input = fopen(fileName, "r");
  double[1024][1024] mtx1;
  double[1024][1024] mtx2;
  double maxChange = 0;
  double epsilon = 0.00001;

  fillMatrix(input, mtx1);

  while(maxChange > epsilon) {
    for(int row = 1; row < 1023; row++) {
      for(int col = 1; col < 1023; col++) {
        mtx2[row][col] = mtx1[row-1][col] + mtx1[row][col-1] +
                         mtx1[row][col+1] + mtx1[row+1][col];
        mtx2[row][col] = mtx2[row][col]/4;
        if(mtx2[row][col] > maxChange) {
          maxChange = mtx2[row][col]-mtx1[row][col];
        }
      }
    }

    if(maxChange <= epsilon) {
      for(int row = 1; row < 1023; row++) {
        for(int col = 1; col < 1023; col++) {
          mtx1[row][col] = mtx2[row-1][col] + mtx2[row][col-1] +
                           mtx2[row][col+1] + mtx2[row+1][col];
          mtx1[row][col] = mtx1[row][col]/4;
          if(mtx1[row][col] > maxChange) {
            maxChange = mtx1[row][col]-mtx2[row][col];
          }
        }
      }
    }
  }
}

void fillMatrix(FILE* input, double[][] mtx){
  int i = 0;
  int row = 0;
  int col = 0;
  char* line = NULL;
  size_t  bufsize = 0;
  ssize_t linelen = getline(&line, &bufsize, input);

  for(col = 0; col < 1024; col++) {
    mtx[col][0] = line[i];
    i++;
  }

  linelen = getline(&line, &bufsize, input);

  for(row = 1; row < 1023; row++) {
    mtx[0][row] = line[0];
    for(col = 0; col < 1023; col++){
      mtx[col][row]= 0.0000000000;
    }
    mrx[1023][row] = line[1023];
    linelen = getline(&line, &bufsize, input);
  }

  i = 0;
  for(col = 0; col < 1024; col++) {
    mtx[col][1023] = line[i];
    i++;
  }
}
