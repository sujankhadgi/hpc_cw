#include <stdio.h>
#include <cuda_runtime_api.h>

/**************************************************************************
  
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __global__ functions
    2) Enable a simulation of password cracking in the absence of library 
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
    nvcc -o PW_Cracking_Cuda PW_Cracking_Cuda.cu
    ./PW_Cracking_Cuda
*****************************************************************************/

/****************************************************************************
  This function returns 1 if the attempt at cracking the password is 
  identical to the plain text password string stored in the program. 
  Otherwise,it returns 0.
*****************************************************************************/

__device__ int is_a_match(char *attempt) {
  char plain_password1[] = "CV78";
  char plain_password2[] = "FR83";
  char plain_password3[] = "IS53";
  char plain_password4[] = "TB35";
			

  char *a = attempt;
  char *b = attempt;
  char *c = attempt;
  char *d = attempt;
  
  char *p1 = plain_password1;
  char *p2 = plain_password2;
  char *p3 = plain_password3;
  char *p4 = plain_password4;
  
  
  while(*a == *p1) {
    if(*a == '\0') {
       printf("Password Found: %s\n",plain_password1);
    break;
    }
    a++;
    p1++;
  }
  while(*b == *p2) {
    if(*b == '\0') {
      printf("Password Found: %s\n",plain_password2);
    break;
    }
    b++;
    p2++;
  }
  while(*c == *p3) {
    if(*c == '\0') {
     printf("Password Found: %s\n",plain_password3);
    break;
    }
    c++;
    p3++;
  }
  while(*d == *p4) {
    if(*d == '\0') {
      printf("Password Found: %s\n",plain_password4);
    break;
    }
    d++;
    p4++;
  }
  return 0;
}

/****************************************************************************
  The kernel function assume that there will be only one thread and uses 
  nested loops to generate all possible passwords and test whether they match
  the hidden password.
*****************************************************************************/

__global__ void  kernel() {
  char a, b;
  int w, y, x, z;
  
  char password[8];
  password[6] = '\0'; 
  a = blockIdx.x+65;
  b = threadIdx.x+65;
  
  password[0] =a;
  password[1] =b;
  
  
  for(w=48; w<=57; w++){
    for(x=48; x<=57; x++){
     for(y=48; y<=57; y++){
      for(z=48; z<=57; z++){
	password[3] = w;
        password[2] = x;
        password[4] = y;
        password[5] = z;

        if(is_a_match(password)) {
        printf("password found: %s\n", password);
      } else {
        //printf("tried: %s\n", password);		  
      }
}
}
}
}

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

  kernel <<<26, 26>>>();
  cudaThreadSynchronize();

  
  
  
  

  clock_gettime(CLOCK_MONOTONIC, &finish);
  timediff(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
	(time_elapsed/1.0e9));

  return 0;
}


