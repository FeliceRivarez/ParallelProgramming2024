#mpi.sh
#!/bin/sh
#PBS -N mpi
#PBS -l nodes=master_vir3

# pssh -h $PBS_NODEFILE rm -rf /home/s2213218
pssh -h $PBS_NODEFILE mkdir -p /home/s2213218/lab4/newMPI 1>&2
scp master:/home/s2213218/lab4/newMPI/testset.txt /home/s2213218//lab4/newMPI
scp master:/home/s2213218/lab4/newMPI/MPIRow.o /home/s2213218//lab4/newMPI

pscp -h $PBS_NODEFILE /home/s2213218/lab4/newMPI/MPIRow.o /home/s2213218//lab4/newMPI 1>&2
mpiexec -np 4 -machinefile $PBS_NODEFILE /home/s2213218/lab4/newMPI/MPIRow.o
