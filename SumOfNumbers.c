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
        fprintf(stderr, "At most use %d processes only", N);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int input_array[world_size];

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
    int *number_buffer = (int*)malloc(sizeof(int));
    assert(number_buffer != NULL);

    //Scatter number array from P0 to other processes

    printf("P%d: Scattered Input Array\n", world_rank);
    MPI_Scatter(input_array, 1, MPI_INT, number_buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("P%d: [%d]\n", world_rank, *number_buffer);

    free(number_buffer);

    MPI_Finalize();
    
    return 0;

}