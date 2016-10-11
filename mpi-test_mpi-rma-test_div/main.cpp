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

//    MPI_Comm_split(MPI_COMM_WORLD, color, key, &new_comm);  // コミュニケータの分割宣言
//    MPI_Comm_rank(new_comm, &rank2);    // 分割後のランクの設定
    
    // for transmitting between process (make "window")
    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &fini);
    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &finr);
    MPI_Win_create(fini, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win); // finishをwindowに置く; extentはsizeof(int)でもいい．
    MPI_Barrier(MPI_COMM_WORLD);    

//    if(rank == (procs-1)){
//        cout << endl << " initial fin: " << *finish << " Rank " << rank << endl;
         for(int j = 0; j < 10000000; j++){
             SIZE++;
         }
        *fini = rank * 100;
        cout << "  " << rank << " (LONG) finish " << *fini << " ! " << endl;
        // MPI_Put(&finish, 1, MPI_INT, 1, 0, 1, MPI_INT, win);  // finish=1をwinにput.していたが，同じプロセスなので特に必要ない．
//    }
    
//    MPI_Barrier(MPI_COMM_WORLD);
    if(rank != (procs-1)){
       // int j = 0;
//        cout << " initial fin: " << *finr << " Rank " << rank << endl;
        MPI_Win_lock(MPI_LOCK_SHARED, procs-1, 0,  win);      
//        MPI_Win_fence((MPI_MODE_NOPUT | MPI_MODE_NOPRECEDE), win);   // MPI_Putを許すためのfence(同期)
        MPI_Get(finr, 1, MPI_INT, procs-1, 0, 1, MPI_INT, win);   // procs-1のプロセスからfinishをGet．
        MPI_Win_unlock(procs-1, win);
//        MPI_Win_fence(MPI_MODE_NOSUCCEED, win); 
    } 
        cout  << " Terminate fin " << *finr <<  " Rank " << rank << endl;
    
    MPI_Win_free(&win);
    MPI_Free_mem(fini);
    MPI_Free_mem(finr);
    MPI_Finalize();
}

