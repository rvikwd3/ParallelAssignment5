/*
Ravikiran Kawade
Parallel Assignment 5

Addition of n nodes within p groups
20180724
*/

#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define N 8

int main(){
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    if(world_size > N){
        if(world_rank==0){ fprintf(stderr, "\nERROR:\tAt most use %d processes only\n", N); }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int input_array[world_size];
    int sum = 0;

    if(world_rank == 0){
        //Get random numbers
        srand(time(NULL));

        printf("Randomized Array:\t");
        for(int i=0; i < world_size; i++){
            input_array[i] = rand() % 100;
            printf(" %d", input_array[i]);
        }
        printf("\n");

    }

    //Create integer buffer to receive number
    int *node_data= (int*)malloc(sizeof(int));
    assert(node_data!= NULL);

    //Scatter number array from P0 to other processes
    MPI_Barrier(MPI_COMM_WORLD);
    if(world_rank==0){ printf("First barrier reached\n"); }

    printf("P%d: Scattered Input Array\n", world_rank);
    MPI_Scatter(input_array, 1, MPI_INT, node_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //Current process and its data
    printf("P%d: [%d]\n", world_rank, *node_data);

    //LSB complement
    int bit_complement = world_rank ^ 1UL << 0;
    printf("P%d: LSB bit complement %d\n", world_rank, bit_complement);

    free(node_data);

    MPI_Finalize();
    
    return 0;

}