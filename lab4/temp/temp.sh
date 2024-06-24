#mpi.sh
#!/bin/sh
#PBS -N mpi
#PBS -l nodes=node1_vir3

pssh -h $PBS_NODEFILE mkdir -p /home/s2213218/lab4/temp 1>&2
scp master:/home/s2213218/lab4/temp/MPIRow.o /home/s2213218/lab4/temp

pscp -h $PBS_NODEFILE /home/s2213218/lab4/temp/MPIRow.o /home/s2213218/lab4/temp 1>&2
mpiexec -np 4 -machinefile $PBS_NODEFILE /home/s2213218/lab4/temp/MPIRow.o