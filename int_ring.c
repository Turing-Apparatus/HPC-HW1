/*  Assignment 1
    Ring
    Andrew Szymczak
*/

#include <unistd.h>
#include "util.h"
#include <mpi.h>

int main (int argc, char **argv)
{
  int i,P,N,M, tag, rank, source, target;                   // P = # PROCESSORS
  int *msg;                                                 // N = # LOOPS
  MPI_Status status;                                        // M = # INTS / MESSAGE

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (P < 2) { MPI_Abort(MPI_COMM_WORLD,1); }                     // INPUT CORRECTIONS
  N   =  (argc > 1 && atoi(argv[1]) > 0)? atoi(argv[1]) : 1;      // DEFAULT N=1, M=1
  M   =  (argc > 2 && atoi(argv[2]) > 0)? atoi(argv[2]) : 1;
  msg =  (int *) malloc(sizeof(int) * M);
  tag =  99;

  if (rank == 0)
  {
      int total = N * P*(P-1)/2;
      timestamp_type tic, toc;
      static double tictoc;
      get_timestamp(&tic);

      printf("\nProcessors: %d \nLoops: %d \n", P, N);
      printf("Message Length: %d \n", M);
      printf("Message Size: %f MB \n\n", M*sizeof(int)/1024./1024);

      source = P-1;
      target = 1;
      MPI_Send(msg, M, MPI_INT, target, tag, MPI_COMM_WORLD);

      for (i=0; i<N-1; i++)
      {
          MPI_Recv(msg, M, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
          MPI_Send(msg, M, MPI_INT, target, tag, MPI_COMM_WORLD);
      }

      MPI_Recv(msg, M, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
      get_timestamp(&toc);
      tictoc = timestamp_diff_in_seconds(tic, toc);

      printf("Message received <%d, %d> : %d\n", source, rank, msg[0]);
      printf((msg[0]==total)? "Ring Success\n\n" : "Ring Failure %d\n\n", total);
      printf("Time  %f seconds\n", tictoc);
      printf("Latency %f seconds\n\n", tictoc/N/P);
  }
  else
  {
      source = rank-1;
      target = (rank+1) % P;

      for (i=0; i<N; i++)
      {
          MPI_Recv(msg, M, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
          if (i == 0)
          {
              printf("Message received <%d, %d> : %d\n", source, rank, msg[0]);
              printf((rank==P-1 && N>1)? "Messages Cycling ...\n": "" );
          }
          msg[0] += rank;
          MPI_Send(msg, M, MPI_INT, target, tag, MPI_COMM_WORLD);
      }
  }

  free(msg);
  MPI_Finalize();
  return 0;
}


