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

//#define SPLIT
using namespace std;

int main(int argc, char** argv)

{
  MPI_Init(&argc, &argv);
  int rank, size, namelen, version, subversion, *a, *b;

  MPI_Win win;
//  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
//  MPI_Get_processor_name(processor_name, &namelen);
//  MPI_Get_version(&version, &subversion);
//
//  cout << " I'm rank " << rank << " of " << size << " on " << processor_name << " running MPI " << version << "." << subversion << endl;

  size = 1;
  MPI_Alloc_mem(sizeof(int)*size, MPI_INFO_NULL, &a);
  MPI_Alloc_mem(sizeof(int)*size, MPI_INFO_NULL, &b);
  MPI_Win_create(a, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

  //if(rank == 0){
      //int i = 0;
//      for (int i = 0; i < size; i++){
        //a[i] = i + 100 * rank;
          *a = rank*100;
//      }
  //}

  printf("Process %d has the following: ", rank);
  for (int i = 0; i < size; i++){
    printf(" %d", *a);
  }
  printf("\n");

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Win_fence((MPI_MODE_NOPUT | MPI_MODE_NOPRECEDE), win);
//  if(rank != 0){
      //int i = 0;
//      for (int i = 0; i < size; i++){
        MPI_Get(b, 1, MPI_INT, 1, 0, 1, MPI_INT, win);
//      }
//  }
  MPI_Win_fence(MPI_MODE_NOSUCCEED, win);
  
  printf("B: Process %d obtained the following: ", rank);
  for (int i = 0; i < size; i++){
    printf(" %d ", *b);
  }
  printf("\n");

//  MPI_Barrier(MPI_COMM_WORLD);
//  printf("A: Process %d has the following: ", rank);
//  for (int i = 0; i < size; i++){
//    printf(" %d", a[i]);
//  }
//  printf("\n");
  
  MPI_Win_free(&win);
  MPI_Free_mem(a);
  MPI_Free_mem(b);
  MPI_Finalize();
}

//// Save-Allreduce ////
//int main(int argc, char** argv){
//    MPI_Init(&argc, &argv);
//    int rank, procs;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &procs);
//    MPI_Aint lb, extent;
//#if defined(SPLIT)
//    MPI_Comm new_comm;  // 宣言：split用のComm
//#endif
//
//    const int SIZE = 10;
//    //std::vector<int> dat(SIZE);
//    
//    // set color
//    int key = 0;
//    int color = (rank != (procs-1));
//    int rank2;          // 宣言：split用のrank
//    
//    // for transmitting between process (make "window")
//    int finish = 0;
//    MPI_Win win;        // 宣言：window
//    MPI_Type_get_extent(MPI_INT, &lb, &extent);
//    cout << "lb " << lb << " extent " << extent << endl;
//    MPI_Win_create(&finish, 1*extent, extent, MPI_INFO_NULL, MPI_COMM_WORLD, &win); // finishをwindowに置く; extentはsizeof(int)でもいい．
//    cout << "sizeof(finish) " << sizeof(finish) << " " << sizeof(int) << " " << rank << " " << win << " np " << procs << endl;
//#if defined(SPLIT)
//    MPI_Comm_split(MPI_COMM_WORLD, color, key, &new_comm);  // コミュニケータの分割宣言
//    MPI_Comm_rank(new_comm, &rank2);    // 分割後のランクの設定
//#endif
//
//    MPI_Win_fence(0, win);      // MPI_Putを許すためのfence(同期)
//    
//    if(rank == (procs-1)){
//        cout << " initial fin: " << finish << " Rank " << rank << endl;
//        std::vector<int> dat2(SIZE*2000);
//        int i = 0;
//        for(std::vector<int>::iterator itr = dat2.begin(); itr != dat2.end(); ++itr){
//            *itr = i;
//            i++;
//        }
//        finish = 1;
//        cout << endl << "  " << rank << " (LONG) finish " << finish << " ! " << dat2.size() << endl;
//         MPI_Put(&finish, 1, MPI_INT, 1, 0, 1, MPI_INT, win);  // finish=1をwinにput.していたが，同じプロセスなので特に必要ない．
//    }
//    
//    MPI_Barrier(MPI_COMM_WORLD);
//    if(rank != (procs-1)){
//        int j = 0;
//        cout << " initial fin: " << finish << " Rank " << rank << endl;
//    //    do {
//    //        j++;
//            MPI_Get(&finish, 1, MPI_INT, procs-1, 0, 1, MPI_INT, win);  // procs-1のプロセスからfinishをGet．
//    //        if(j % 30000 == 0){
//    //            cout << j/10000 << " " << finish << " ";
//    //       }
//    //    } while((finish != 1)&&(j<2000000));//
//        cout << endl << " Terminate fin " << finish <<  " Rank " << rank << " Itr: "<< j << endl;
//    } 
//    
////    cout << "Rank: " << rank << " Size: " << procs <<  " color: " << color << " Key: " << key << endl;
//#if defined(SPLIT)
//    cout << " COMM: " << &new_comm << " Rank2: " << rank2 << endl;
//#endif
//    
//    MPI_Barrier(MPI_COMM_WORLD);
//    cout << "All process finished! " << rank << endl;
//
//    MPI_Win_fence(0, win);    // あると終わらない(-np 4のとき)が，ないとエラー出る．全ての片方向通信が終わらないから？    
//    MPI_Win_free(&win);
//    MPI_Finalize();
//}
