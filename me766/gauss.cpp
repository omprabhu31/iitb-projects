#include<omp.h>
#include<math.h>
#include<iostream>

using namespace std;
//omp_get_wtime()*1000;

int main()
{
	for (int n = 100; n<=2000; n+=100) {

		for(int threads=1;threads<=8;threads*=2) {
			omp_set_num_threads(threads);
			//int n=4;
			
			float *a = (float *)malloc(n * n * sizeof(float));
			float *b = (float *)malloc(n * n * sizeof(float));
			float *c = (float *)malloc(n * n * sizeof(float));

			srand((unsigned)time(NULL));
			float randomno;
			#pragma omp parallel for default(none), private(randomno), shared(c,n)
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
				{	
				float randomno = (((float) rand()) / (float) RAND_MAX);
				*(c+i*n+j)=randomno;
				//*(c+i*n+j)=1;
				}
			}

			
			float t1=omp_get_wtime()*1000;
			
			float divid;
			#pragma omp parallel for default(none), shared(n,c)
			for(int k=0; k<n-1;k++)
			{
				#pragma omp parallel for default(none), shared(k,n,c), private(divid)
				for(int i=k+1;i<n;i++)
				{
					float divid=(*(c+i*n+k))/(*(c+k*n+k));
					for(int j=0;j<n;j++)
					{
						*(c+i*n+j)=(float)((*(c+i*n+j))-(divid*(*(c+k*n+j))));
					}
					*(c+i*n+k)=0;
				}
			}
			
			float t2=omp_get_wtime()*1000;
			//cout << n << ", " << threads << ": " << (t2-t1) << "ms" << endl;
			cout << "Order of matrix: " << n << "; No. of threads: " << threads << " --> " << (t2-t1) << "ms" << endl;	
			
			/*if(threads==1){
				for(int i=0;i<n;i++)
				{
					for(int j=0;j<n;j++)
					{	
						cout<<*(c+i*n+j)<<"   ";
					}
					cout<<endl;
				}
			}*/
	    }
	}
}