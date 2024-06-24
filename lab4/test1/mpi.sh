#mpi.sh
#!/bin/sh
#PBS -N mpi
#PBS -l nodes=node1_vir4

# pssh -h $PBS_NODEFILE rm -rf /home/s2213218
pssh -h $PBS_NODEFILE mkdir -p /home/s2213218/lab4/test1 1>&2
scp master:/home/s2213218/lab4/test1/testset.txt /home/s2213218/lab4/test1
scp master:/home/s2213218/lab4/test1/MPIRow.o /home/s2213218/lab4/test1

pscp -h $PBS_NODEFILE /home/s2213218/lab4/test1/MPIRow.o /home/s2213218/lab4/test1 1>&2
mpiexec -np 4 -machinefile $PBS_NODEFILE /home/s2213218/lab4/test1/MPIRow.o
