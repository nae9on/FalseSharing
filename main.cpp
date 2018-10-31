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

const double xOrigin(-1), yOrigin(-0.5), zOrigin(-0.5), delta(0.01);

void case1(unsigned nx, unsigned ny, unsigned nz, unsigned NUM_THREADS) {

	// Allocate dynamic memory of type double and size nx*ny*nz
	double* gridData = new double[nx * ny * nz]();

#pragma omp parallel default(none) num_threads(NUM_THREADS)\
shared(nx, ny, nz, gridData, std::cout)
	{
		unsigned id = omp_get_thread_num(); // This is a run-time library routine.
		unsigned total = omp_get_num_threads();
		//if (id==0) printf("Total threads executing the parallel region = %d\n", total);

#pragma omp for schedule(static)
		for (unsigned kk = 0; kk < nz; kk++)
			for (unsigned jj = 0; jj < ny; jj++)
				for (unsigned ii = 0; ii < nx; ii++) {

					unsigned long int idx = ii + nx * jj + nx * ny * kk;

					double xx = xOrigin+ii*delta;
					double yy = yOrigin+jj*delta;
					double zz = zOrigin+kk*delta;
					double dist2 = xx*xx+yy*yy+zz*zz;

					double value = 0;

					unsigned nStencil = 10;
					for (unsigned itr = 1; itr <= nStencil; itr++) {
						value += exp(-dist2*itr*itr);
					}

					gridData[idx] = value;
				} // End of nested for

	} // End of pragma
	delete[] gridData;
}

int main() {
	printf("Max no of threads = %d\n\n", omp_get_max_threads());

	mymisc::Stopwatch mainWatch, watch;
	mainWatch.start();

	watch.start();
	for (unsigned itr = 1; itr <= 100; itr++) {
		case1(201, 101, 101, 1);
	}
	watch.stop();
	watch.showTime("case1");

	mainWatch.stop();
	mainWatch.showTime("End main");
	return 0;
}
