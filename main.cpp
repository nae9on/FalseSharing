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

#include"stopwatch.h"

const double xOrigin(0), yOrigin(0), zOrigin(0), delta(1);

void calc_exp(unsigned col, unsigned row, unsigned NUM_THREADS,
		unsigned nStencil) {

	// Allocate dynamic memory of type double and size col*row
	double* gridData = new double[col * row]();
	printf("Total size of array = %ld\n", col * row);

#pragma omp parallel default(none) num_threads(NUM_THREADS)\
shared(col, row, gridData, std::cout, nStencil)
	{
		unsigned id = omp_get_thread_num(); // This is a run-time library routine.
		unsigned total = omp_get_num_threads();
		//if (id==0) printf("Total threads executing the parallel region = %d\n", total);

#pragma omp for schedule(static,1)
		for (unsigned jj = 0; jj < row; jj++) {
			//printf("Row %d executed by thread %d\n", jj, omp_get_thread_num());
			for (unsigned ii = 0; ii < col; ii++) {

				unsigned long int idx = ii + col * jj;

				double xx = xOrigin + ii * delta;
				double yy = yOrigin + jj * delta;
				double dist2 = xx * xx + yy * yy;

				double value = 0;

				for (unsigned itr = 1; itr <= nStencil; itr++) {
					value += exp(-dist2 * itr * itr);
				}

				gridData[idx] = value;
			} // End of nested for
		}

	} // End of pragma
	delete[] gridData;
}

int main(int argc, char *argv[]) {
	char *p;
	unsigned CACHE_LINE_SIZE, M, MAX_THREADS;

	if (argc < 2) {
		printf("The CACHE_LINE_SIZE argument is missing\n");
		exit(0);
	} else {
		CACHE_LINE_SIZE = strtol(argv[1], &p, 10);
		M = CACHE_LINE_SIZE / sizeof(double);
		printf("CACHE_LINE_SIZE (in bytes) = %d\n", CACHE_LINE_SIZE);
		printf("CACHE_LINE_SIZE / sizeof(double) = %d\n\n", M);
	}

	MAX_THREADS = omp_get_max_threads();
	printf("Max no of threads = %d\n\n", MAX_THREADS);

	mymisc::Stopwatch mainWatch, watch;
	mainWatch.start();

	// Best case
	watch.start();
	calc_exp(M, MAX_THREADS*1000, MAX_THREADS, 10000);
	watch.stop();
	watch.showTime("Best");

	// Worst case
	watch.start();
	calc_exp(M/2, 2*MAX_THREADS*1000, MAX_THREADS, 10000);
	watch.stop();
	watch.showTime("Worst");

	mainWatch.stop();
	mainWatch.showTime("End main");
	return 0;
}
