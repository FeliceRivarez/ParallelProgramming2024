# openMPRow.sh
#!/bin/sh
# PBS -N openMPRow.o
# PBS -l node=4

pssh -h $PBS_NODEFILE mkdir -p /home/s2213218/lab3 1>&2
scp master:/home/s2213218/lab3/openMPRow.o /home/s2213218/lab3
pscp -h $PBS_NODEFILE master:/home/s2213218/lab3/openMPRow.o /home/s2213218/lab3 1>&2
mpiexec -np 4 -machinefile $PBS_NODEFILE /home/s2213218/lab3/openMPRow.o
#perf record -e L1-dcache-load-misses,L1-dcache-loads,cache-misses ./openMPRow.o
#perf report
#/home/s2213218/lab3/openMPRow.o
