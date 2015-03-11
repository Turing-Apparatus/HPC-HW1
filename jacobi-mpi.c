/*  Assignment 1
    Jacobi-MPI
    Andrew Szymczak
*/

#include "util.h"
#include <math.h>
#include <mpi.h>


void jacobi(long, double, double*, double*);
double residual(long, double, double*);



int main (int argc, char **argv)
{

    long i, I, N, m;                                        // INITIALIZE
    int rank, P, tag, res_i;
    double *u, *u_, *u1, *u2;
    double res, res_, res1, hh;
    static double tictoc;
    timestamp_type tic, toc;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 2 || atol(argv[1])%P != 0 || atol(argv[2])<0)
    {
        printf("Must specify dimension and max iteration\n");
        printf("The number of processors must divide the dimension, p|N\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    N  =  atol(argv[1]);
    I  =  atol(argv[2]);
    m  =  N/P+2;
    u  =  (double *) malloc(sizeof(double) * m);
    u_ =  (double *) malloc(sizeof(double) * m);


    for (i=0; i<m; i++) {
      u[i] = 0;
      u_[i] = 0;
    }

    res1 =  sqrt(N);
    res  =  res1/1000;
    res_i =  1;
    hh   =  1./((N-1)*(N-1));
    tag  =  99;
    if (rank == 0) { get_timestamp(&tic); }


    for (i=0; i<I && res*1000000>res1; i++)                 // ITERATE JACOBI
    {
        u1 = (i%2)? u_ : u;
        u2 = (i%2)? u : u_;

        if (rank > 0  )   { MPI_Recv(&u1[0],   1, MPI_DOUBLE, rank-1, tag, MPI_COMM_WORLD, &status); }
        if (rank < P-1)   { MPI_Send(&u1[m-2], 1, MPI_DOUBLE, rank+1, tag, MPI_COMM_WORLD); }

        if (i%res_i == 0)                                   // CALCULATE RESIDUAL
        {
            res_ = residual(m, hh, u1);
            MPI_Allreduce(&res_, &res, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            res  = sqrt(res);
        }

        jacobi(m, hh, u1, u2);
        if (rank > 0  )   { MPI_Send(&u2[1],   1, MPI_DOUBLE, rank-1, tag, MPI_COMM_WORLD); }
        if (rank < P-1)   { MPI_Recv(&u2[m-1], 1, MPI_DOUBLE, rank+1, tag, MPI_COMM_WORLD, &status); }
        if (rank == 0 && i%res_i==0)    { printf("%ld %f\n", i, res); }
    }

    if (rank == 0)                                          // TIME AND PRINT
    {
        get_timestamp(&toc);
        tictoc = timestamp_diff_in_seconds(tic,toc);
        printf("Time elapsed is %f seconds.\n", tictoc);
        printf("Number of iterations : %ld\n", i);
        printf("Residual ||Au-f|| = %f \n\n", res);
    }

    free(u);
    free(u_);
    MPI_Finalize();
    return 0;
}



// LOCAL JACOBI
void jacobi(long m, double hh, double *u, double *u_)
{
    int i;
    for (i=1; i<m-1; i++)
        u_[i] = (hh + u[i-1] + u[i+1])/2;
}



// LOCAL RESIDUAL
double residual(long m, double hh, double *u)
{
    int i;
    double res = 0;
    for (i=1; i<m-1; i++)
        res += pow((-u[i-1] + 2*u[i] - u[i+1])/hh - 1, 2);
    return res;
}


