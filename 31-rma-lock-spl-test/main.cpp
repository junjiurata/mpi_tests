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

void numRanks(int& numRank, int& myRank, MPI_Comm comm) {
    MPI_Comm_size(comm, &numRank);
    MPI_Comm_rank(comm, &myRank);
}

//// Save-Allreduce ////
int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, procs, rank2, procs2;
    int *fini;
    int target = 1;
    int iter = 0;
    int namelen, version, subversion;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm new_comm;
    MPI_Win win;        // 宣言：window

    MPI_Get_processor_name(processor_name, &namelen);
    MPI_Get_version(&version, &subversion);

//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    numRanks(procs, rank, MPI_COMM_WORLD);

    int key = rank; //子コミュニケータにおけるrankの割り当て
    int color = (rank != target);
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &new_comm);  // コミュニケータの分割宣言
//    MPI_Comm_rank(new_comm, &rank2);    // 分割後のランクの設定
//    MPI_Comm_size(new_comm, &procs2);
    numRanks(procs2, rank2, new_comm);

    cout << " I'm rank " << rank << " of " << procs << " / " << rank2 << " of " << procs2 <<
            " on " << processor_name << " running MPI " << version << "." << subversion << endl;

    // for transmitting between process (make "window")
    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &fini);
//    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &iter);
    MPI_Win_create(fini, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win); // finishをwindowに置く; extentはsizeof(int)でもいい．
//    MPI_Win_create(iter, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win2);
    *fini = 0;
    int sumtest = rank;
    int sumresl = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    
    for(int i=0; ; i++){
        MPI_Barrier(MPI_COMM_WORLD);    
        *fini = 0;
//        j = 0;
        cout << "[" << i << "] Rank " << rank << " initial fin: " << *fini << " j " << iter << endl;
        if(rank == 0){
             for(int p=0; p < 100000000; p++){
             }
            *fini = 1;
//            cout << " Rank " << rank << " (LONG) finish " << *fini << " ! " << endl;
            MPI_Win_lock(MPI_LOCK_SHARED, target, 0,  win);  // memoryはrank=1
             MPI_Put(fini, 1, MPI_INT, target, 0, 1, MPI_INT, win);  // finish=1をwinにput.していたが，同じプロセスなので特に必要ない．
            MPI_Win_unlock(target, win);
        } else if(rank == 2){
            for(; ; iter++){   // 繰り返し回数の上限を指定することも可能
                
                MPI_Win_lock(MPI_LOCK_SHARED, target, 0,  win);      
                MPI_Get(fini, 1, MPI_INT, target, 0, 1, MPI_INT, win);   // procs-1のプロセスからfinishをGet．
                MPI_Win_unlock(1, win);
                if(*fini==1) break;
        //            if(j % 1000 == 0){
        //                cout << j << " (" << rank << ", " << *fini << ")"<< endl;
        //            }
            }
        }
        if(rank != target){
            MPI_Allreduce(&sumtest, &sumresl, 1,MPI_INT, MPI_SUM, new_comm);
        }
        MPI_Barrier(MPI_COMM_WORLD);    
        MPI_Bcast(&iter, 1, MPI_INT, 2, MPI_COMM_WORLD);
        cout << "[" << i << "] Rank " << rank << " Terminate fin " << *fini  << " j " << iter << " allsum " << sumresl << endl;
        if(iter > 300000) break;
    }

    cout << "# Rank " << rank << " Terminate fin " << *fini  << " j " << iter << " allsum " << sumresl << endl;
        
    MPI_Win_free(&win);
    MPI_Free_mem(fini);
//    MPI_Win_free(&win2);
//    MPI_Free_mem(iter);
    MPI_Comm_free(&new_comm);
    MPI_Finalize();
}

