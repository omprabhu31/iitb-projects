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
			//int n=1200;
			
			float *u = (float *)malloc(n * n * sizeof(float));
			float *l = (float *)malloc(n * n * sizeof(float));
			float *a = (float *)malloc(n * n * sizeof(float));

			srand((unsigned)time(NULL));
			float randomno;
			#pragma omp parallel for default(none), private(randomno), shared(a,n,u)
			for(int i=0;i<n;i++)
			{
				for(int j=0;j<n;j++)
				{	
				float randomno = (((float) rand()) / (float) RAND_MAX);
				*(a+i*n+j)=randomno;
				//*(a+i*n+j)=1;

				if(i==j){*(u+i*n+j)=1;}
				}
			}

			
			float t1=omp_get_wtime()*1000;
			
			float sum;
			int j,k;
			#pragma omp parallel for default(none), shared(n,a,j,k,l,u)
			for(int i=0; i<n;i++)
			{
				#pragma omp parallel for default(none), shared(k,n,a,l,i,u), private(sum)
				for(j=i;j<n;j++)
				{
					sum=0;
					k=0;
					for(k=0;k<i;k++)
					{
						sum+= ((*(l+j*n+k))*(*(u+k*n+i)));
					}
					*(l+j*n+i) = (*(a+j*n+i))-sum;
				}

				#pragma omp parallel for default(none), shared(k,n,a,i,u,l), private(sum)
				for(j=i;j<n;j++)
				{
					sum=0;
					for(k=0;k<i;k++)
					{
						sum+= ((*(l+i*n+k))*(*(u+k*n+j)));
					}
					*(u+i*n+j) = ((*(a+i*n+j))-sum)/(*(l+i*n+i));
				}
			}
			
			float t2=omp_get_wtime()*1000;
			//cout << n << ", " << threads << ": " << (t2-t1) << "ms" << endl;
			cout << "Order of matrix: " << n << "; No. of threads: " << threads << " --> " << (t2-t1) << "ms" << endl;
			
		    /*if(threads==1)
		    {
				for(int i=0;i<n;i++)
				{
					for(int j=0;j<n;j++)
					{	
						cout<<*(u+i*n+j)<<"   ";
					}
					cout<<endl;
				}
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
	    cout << endl;
    }
}