#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){
    const int PING_PONG_LIMIT = 5;

    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    if(world_size != 2){
        fprintf(stderr, "World size must only 2 processes\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int ping_pong_count;
    int partner_rank = (world_rank + 1) % 2;

    while(ping_pong_count < PING_PONG_LIMIT){
        if(world_rank == ping_pong_count % 2){
            ping_pong_count++;

            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("P%d Ping %d to %d\n", world_rank, ping_pong_count, partner_rank);
        }else{
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d Pong %d from %d\n", world_rank, ping_pong_count, partner_rank);
        }
    }

    MPI_Finalize();

    return 0;
}