//OpenMP parallelization of Doolittle Algorithm for LU decomposition

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>
#include <cmath>
#include <omp.h>
#define chunk 10
#define ld long double
#include <bits/stdc++.h>

using namespace std;
int nThreads,n;
int main() {
	for (int n = 100; n <= 2000; n+= 100) {

		for (int nThreads = 1; nThreads <= 8; nThreads *= 2) {
			omp_set_num_threads(nThreads);
			int i, k, j;

			//define time parameters
			float t1=omp_get_wtime()*1000;
		
			//define matrix
			float *a = (float *)malloc(n * n * sizeof(float));			
			float *l = (float *)malloc(n * n * sizeof(float));			
			float *u = (float *)malloc(n * n * sizeof(float));			
			
			srand((unsigned)time(NULL));
			float randomno;
			#pragma omp parallel for default(none), private(randomno), shared(a,n)
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
				{	
				float randomno = (((float) rand()) / (float) RAND_MAX);
				//*(a+i*n+j)=randomno;
				*(a+i*n+j)=i+j-randomno;
				}
			}

			//define upper and lower triangular matrices based on doolittle algorithm

			/*float sum=0;
			for (int p = 0; p < n; p++) {
				for (int q = 0; q <= p; q++) {

					if(p==q){ 
						*(l+p*n+p)=1;
						*(u+p*n+q) = 0;
					}
					else{
						*(l+q*n+p) = 0;
						*(u+p*n+q) = 0;
					}

					for (int r = 0; r < q; r++) {
						sum += ((*(l+p*n+r)) * (*(u+r*n+q)));
					}

					*(l+p*n+q) = (*(a+p*n+q) - sum)/(*(u+q*n+q));
					*(u+q*n+p) = *(a+q*n+p) - sum;

					if (q == p) {
						*(l+q*n+q) = 1;
					}
				}
			}*/

			#pragma omp parallel shared(l,u,nThreads) private(i,k,j)
			{
				for(int k=0;k<n;k++)
				{
					*(l+k*n+k)=1;
					#pragma omp for schedule(static,chunk)
					for(int j=k;j<n;j++)
					{
						ld sum=0;
						for(int s=0;s<=k-1;s++)
						{
							sum += ((*(l+k*n+s)) * (*(u+s*n+j)));
						}
						//upr[k][j] = a[k][j]-sum;
						*(u+k*n+j) = (*(a+k*n+j) - sum);
					}
					#pragma omp for schedule(static,chunk)
					for(int i=k+1;i<n;i++)
					{
						ld sum=0;
						for(int s=0;s<=k-1;s++)
						{
							sum += ((*(l+i*n+s)) * (*(u+s*n+k)));
						}
						//lwr[i][k]=(a[i][k]-sum)/upr[k][k];
						*(l+i*n+k) = (*(a+i*n+k) - sum)/(*(u+k*n+k));
					}
				}	
			}

			float t2=omp_get_wtime()*1000;
			float time_taken = t2-t1;
			cout << "Order of matrix: " << n << "; No. of threads: " << nThreads << " --> " << time_taken << "ms" << endl;
		
			/*if(nThreads==1){
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
				{	
					cout<<*(a+i*n+j)<<"   ";
				}
				cout<<endl;
			}
			cout<<endl<<endl	
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
				{	
					cout<<*(u+i*n+j)<<"   ";
				}
				cout<<endl;
			}
			cout<<endl<<endl;
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
				{	
					cout<<*(l+i*n+j)<<"   ";
				}
				cout<<endl;
			}
			}*/
	   	}

	   	cout<< endl;

   	}
   	
	return 0;
}