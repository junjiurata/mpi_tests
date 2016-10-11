/* 
 * File:   main.cpp
 * Author: urata based on http://www.linux-mag.com/id/1793/
 * groupとコミュニケータの関係がわからないので，このプログラムが判読しきれず．
 * Created on August 8, 2016, 6:27 PM
 */

#include <mpi.h>
#include <vector>

using namespace std;

int main(int argc, char** argv){

    int rank, size, namelen, version, subversion, from_rank, to_rank;
    int *local_secret, *remote_secret;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Win win;
    MPI_Group whole_group, from_group, to_group;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &namelen);
    MPI_Get_version(&version, &subversion);

    printf("Hello world! I’m rank %d of %d on %s running MPI %d.%d\n", rank, size, processor_name, version, subversion);
    MPI_Comm_group(MPI_COMM_WORLD, &whole_group);

    if(rank == 0){
        to_rank = 1;
        from_rank = 0;
    } else {
        to_rank = 0;
        from_rank = 1;
    }
    to_rank = ( rank == (size - 1) ? 0 : rank + 1 );
    from_rank = ( rank ? rank - 1 : size - 1 );
    cout << "rank " << rank << " to " << to_rank << " from " << from_rank << " whole " << &whole_group << endl;
    MPI_Group_incl(whole_group, 1, &to_rank, &to_group);
    MPI_Group_incl(whole_group, 1, &from_rank, &from_group);

    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &local_secret);
    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &remote_secret);
    MPI_Win_create(local_secret, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

//    if(rank == 0){
        *local_secret = rank;
        MPI_Win_post(to_group, MPI_MODE_NOPUT, win);
//    }
    printf("Process %d has the following secret: %d\n", rank, *local_secret);

//    if(rank != 0){
        
        MPI_Win_start(from_group, 0, win);
        MPI_Get(remote_secret, 1, MPI_INT, from_rank, 0, 1, MPI_INT, win);
        MPI_Win_complete(win);
        MPI_Win_wait(win);
//    }
    printf("Process %d obtained the following secret: %d\n", rank, *remote_secret);

    MPI_Win_free(&win);
    MPI_Free_mem(local_secret);
    MPI_Free_mem(remote_secret);
    MPI_Group_free(&to_group);
    MPI_Group_free(&from_group);
    MPI_Finalize();
}

//// Save-Allreduce ////
//int main(int argc, char** argv){
//    MPI_Init(&argc, &argv);
//    MPI_Win win;        // 宣言：window
//    int rank, procs;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &procs);
//    MPI_Comm new_comm;  // 宣言：split用のComm
//    MPI_Group sender, receiver;
//
////    const int SIZE = 10;
////    std::vector<int> dat(SIZE);
//    
//    // set color
//    int key = 0;
//    int color = (rank != (procs-1));
//    int rank2;          // 宣言：split用のrank
//    
//    // for transmitting between process (make "window")
//    int *finish = 0;
////    int *fin = 0;
//    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &finish);
////    MPI_Alloc_mem(sizeof(int), MPI_INFO_NULL, &fin);
//    MPI_Win_create(finish, 1, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win); // finishをwindowに置く
//    cout << "sizeof(finish) " << sizeof(*finish) <<  " " << rank << " " << win << endl;
//    
////    MPI_Comm_split(MPI_COMM_WORLD, color, key, &new_comm);  // コミュニケータの分割宣言
////    MPI_Comm_rank(new_comm, &rank2);    // 分割後のランクの設定
//        int j = 0;
//        cout << " initial fin: " << *finish << " Rank " << rank << endl;
//
////    MPI_Barrier(MPI_COMM_WORLD);
//    if(rank == (procs-1)){
////        std::vector<int> dat2(100);
////        int i = 0;
////        for(std::vector<int>::iterator itr = dat2.begin(); itr != dat2.end(); ++itr){
////            *itr = i;
////            i++;
////            //cout << i ;
////        }
//        *finish = 1;
////        cout << endl << "  " << rank << " (LONG) finish! " << dat2.size() << endl;
//        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, MPI_MODE_NOCHECK,  win);
//        MPI_Put(finish, 1, MPI_INT, 0, 0, 1, MPI_INT, win);  // finish=1をwinにput.していたが，同じプロセスなので特に必要ない．
////      MPI_Get(finish, 1, MPI_INT, procs-1, 0, 1, MPI_INT, win);  // procs-1のプロセスからfinishをGet．
//        MPI_Win_unlock(0, win);
//    } //else {
//    if(rank != (procs-1)){
//        do {
//            j++;
////            MPI_Win_lock(MPI_LOCK_EXCLUSIVE, procs-1, 0,  win);
////            MPI_Get(finish, 1, MPI_INT, procs-1, 0, 1, MPI_INT, win);  // procs-1のプロセスからfinishをGet．
////            MPI_Win_unlock(procs-1, win);
////            if(j % 10000 == 0){
////                cout << j/10000 << " ";
////            }
//        } while((*finish != 1)&&(j<100000000));//
//        cout << " Terminate fin " << *finish << " Rank " << rank << " Itr: "<< j << endl;
//    } 
//
//    
////    cout << "Rank: " << rank << " Size: " << procs << 
////            " color: " << color << " Key: " << key << 
////            " COMM: " << &new_comm << " Rank2: " << rank2 << endl;
//    MPI_Barrier(MPI_COMM_WORLD);
//    cout << "RE Terminate fin " << *finish << " Rank " << rank << " Itr: "<< j << endl;
////    cout << "All process finished! " << rank << endl;
//
//    MPI_Win_free(&win);
//    MPI_Free_mem(finish);
////    MPI_Free_mem(fin);
//    MPI_Finalize();
//}
