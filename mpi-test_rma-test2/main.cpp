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

using namespace std;
        
// Save-Allreduce ////
int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    MPI_Win win;        // 宣言：window
    int rank, procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm new_comm;  // 宣言：split用のComm

//    const int SIZE = 10;
//    std::vector<int> dat(SIZE);
    
    // set color
    int key = 0;
    int color = (rank != (procs-1));
    int rank2;          // 宣言：split用のrank
    
    // for transmitting between process (make "window")
    int *finish = 0;
//    int *fin = 0;
    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &finish);
//    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &fin);
    MPI_Win_create(finish, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win); // finishをwindowに置く
    cout << "sizeof(finish) " << sizeof(*finish) <<  " " << rank << " " << win << endl;
    
//    MPI_Comm_split(MPI_COMM_WORLD, color, key, &new_comm);  // コミュニケータの分割宣言
//    MPI_Comm_rank(new_comm, &rank2);    // 分割後のランクの設定
        int j = 0;


//    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == (procs-1)){
        cout << " initial fin: " << *finish << " Rank " << rank << endl;
        do {
            j++;
        } while((j<100));//&&
        *finish = 1;
        cout << endl << "  " << rank << " finish! " << *finish << " j " << j<< endl;
//        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, MPI_MODE_NOCHECK,  win);
//        MPI_Put(finish, 1, MPI_INT, 0, 0, 1, MPI_INT, win);  // finish=1をwinにput.していたが，同じプロセスなので特に必要ない．
////      MPI_Get(finish, 1, MPI_INT, procs-1, 0, 1, MPI_INT, win);  // procs-1のプロセスからfinishをGet．
//        MPI_Win_unlock(0, win);
    } //else {
    if(rank != (procs-1)){
        cout << " initial fin: " << *finish << " Rank " << rank << endl;
        do {
            j++;
            MPI_Win_lock(MPI_LOCK_EXCLUSIVE, procs-1, 0,  win);
            MPI_Get(finish, 1, MPI_INT, procs-1, 0, 1, MPI_INT, win);  // procs-1のプロセスからfinishをGet．
            MPI_Win_flush(procs-1, win);
            MPI_Win_unlock(procs-1, win);
            if(j % 10000 == 0){
                cout << j/10000 << " ";
            }
        } while((*finish != 1));//&&(j<100000000)
        cout << " Terminate fin " << *finish << " Rank " << rank << " Itr: "<< j << endl;
    } 

    
//    cout << "Rank: " << rank << " Size: " << procs << 
//            " color: " << color << " Key: " << key << 
//            " COMM: " << &new_comm << " Rank2: " << rank2 << endl;
    MPI_Barrier(MPI_COMM_WORLD);
    cout << "RE Terminate fin " << *finish << " Rank " << rank << " Itr: "<< j << endl;
//    cout << "All process finished! " << rank << endl;

    MPI_Win_free(&win);
    MPI_Free_mem(finish);
//    MPI_Free_mem(fin);
    MPI_Finalize();
}
