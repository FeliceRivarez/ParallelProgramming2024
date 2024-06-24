#test.sh
#!/bin/sh
#PBS -N test
#PBS -l nodes=node1_vir3

pssh -h $PBS_NODEFILE mkdir -p /home/s2213218/MPIRow 1>&2
scp master:/home/s2213218/lab4/MPIRow/testset.txt /home/s2213218/MPIRow
scp master:/home/s2213218/lab4/MPIRow/test /home/s2213218/MPIRow

pscp -h $PBS_NODEFILE /home/s2213218/lab4/MPIRow/test /home/s2213218/MPIRow 1>&2
/home/s2213218/MPIRow/test
