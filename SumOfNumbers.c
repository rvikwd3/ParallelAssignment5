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

unsigned int getBinaryLength(unsigned int v){
    //DeBruijn Method of Highest Bit Position
    //http://supertech.csail.mit.edu/papers/debruijn.pdf
    static const int MultiplyDeBruijnBitPosition[32] =
    {
        0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
        8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
    };

    v |= v >> 1; // first round down to one less than a power of 2
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    return MultiplyDeBruijnBitPosition[(unsigned int)( v * 0x07C4ACDDU ) >> 27];
}

int main(){
    MPI_Init(NULL, NULL);

    unsigned int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    unsigned int world_rank;
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

    //Check number of binary digits
    int binary_digits = getBinaryLength(world_size); 
    if(world_rank == 0){ printf("Maximum bit size:\t%d\n", binary_digits); }

    //Scatter number array from P0 to other processes
    MPI_Barrier(MPI_COMM_WORLD);
    if(world_rank==0){ printf("First barrier reached\n"); }

    printf("P%d: Scattered Input Array\n", world_rank);
    MPI_Scatter(input_array, 1, MPI_INT, node_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //Current process and its data
    printf("P%d: [%d]\n", world_rank, *node_data);

    //Ready the receiving buffer
    unsigned int* recv_buffer = (int*)malloc(sizeof(int));
    assert(recv_buffer != NULL);

    for(int i=0; i < binary_digits; i++){
        //LSB complement
        int bit_complement = world_rank ^ 1UL << i;
        printf("P%d: LSB bit complement %d\n", world_rank, bit_complement);

        // MPI_Ssend(node_data, 1, MPI_INT, bit_complement, 0, MPI_COMM_WORLD);

    }
    free(node_data);

    MPI_Finalize();
    
    return 0;

}