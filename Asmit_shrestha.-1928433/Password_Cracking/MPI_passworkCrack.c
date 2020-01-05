#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include <pthread.h>
/*
 To compile:
     mpicc -o MPI_passworkCrack MPI_passworkCrack.c -lcrypt

 To run 3 processes on this computer:
     mpirun -n 3 ./MPI_passworkCrack
*/

int n_passwords = 4;

char *encrypted_passwords[] = {
"$6$KB$0G24VuNaA9ApVG4z8LkI/OOr9a54nBfzgQjbebhqBZxMHNg0HiYYf1Lx/HcGg6q1nnOSArPtZYbGy7yc5V.wP/",
"$6$KB$VDUCASt5S88l82JzexhKDQLeUJ5zfxr16VhlVwNOs0YLiLYDciLDmN3QYAE80UIzfryYmpR.NFmbZvAGNoaHW.",
"$6$KB$0n1YjoLnJBuAdeBsYFW3fpZzMPP8xycQbEj35GvoerMnEkWIAKnbUBAb70awv5tfHylWkVzcwzHUNy/7l7I1c/",
"$6$KB$HKffNNiGzngqYueF89z3gwWZMg.xUBIz/00QSCbgwKtRHmwUbZX6jTH4VUAg3L3skaO8qtNf5LE7WP39jQ7ZJ0"
};

void posix()
{
  int i;
pthread_t thread1, thread2;

    void *function1();
    void *function2();
for(i=0;i<n_passwords;i<i++) {
   
    
    pthread_create(&thread1, NULL,function1, encrypted_passwords[i]);
    pthread_create(&thread2, NULL,function2, encrypted_passwords[i]);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
 }
}


/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/


void *function1(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hahttps://www.youtube.com/watch?v=L8yJjIGleMwshing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

substr(salt, salt_and_encrypted, 0, 6);

for(x='A'; x<='M'; x++){
   for(y='A'; y<='Z'; y++){
     for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } 
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void *function2(char *salt_and_encrypted){
  int i, j, k;     // Loop counters
  char salt[7];    // String used in hahttps://www.youtube.com/watch?v=L8yJjIGleMwshing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(i='N'; i<='Z'; i++){
    for(j='A'; j<='Z'; j++){
      for(k=0; k<=99; k++){
        sprintf(plain, "%c%c%02d", i,j,k);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

//Calculating time

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
 {
	  long long int ds =  finish->tv_sec - start->tv_sec; 
	  long long int dn =  finish->tv_nsec - start->tv_nsec; 

	  if(dn < 0 ) {
	    ds--;
	    dn += 1000000000; 
  } 
	  *difference = ds * 1000000000 + dn;
	  return !(*difference > 0);
}
int main(int argc, char *argv[])
{
  	
	struct timespec start, finish;   
  	long long int time_elapsed;
	int size, rank, i;
	clock_gettime(CLOCK_MONOTONIC, &start);

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
}
	else{
	  if(rank == 0){
		int x;
		MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		}
		else if(rank == 1){
		int number;
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
		                 MPI_STATUS_IGNORE);
		for(i-0;i<n_passwords;i<i++){
			function1(encrypted_passwords[i]);	
		}
	      }
		else{
		int number;
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
		                 MPI_STATUS_IGNORE);
		for(i-0;i<n_passwords;i<i++){
			function2(encrypted_passwords[i]);	
			}
		}
}

	MPI_Finalize(); 
  	
	clock_gettime(CLOCK_MONOTONIC, &finish);
	  time_difference(&start, &finish, &time_elapsed);
	if(rank==0){
	  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		                                 (time_elapsed/1.0e9)); 
}
  return 0;
}
