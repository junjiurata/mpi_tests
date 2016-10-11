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

using namespace std;

//// Save-Allreduce ////
int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, procs;
    int *fini = 0;
    int *finr;
    int SIZE;

    MPI_Win win;        // 宣言：window

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    
    // for transmitting between process (make "window")
    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &fini);
    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &finr);
    MPI_Win_create(fini, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win); // finishをwindowに置く; extentはsizeof(int)でもいい．
    MPI_Barrier(MPI_COMM_WORLD);    

    //*finish = 0;
//    if(rank == (procs-1)){
//        cout << endl << " initial fin: " << *finish << " Rank " << rank << endl;
        *fini = rank * 100;
        cout << "  " << rank << " (LONG) finish " << *fini << " ! " << endl;
        // MPI_Put(&finish, 1, MPI_INT, 1, 0, 1, MPI_INT, win);  // finish=1をwinにput.していたが，同じプロセスなので特に必要ない．
//    }
    
    //*finr = 0;
    MPI_Barrier(MPI_COMM_WORLD);
//     if(rank != (procs-1)){
       MPI_Win_fence((MPI_MODE_NOPUT | MPI_MODE_NOPRECEDE), win);      // MPI_Putを許すためのfence(同期)
       // int j = 0;
//        cout << " initial fin: " << *finr << " Rank " << rank << endl;
        MPI_Get(finr, 1, MPI_INT, 0, 0, 1, MPI_INT, win);   // procs-1のプロセスからfinishをGet．
        MPI_Win_fence(MPI_MODE_NOSUCCEED, win);    // あると終わらない(-np 4のとき)が，ないとエラー出る．全ての片方向通信が終わらないから？    
//    } 
        cout  << " Terminate fin " << *finr <<  " Rank " << rank << endl;
    
    MPI_Win_free(&win);
    MPI_Free_mem(fini);
    MPI_Free_mem(finr);
    MPI_Finalize();
}

