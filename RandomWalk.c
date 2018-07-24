/*
Ravikiran Kawade
Parallel Assignment 5

Addition of n nodes within p groups
20180724
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 8

int main(int argc, char** argv){

    MPI_Init(NULL, NULL);
    

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size > N){
        fprintf(stderr, "At most use %d processes only", N);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank == 0){

        printf("No of Arguments:\t%d\n", argc);
        for(int c=0; c < argc; c++){
            printf("Argument %d:\t%s\n", c, argv[c]);
        }
    }

    //Get random numbers
    int input_array[10];

    for(int i=0; i < world_size; i++){
        printf("P%d (%d)\n", world_rank, i);
    }

    MPI_Finalize();

    return 0;
}