#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "license.h"
#include "config.h"
#include "common.h"

/* return the maximum number */
static unsigned int maximum(const unsigned int * num, const int n){
  unsigned int i, m = 0;
  for(i=1; i < n; i++)
    if(num[i] > num[m])
      m = i;  /* save maximum index */

  return num[m];
}

/* return the smaller number */
static int minimum(const int num1, const int num2, const int id1, const int id2){
  if(num1 < num2){
    return 1;
  }else if(num1 == num2){ /* if numbers match */
    return (id1 < id2);   /* use process ids if equal */
  }else{
    return 0;
  }
}

static void crit_lock(){
  int i;

  license_obj->entering[proc_id] = 1;
  license_obj->number[proc_id]   = 1 + maximum(license_obj->number, PROC_LIMIT);
  license_obj->entering[proc_id] = 0;

  for(i=0; i < PROC_LIMIT; i++){
    /* wait threads before us get a number */
    while (license_obj->entering[i]){}
    /* wait for threads with smaller numbers or higher priority finish */
    while ( (license_obj->number[i] != 0) &&
            minimum(license_obj->number[i], license_obj->number[proc_id], i, proc_id) ){ }
  }
}

static void crit_unlock(){
  /* remove or number to unlock */
  license_obj->number[proc_id] = 0;
}


int getlicense(void){
  crit_lock();

  /* while there are no licenses */
  while(license_obj->nlicenses <= 0){
    /* release critical section */
    crit_unlock();

    /* if we are signalled, stop */
    if(is_signalled){
      return -1;
    }

    //sleep(1);
    /* take the crit_lock back */
    crit_lock();
  }

  const int l = --license_obj->nlicenses;
  crit_unlock();

  fprintf(stderr, "P%d: Using license %d\n", getpid(), l);

  return l;
}

int returnlicense(void){
  crit_lock();
  license_obj->nlicenses++;
  crit_unlock();

  fprintf(stderr, "P%d: license returned\n", getpid());

  return 0;
}


int initlicense(void){
  int i;

  /* initialize the license and bakery variables */
  license_obj->counter = 0;

  for(i=0; i < PROC_LIMIT; i++){
    license_obj->entering[i] = 0;
    license_obj->number[i] = 0;
  }

  return 0;
}

int addtolicense(int n){
  crit_lock();
  license_obj->nlicenses += n;
  crit_unlock();
  fprintf(stderr, "P%d: %d license added\n", getpid(), n);
  return 0;
}

int removelicenses(int n){
  crit_lock();
  license_obj->nlicenses -= n;
  crit_unlock();
  fprintf(stderr, "P%d: %d license removed\n", getpid(), n);
  return 0;
}


void logmsg(const char* msg){

  const size_t len = strlen(msg);


  fprintf(stderr, "P%d: logging -> %s", getpid(), msg);

  /*log file is critical, so we have to crit_lock it */
  crit_lock();

  const int fd = open(LOGNAME, O_CREAT | O_APPEND | O_WRONLY, 0700);
  if(fd == -1){
    perror(err_prefix);
  }else{
    write(fd, (void*) msg, len);
    close(fd);
  }

  crit_unlock();
}
