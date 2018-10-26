/*
 * main.cpp
 *
 *  Created on: Oct 26, 2018
 *      Author: akadar
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h> // for including run-time library routines
#include <cmath>
#include <time.h>

#define BILLION  1000000000L;

int main()
{
struct timespec start, stop;
double accum;

if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
	perror( "clock gettime" );
	exit( EXIT_FAILURE );
}

std::cout<<"In the main function for illustrating false_sharing\n";
printf("Max no of threads = %d\n",omp_get_max_threads());

unsigned int nx=1000, ny=1000, nz=100;

// Allocate dynamic memory of type double and size nx*ny*nz
double* gridData = new double[nx * ny * nz] ();

#pragma omp parallel default(none) \
shared(nx, ny, nz, gridData, std::cout)
	{
	int id = omp_get_thread_num(); // This is a run-time library routine.
	int total = omp_get_num_threads();
	if (id==0) {
		printf("I am the master thread with id = %d and total threads = %d\n", id, total);
	}

#pragma omp for schedule(dynamic,1)
		for (unsigned int kk = 0; kk < nz; kk++)
			for (unsigned int jj = 0; jj < ny; jj++)
				for (unsigned int ii = 0; ii < nx; ii++) {
					int idx = ii + nx * jj + nx * ny * kk;

					double value = 0;

					unsigned int nStencil = 10;
					for (unsigned int itr = 0; itr < nStencil; itr++) {
						value += exp(-1);
					}

					gridData[idx] = value;
				} // End of nested for

	} // End of pragma

delete[] gridData;

if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
	perror( "clock gettime" );
	exit( EXIT_FAILURE );
}

accum = (stop.tv_sec - start.tv_sec)+(stop.tv_nsec - start.tv_nsec)/BILLION;

int Hours = accum/3600;
int Minutes = (accum-Hours*3600)/60;
int Seconds = accum - Hours*3600 - Minutes*60;
std::cout<<"\nElapsed time = "<<Hours<<":"<<Minutes<<":"<<Seconds<<"\n";

std::cout<<"End main\n";
return 0;
}
