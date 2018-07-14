#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size != 2){
        fprintf(stderr, "Must use 2 processes only");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    const int MAX_NUMBERS = 100;
    int numbers[MAX_NUMBERS];
    int number_amount;

    if(world_rank == 0){
        //Pick random amount of integers to send to P1
        srand(time(NULL));
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

        //Send numbers to P1
        MPI_Send(&numbers, number_amount, MPI_INT, 1, 5, MPI_COMM_WORLD);
        printf("P%d:\tSent %d numbers to P1\n", world_rank, number_amount);
    }else if(world_rank == 1){
        MPI_Status status;

        //Receive at most MAX_NUMBERS from P0
        MPI_Recv(&numbers, MAX_NUMBERS, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);

        MPI_Get_count(&status, MPI_INT, &number_amount);
    
        printf("P%d:\tReceived %d numbers from P0\n", world_rank, number_amount);
        printf("Source = %d\nTag = %d", status.MPI_SOURCE, status.MPI_TAG);
    }

    MPI_Finalize();

    return 0;
}