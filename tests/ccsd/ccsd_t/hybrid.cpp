/*------------------------------------------hybrid execution------------*/
/* $Id$ */
#include <assert.h>
///#define NUM_DEVICES 1
static long long device_id=-1;
#include <stdio.h>
#include <stdlib.h>
#include "header.hpp"
// #include "ga.h"
// #include "typesf2c.h"

// void FATR util_getppn_(Integer *);
// int util_my_smp_index();

int check_device(long icuda) {
  /* Check whether this process is associated with a GPU */
  // if((util_my_smp_index()) < icuda) return 1;
  return 0;
}

//void device_init_(int *icuda) {
int device_init(long icuda,int *cuda_device_number ) {
  /* Set device_id */
  
  // int dev_count_check=0;
  // device_id = util_my_smp_index();
  // cudaGetDeviceCount(&dev_count_check);
  // if(dev_count_check < icuda){
  //   printf("Warning: Please check whether you have %ld cuda devices per node\n",icuda);
  //   fflush(stdout);
  //   *cuda_device_number = 30;
  // }
  // else {
    cudaSetDevice(device_id);
  // }
  return 1;
}

