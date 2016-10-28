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
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//// Save-Allreduce ////
int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, procs;
    int size, namelen, version, subversion;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    MPI_Get_processor_name(processor_name, &namelen);
    MPI_Get_version(&version, &subversion);

    cout << " I'm rank " << rank << " of " << procs << " on " << processor_name << " running MPI " << version << "." << subversion << endl;

    srand(1);
    int a,b,c,d,e;
    
    a = rand();
    b = rand();
    c = rand();
    d = rand();
    e = rand();
    
    cout << "(" << rank << ")" << a << " " << b << " " << c << " " << d << " " << e << endl;
    
    MPI_Finalize();
}

