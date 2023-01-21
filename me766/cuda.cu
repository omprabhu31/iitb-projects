#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <cuda.h>
#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;
#define TILE 16
  
void initialize_matrices(double** a, double** l, double** u, int size)
{
  for (int i = 0; i < size; ++i)
  {
    a[i] = new double[size];
    l[i] = new double[size];
    u[i] = new double[size];
  }
}

__global__ void scaleIndex(double *matrix, int n, int index){
  int start=(index*n+index);
	int end=(index*n+n);
	
	for(int i= start+1 ; i<end; ++i){
		matrix[i]=(matrix[i]/matrix[start]);
	}

}


__global__ void elim(double *A, int n, int index, int bsize){
	extern __shared__ double pivot[];

	int idThread=threadIdx.x;
	int idBlock=blockIdx.x;
	int blockSize=bsize;


	if(idThread==0){
	     for(int i=index;i<n;i++) pivot[i]=A[(index*n)+i];
	}

	__syncthreads();
	int pivotRow=(index*n);
	int currentRow=(((blockSize*idBlock) + idThread)*n);
	int start=currentRow+index;
	int end=currentRow+n;
	if(currentRow >pivotRow){
    for(int i= start+1; i<end; ++i){

        A[i]=A[i]-(A[start]*pivot[i-currentRow]);

             }
      }
}

void fillMatrix(double* a, int n){

   for (int i = 0; i <= (n*n); ++i) {
    a[i] =((rand()%10)+1);
  }


  int diagCount = 0;
  double sum = 0;
  for(int i = 0; i < n; ++i){

    for(int j = i*n; j < i*n + n; ++j){
      sum += abs(a[j]);

    }

    sum -= abs(a[i*n + diagCount]);
    a[i*n + diagCount] = sum + ((rand()%5)+1);
    ++diagCount;
    sum = 0;
  }

}

void printMatrix(double* a, int n){
    for(int i=0; i<(n*n); ++i){
           if(i%n==0)
       		   cout << endl << left << setw(9) << setprecision(3) << a[i] << left <<  setw(9);
           else cout << left << setw(9) << setprecision(3) << a[i] << left <<  setw(9);
         }

    cout << endl;
}

void print2D(double** matrix, int size)
{
  
  for (int i = 0; i < size; i++)
  {
   
    for (int j = 0; j < size; j++)
    {
      
      cout << left << setw(9) << setprecision(3) << matrix[i][j] << left <<  setw(9);
    }
   
    cout << endl;
  }
}

int main(int argc, char** argv){
  
  int n = atoi(argv[1]);

  srand(1);

  
  double *a = new double[n*n];
  double *ret=new double[n*n];

  
  fillMatrix(a, n);
 

  
  double *da;
  int numblock = n/TILE + ((n%TILE)?1:0);

  double runtime;
  runtime = clock()/(double)CLOCKS_PER_SEC;
  cudaMalloc(&da, n*n* sizeof (double));
  cudaMemcpy(da, a, n*n*sizeof(double), cudaMemcpyHostToDevice);
  for(int i=0; i<n; ++i){
      scaleIndex<<<1,1>>>(da,n,i);
	    elim<<<numblock,TILE,n*sizeof(double)>>>(da,n,i,TILE);
   }
  cudaMemcpy(ret, da, n*n*sizeof(double),cudaMemcpyDeviceToHost );
  runtime = clock() - runtime;
  printf("For %u x %u Matrix\n",n,n);
  cout << "Runtime for LU Decomposition is: " << (runtime)/float(CLOCKS_PER_SEC) << endl;
  
  double** A = new double* [n];
  double** u = new double* [n];
  double** l = new double* [n];
 
  
  initialize_matrices(A,u,l,n);
  for(int i = 0 ;i < n ; ++i){
    for(int j= 0; j < n; ++j){
      A[i][j]=ret[i*n+j];
    }
  }
  
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
        
        for(int k=0; k<n; k++){
            
            if(i>=k)
              l[i][k] = A[i][k];
            
            else l[i][k] = 0;
            
            if(k==j)u[k][j] = 1;
            else if(k<j)u[k][j] = A[k][j];
            else u[k][j] = 0.0;
        }
    }
  }
  
 if(atoi(argv[2]) == 1){
  printf("Matrix 'A' is:\n");
  printMatrix(a,n);  
  printf("Matrix 'L' is:\n");
  print2D(l,n);
  printf("Matrix 'U' is: \n");
  print2D(u,n);
 }


  cudaFree(da);
  cudaFree(ret);
  delete[] a;
  delete[] ret; 

  return 0;
}