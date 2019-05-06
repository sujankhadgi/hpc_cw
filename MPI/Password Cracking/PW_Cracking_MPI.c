#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>
/***********************************************************************
  Compile with:
    mpicc -o PW_Cracking_MPI PW_Cracking_MPI.c -lcrypt

  To run

    mpirun -n 3 ./PW_Cracking_MPI

************************************************************************/


int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$aFJ.B9OwE7S.ouqqP9IcmoTjL2lBggrJgnK40p.Q.KWWJ6a.eD9oqlFSECfqRu5b.oWW5BBBk6dXSOVZp7.pD.",

	"$6$KB$gn/N2E2JEi0oEmVD5h36H.hRi3GHZC9umjMwG3XbjFSWoZX4dbxTcbyGNhdq2.zDFwOid7ZgwhFhdgSGpn0Xf0",

	"$6$KB$EQa/PAHYxEYj2vICEL7CT01Og7/zJ28/3Fxf.5XuKuuGZdWxBr.I4lnvoHhfwjVyRCIT34siGLYB7Q2fpmNGG.",

	"$6$KB$8CNo1ZdhetazoVpKoIe/jrgcObb.EeXAjwDgvXYQr9ehdRrAsokQDruFmGVK.hWwnlZUctvSy.H94jgKWLmnO1"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}


void func1(char *salt_and_encrypted){
  int a, y, c, e;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='M'; a++){
    for(y='N'; y<='Z'; y++){
      for(c=0; c<=99; c++){
         for(e=0;e<=99; e++){
  
        sprintf(plain, "%c%c%02d%02d", a, y, c, e); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
        }
       } 
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void func2(char *salt_and_encrypted){
  int a, y, c , e;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='N'; a<='Z'; a++){
    for(y='A'; y<='Z'; y++){
      for(c=0; c<=99; c++){
         for(e=0;e<=99;e++){
        sprintf(plain, "%c%c%02d%02d", a, y, c, e); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
        }
       }
      }
    }
  }
  printf("%d solutions explored\n", count);
}
int timediff(struct timespec *start, 
                    struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main(){

  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 

int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){
      int a;
      int y;
      int i;
	MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
      MPI_Send(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	
    } else {
      if(rank == 1){
	int i;
        int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i<i++){
		func1(encrypted_passwords[i]);
	}
      }
	else if(rank == 2){
	int i;
      	int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i<i++){
		func2(encrypted_passwords[i]);
	}
	}
    }
}  
  MPI_Finalize(); 

  clock_gettime(CLOCK_MONOTONIC, &finish);
  timediff(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
	(time_elapsed/1.0e9));


  return 0;
}


