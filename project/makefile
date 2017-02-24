all: check_dir sequential omp_busy_wait omp_mutex pthreads_busy_wait pthreads_mutex omp_odd_even

check_dir:
	mkdir bin -p

sequential: src/sequential.c
	gcc src/sequential.c -Wall -O2 -o sequential

omp_busy_wait: src/omp_busy_wait.c
	gcc src/omp_busy_wait.c -Wall -O2 -fopenmp -o omp_busy_wait

omp_mutex: src/omp_mutex.c
	gcc src/omp_mutex.c -Wall -O2 -fopenmp -o omp_mutex

pthreads_busy_wait: src/pthreads_busy_wait.c
	gcc src/pthreads_busy_wait.c -Wall -O2 -lpthread -o pthreads_busy_wait

pthreads_mutex: src/pthreads_mutex.c
	gcc src/pthreads_mutex.c -Wall -O2 -lpthread -o pthreads_mutex

omp_odd_even: src/omp_odd_even.c
	gcc src/omp_odd_even.c -Wall -O2 -fopenmp -o omp_odd_even

clean:
	rm -f sequential omp_busy_wait omp_mutex pthreads_busy_wait pthreads_mutex
