/* File:	bubble.c
 *
 * Purpose: Use bubble sort to sort a list of ints.
 *
 * Compile: gcc -g -Wall -o bubble bubble.c
 *	 Usage:	bubble <n> <g|i>
 *		 n:	number of elements in list
 *	   'g':	generate list using a random number generator
 *	   'i':	user input list
 *
 * Input:	list (optional)
 * Output:	sorted list
 *
 * IPP:	Section 3.7.1 (pp. 127 and ff.) and Section 5.6.1 
 *		(pp. 232 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

/* For random list, 0 <= keys < RMAX */
const int RMAX = 100;
int chunckSize,nthreads;

struct args {
	int*a;
	int n;
	int max;
	pthread_mutex_t *lock;
};

void Usage(char* prog_name);
void Get_args(int argc, char* argv[], int* n_p, char* g_i_p);
void Generate_list(int a[], int n);
void Print_list(int a[], int n, char* title);
void Read_list(int a[], int n);
void Bubble_sort(int a[], int n);

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	int n;
	char g_i;
	int* a;

	Get_args(argc, argv, &n, &g_i);
	a = (int*) malloc(n*sizeof(int));
	if (g_i == 'g') {
		Generate_list(a, n);
		//Print_list(a, n, "Before sort");
	} else {
		Read_list(a, n);
	}

	Bubble_sort(a, n);

	//Print_list(a, n, "After sort");
	
	free(a);
	return 0;
}	/* main */


/*-----------------------------------------------------------------
 * Function:	Usage
 * Purpose:	Summary of how to run program
 */
void Usage(char* prog_name) {
	fprintf(stderr, "usage:	%s <n> <g|i>\n", prog_name);
	fprintf(stderr, "	n:	number of elements in list\n");
	fprintf(stderr, "	'g':	generate list using a random number generator\n");
	fprintf(stderr, "	'i':	user input list\n");
}	/* Usage */


/*-----------------------------------------------------------------
 * Function:	Get_args
 * Purpose:	Get and check command line arguments
 * In args:	argc, argv
 * Out args:	n_p, g_i_p
 */
void Get_args(int argc, char* argv[], int* n_p, char* g_i_p) {
	*n_p = atoi(argv[1]);
	*g_i_p = argv[2][0];
	chunckSize = atoi(argv[3]);
	nthreads = atoi(argv[4]);
	if (nthreads<=0)
		nthreads=1;

	if (*n_p <= 0 || (*g_i_p != 'g' && *g_i_p != 'i') ) {
		Usage(argv[0]);
		exit(0);
	}
}	/* Get_args */


/*-----------------------------------------------------------------
 * Function:	Generate_list
 * Purpose:	Use random number generator to generate list elements
 * In args:	n
 * Out args:	a
 */
void Generate_list(int a[], int n) {
	int i;

	srandom(0);
	for (i = 0; i < n; i++)
		a[i] = random() % RMAX;
}	/* Generate_list */


/*-----------------------------------------------------------------
 * Function:	Print_list
 * Purpose:	Print the elements in the list
 * In args:	a, n
 */
void Print_list(int a[], int n, char* title) {
	int i;

	printf("%s:\n", title);
	for (i = 0; i < n; i++)
		printf("%d ", a[i]);
	printf("\n\n");
}	/* Print_list */


/*-----------------------------------------------------------------
 * Function:	Read_list
 * Purpose:	Read elements of list from stdin
 * In args:	n
 * Out args:	a
 */
void Read_list(int a[], int n) {
	int i;

	printf("Please enter the elements of the list\n");
	for (i = 0; i < n; i++)
		scanf("%d", &a[i]);
}	/* Read_list */

void* sort(void* args) {
	struct args *arguments = (struct args*) args;
	int *a = arguments->a;
	int n = arguments->n;
	int max = arguments->max;
	pthread_mutex_t *lock = arguments->lock;

	while ( n>1 ) {
		int i,end,temp,currSize,block;

		pthread_mutex_lock (&lock[0]);
		currSize=--n;

		for (block=0; block<max; block++) {		
			
			end = chunckSize*(block+1);

			for (i = chunckSize*block; i<end-1 && i<currSize ; i++) {
				if (a[i] > a[i+1]) {
					temp = a[i];
					a[i] = a[i+1];
					a[i+1] = temp;
				}
			}
			if (end <= currSize) {
				pthread_mutex_lock (&lock[block+1]);
				if (a[end-1] > a[end]) {
					temp = a[end-1];
					a[end-1] = a[end];
					a[end] = temp;
				}
				pthread_mutex_unlock (&lock[block]);
			}
			else {
				pthread_mutex_unlock (&lock[block]);
				break;
			}
				
		}
	}
	return NULL;
}

/*-----------------------------------------------------------------
 * Function:	 Bubble_sort
 * Purpose:		Sort list using bubble sort
 * In args:		n
 * In/out args:	a
 */
void Bubble_sort(
	int a[]	/* in/out */, 
	int n	/* in	 */) {
	double start,end,elapsed;
	GET_TIME(start);
	int i,max = n%chunckSize==0 ? n/chunckSize : n/chunckSize+1;
	
	//create locks
	pthread_mutex_t *lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t) * max);
	for(i=0; i<max; i++)
		pthread_mutex_init(lock+i,NULL);

	//create threads
	struct args *arguments = malloc (sizeof(struct args));
	arguments->a = a;
	arguments->n = n;
	arguments->max = max;
	arguments->lock = lock;
	pthread_t *threads = malloc (sizeof(pthread_t) * nthreads-1);
	for(i=0; i<nthreads-1; i++)
		pthread_create(threads+i, NULL, sort, (void*)arguments);

	sort((void*) arguments);

	//join threads
	for(i=0; i<nthreads-1; i++)
		pthread_join(threads[i], NULL);

	//destroy locks
	for(i=0; i<max; i++)
		pthread_mutex_destroy(lock+i);
	free(lock);

	GET_TIME(end);
	elapsed=end-start;
	printf("%lf\n",elapsed);
}	/* Bubble_sort */
