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

//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//#include <fstream>

#include <mpi.h>
#include <vector>
using namespace std;

//#define VEC_DEF
//#define INT
//#define VEC_DYN
//#define VEC_DYN2
#define VEC_DYN3

// Save-Allreduce ////
int main(int argc, char** argv){
    
    MPI_Init(&argc, &argv);
    int rank, procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    const int SIZE = 10;
    
#if defined(VEC_DEF)
    std::vector<int> dat(SIZE);
    if(rank==0){
        for(int i = 0; i < SIZE; i++){
        //    dat[i] = i;
            dat.push_back(i);
        }
    }
    MPI_Bcast(&dat[0], dat.size(), MPI_INT, 0, MPI_COMM_WORLD);
    cout << "vector_int " << dat[1] << " " << dat[3] << " " << rank << endl;
#endif
    
#if defined(VEC_DYN)
    std::vector<int> datd;
    if(rank==0){
        for(int i = 0; i < SIZE; i++){
            //datd[i] = i;
            datd.push_back(i);
        }
    }
    cout << "vector_dyn_before " << datd[1] << " " << datd[3] << " " << rank << endl;
    MPI_Bcast(&datd[0], SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    cout << "vector_dyn " << datd[1] << " " << datd[3] << " " << rank << endl;
#endif

#if defined(VEC_DYN2)
    std::vector<int> datd;
    if(rank==0){
        for(int i = 0; i < SIZE; i++){
            //datd[i] = i;
            datd.push_back(i);
        }
    }
    datd.resize(SIZE);
    cout << "vector_dyn2_before " << datd[1] << " " << datd[3] << " " << rank << endl;
    MPI_Bcast(&datd[0], datd.size(), MPI_INT, 0, MPI_COMM_WORLD);
    cout << "vector_dyn2 " << datd[1] << " " << datd[3] << " " << rank << endl;
#endif

#if defined(VEC_DYN3)
    std::vector<int> datd;
    if(rank==0){
        for(int i = 0; i < SIZE; i++){
            //datd[i] = i;
            datd.push_back(i);
        }
    }
    int size0 = datd.size();
    MPI_Bcast(&size0, 1, MPI_INT, 0, MPI_COMM_WORLD);
    cout << size0 << " " << rank << endl;
    datd.resize(size0);

    cout << "vector_dyn2_before " << datd[1] << " " << datd[3] << " " << rank << endl;
    MPI_Bcast(&datd[0], datd.size(), MPI_INT, 0, MPI_COMM_WORLD);
    cout << "vector_dyn2 " << datd[1] << " " << datd[3] << " " << rank << endl;
#endif

#if defined(INT)
    int dati[SIZE];
    if(rank==0){
        for(int i = 0; i < SIZE; i++){
            dati[i] = i;
        }
    }
    MPI_Bcast(&dati[0], SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    cout << "int " << dati[1] << " " << dati[3] << " " << rank << endl;
#endif    
    
    MPI_Finalize();
}

//// Save ////

//    
//    value += 1.0;
//    
//    if(rank==0){
//            value += 10.1;
//    }
//    std::cout << "B " << value << " " << rank << std::endl;
//
//    std::vector<double> buf((int)(SIZE*procs));
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    std::cout << " data.size " << data.size() << std::endl;
////    MPI_Gather(data.data(), data.size(), MPI_DOUBLE, buf.data(), data.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    MPI_Bcast(data.data(), data.size(), MPI_DOUBLE, 1, MPI_COMM_WORLD);
//    MPI_Barrier(MPI_COMM_WORLD);
//
//    for(int i=0; i < SIZE; i++){
////            std::cout << " " << buf[i] << " " << rank << std::endl;
//            std::cout << " " << data[i] << " " << rank << std::endl;
//        }
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    std::cout << " test " << rank << std::endl;
//
//    int isAccepted = 0;
//    if(rank == 0){
//        isAccepted = 1;
//    }
//    MPI_Barrier(MPI_COMM_WORLD);
//    std::cout << " " << isAccepted << " " << rank << std::endl;
//    MPI_Bcast(&isAccepted, 1, MPI_INT, 0, MPI_COMM_WORLD);
//    std::cout << " " << isAccepted << " " << rank << std::endl;
//   

//    vector<int> dat(3);
//    dat[0] = 1;
//    dat[1] = 2;
//    dat[2] = 3;
//    vector<int> data_c;
//    data_c.push_back(-1);
    
//    cout << " " << dat.size() << " _c " << data_c.size() << endl;
//    data_c.resize(dat.size());
//    cout << " " << dat.size() << " _c " << data_c.size() << endl;
//    
//    //back_insert_iterator<vector<int> iit_data_c(data_c);
//    
//    copy(dat.begin(), dat.end(), data_c.begin());
////    copy(dat.begin(), dat.end(), iit_data_c);
//    for(int i; i<data_c.size(); i++){
//        cout << data_c[i] << " " ;
//    }


//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Bcast(&value, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//
//    std::cout << "A " << value << " " << rank << std::endl;
//    
//    for(int i =0; i < SIZE; i++){
//        data[i] = rank + value;
//        //data[i] = (double)rank + value;
//        std::cout << "(" << rank << ")" << data[i] << " " << std::endl;
//    }

//int main (int argc, char** argv){
//    MPI_Init(&argc, &argv);
//    int rank, procs;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &procs);
//    const int SIZE = 3;
//    int array[SIZE];
//    for(int i = 0; i < SIZE; i++){
//        array[i] = rank;
//    }
//    if(rank==0){
//        std::ofstream ofs("data.dat");
//        ofs.close();
//    }
//    for(int j=0; j < procs; j++){
//        MPI_Barrier(MPI_COMM_WORLD);
//        if(j!=rank)continue;
//        std::ofstream ofs("data.dat", std::ios::app);
//        for(int i = 0; i<SIZE; i++){
//            ofs << array[i] << std::endl;
//        }
//        ofs.close();
//    }
//    MPI_Finalize();
//}


//// Broadcast ////
struct parameter{
    int seed;
    double temperature;
    //vector<double> dat;
    double dat[2];
};

//int main(int argc, char** argv) {
//    MPI_Init(&argc, &argv);
//    int rank = 0, procs, pvalue;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &procs);
//    parameter param;
//
//    if(rank==0){
//        param.seed = 123;
//        param.temperature = 456;
//    }
//    MPI_Bcast(&param, sizeof(param), MPI_BYTE, 0, MPI_COMM_WORLD);
//    std::cout << "[1] rank = " << rank << " seed = " << param.seed << " temperature = " << param.temperature << std::endl;
//    
//    param.seed += rank+1;
//    param.temperature += rank+1;
//
//    std::cout << "[2] rank = " << rank << " seed = " << param.seed << " temperature = " << param.temperature << std::endl;
//
//    if(rank==0){
//        pvalue = param.seed;
//    }
//    MPI_Bcast(&pvalue, 1, MPI_INT, 0, MPI_COMM_WORLD);
//    std::cout << "[3] rank =" << rank << " pvalue = " << pvalue << std::endl;
//    //parameter param_unit[procs];
//    //MPI_Gather(param.data(), param.size(), );
//    //MPI_Gather(data.data(), data.size(), MPI_DOUBLE, buf.data(), data.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    
//    MPI_Finalize();
//}
//
//int main(int argc, char** argv) {
//    // 構造体を送るための準備
//    int blocklength[3] = {1, 1, 2};
//
//    //型指定
//    MPI::Datatype type[3];
//    type[0] = MPI_INT;
//    type[1] = MPI_DOUBLE;
//    type[2] = MPI_DOUBLE;
//
//    MPI_Init(&argc, &argv);
//    int rank = 0, procs, pvalue;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &procs);
//
//    parameter param;
//
//    //アドレスを得る
//    MPI::Aint base = MPI::Get_address(&param);
//    MPI::Aint displacements[3];
//    displacements[0] = MPI::Get_address(&param.seed) - base;
//    displacements[1] = MPI::Get_address(&param.temperature) - base;
//    displacements[2] = MPI::Get_address(&param.dat) - base;
//    // 構造体のMPIへの登録
//    cout << displacements[0] << " " << displacements[1] << " " << displacements[2] << " " << rank << endl; 
//    MPI::Datatype MPI_PARAMETER = MPI::Datatype::Create_struct(3, blocklength, displacements, type);
//    MPI_PARAMETER.Commit();
//    
//    vector<double> input;
//    for(int i = 0; i < 2; i++){
//        input.push_back(i+0.5);
//    }
//    
//    if(rank==0){
//        param.seed = 123;
//        param.temperature = 45.6;
//        //param.dat.push_back(-3.1);
//        //param.dat.push_back(4.1);
//        param.dat[0] = input[0];
//        param.dat[1] = input[1];
//    }
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Bcast(&param, 1, MPI_PARAMETER, 0, MPI_COMM_WORLD);
//    //MPI_Bcast(&param, sizeof(param), MPI_BYTE, 0, MPI_COMM_WORLD);
//    //std::cout << "[1] rank = " << rank << " seed = " << param.seed << " temperature = " << param.temperature << std::endl;
//    std::cout << "[01] rank = " << rank << " seed = " << param.seed << " temperature = " << param.temperature ;//<< endl;
//    cout <<  " dat = " << param.dat[0] <<  " " << param.dat[1] << endl;//endl; // 
//    
//    param.seed += rank+1;
//    param.temperature += rank+1;
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    std::cout << "[2] rank = " << rank << " seed = " << param.seed << " temperature = " << param.temperature << std::endl;
//
//    pvalue = 22;
//    if(rank==0){
//        param.seed = pvalue;
//    }
//    MPI_Barrier(MPI_COMM_WORLD);
//    MPI_Bcast(&pvalue, 1, MPI_INT, 0, MPI_COMM_WORLD);
//    std::cout << "[3] rank = " << rank << " pvalue(seed) = " << pvalue << std::endl;
//    
//    MPI_Finalize();
//}