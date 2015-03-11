HW=1
hw:  hw$(HW)
hw1: compile


clean:
	rm -f int_ring jacobi-mpi


compile:
	mpicc int_ring.c -o int_ring
	mpicc jacobi-mpi.c -o jacobi-mpi


run:
	mpirun -np 10 ./int_ring 1000
	mpirun -np 10 ./jacobi-mpi 10000 10

	# mpirun -np 20 ./int_ring 1000
	# mpirun -np 20 ./int_ring 2000
	# mpirun -np 20 ./int_ring 3000
	# mpirun -np 20 ./int_ring 4000
	# mpirun -np 20 ./int_ring 5000
	# mpirun -np 20 ./int_ring 6000
	# mpirun -np 20 ./int_ring 7000
	# mpirun -np 20 ./int_ring 8000
	# mpirun -np 20 ./int_ring 9000
	# mpirun -np 20 ./int_ring 10000

	# mpirun -np 20 ./int_ring 10000 524288

	# mpirun -np 1 ./jacobi-mpi 2520 5000000
	# mpirun -np 2 ./jacobi-mpi 2520 5000000
	# mpirun -np 3 ./jacobi-mpi 2520 5000000
	# mpirun -np 4 ./jacobi-mpi 2520 5000000

	# mpirun -np 20 ./jacobi-mpi 1200 100000
	# mpirun -np 40 ./jacobi-mpi 1200 100000
	# mpirun -np 60 ./jacobi-mpi 1200 100000
	# mpirun -np 80 ./jacobi-mpi 1200 100000
	# mpirun -np 100 ./jacobi-mpi 1200 100000

