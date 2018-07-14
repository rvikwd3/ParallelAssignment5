#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int token;

    //Receive token from lower rank
    if(world_rank != 0){
        MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P%d:\tToken (%d) received from P%d\n", world_rank, token, world_rank - 1);
    }else{  
        token = -1; //If P0 then set token
    }

    //Send token to higher rank process
    MPI_Send(&token, 1, MPI_INT, (world_rank + 1)%world_size, 0, MPI_COMM_WORLD);
    printf("P%d:\tToken (%d) sent to P%d\n", world_rank, token, (world_rank + 1)%world_size);

    if(world_rank == 0){
        MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P0 received token from last Process P%d\n", world_size - 1);
    }

    MPI_Finalize();

    return 0;
}