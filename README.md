# qubo_tabu_mpi
This is a MPI version of D2TS tabu search algorithm for Unconstrained Binary Optimization

This code is written to perform a tabu search using the following algorithm:
Diversification-Driven Tabu Search (D2TS) for Unconstrained Binary Quadratic Programming  (QUBO problem)
The details of this algorithm can be found in
 Glover, F., Lü, Z. & Hao, J.-K. Diversifcation-driven tabu search for unconsrained binary quadratic problems. 4OR 8, 23-253 (2010). http://dx.doi.org/10.10\
 07/s10288-009-0115-y

to compile use gnumake  (change the compiler if necessary)
make
to run use
mpirun -np 4 ./d2ts -i inputfile
A sample qubo matrices are attached herewith
q1.txt
q2.txt

alternately one can use test.sh for testing