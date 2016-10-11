/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: urata
 *
 * Created on August 8, 2016, 6:27 PM
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

double myrand(void){
    return (double)rand()/(double)RAND_MAX;
}

double calc_pi(int seed, int trial){
    srand(seed);
    int n = 0;
    for(int i=0; i<trial; i++){
        double x = myrand();
        double y = myrand();
        if(x*x + y*y < 1.0){
            n++;
        }
    }
    return 4.0*(double)n/(double)trial;
}

int main(int argc, char** argv) {
    int rank, size;
    int iteration = 10000000;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double pi = calc_pi(rank+1,iteration);
    printf("[%d/%d] %f \n", rank, size, pi);
    
    MPI_Barrier(MPI_COMM_WORLD);
    double sum = 0;
//    MPI_Allreduce(&pi, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    int pres = 0;
    MPI_Reduce(&pi, &sum, 1, MPI_DOUBLE, MPI_SUM, pres, MPI_COMM_WORLD);
    sum = sum / (double)size;
    if(rank==pres){
        printf("[%d]average = %f\n", rank, sum);
    }
    
    MPI_Finalize();
}
