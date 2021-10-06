#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <strings.h>
#include <time.h>
#include "config.h"
#include "common.h"
#include "license.h"

static int shmid = -1;
static char license_file[PATH_MAX];

int proc_id = 0; /* unique process id ( for bakery algorithm )*/
int is_signalled = 0;
struct license_object * license_obj = NULL;
char err_prefix[100];

int create_license(const int num_licenses){

  /* create the license filename, using user ID*/
  snprintf(license_file, PATH_MAX, "/tmp/license.%u", getuid());

  if(num_licenses){
    fprintf(stderr, "P%u| License file is %s\n", getpid(), license_file);

    /* create the license file */
    int fd = creat(license_file, 0700);
    if(fd == -1){
      perror(err_prefix);
      return -1;
    }
    close(fd);
  }

  key_t license_key = ftok(license_file, 4444);
  if(license_key == -1){
    perror(err_prefix);
    return -1;
  }

  shmid = shmget(license_key, sizeof(struct license_object), (num_licenses) ? IPC_CREAT | IPC_EXCL | S_IRWXU : 0);
  if(shmid == -1){
    perror(err_prefix);
    return -1;
  }

  license_obj = (struct license_object*) shmat(shmid, NULL, 0);
  if(license_obj == (void*)-1){
    perror(err_prefix);
    return -1;
  }

  if(num_licenses){

    /* clear the license object */
    bzero(license_obj, sizeof(struct license_object));
    /* initialize it */
    initlicense();
    /* add the provided number of licenses */
    addtolicense(num_licenses);
  }

  return 0;
}

int destroy_license(const int num_licenses){


  if(shmdt(license_obj) == -1){
    perror(err_prefix);
    return -1;
  }

  if(num_licenses > 0){
    fprintf(stderr, "P%u| Destroying license file %s\n", getpid(), license_file);

	  if(shmctl(shmid, IPC_RMID, NULL) == -1){
      perror(err_prefix);
      return -1;
    }

    if(unlink(license_file) == -1){
      perror(err_prefix);
      return -1;
    }
  }
  return 0;
}

const char * make_msg(const char * msg){
  static char buf[100];
  char stamp[30];

  //create a time stamp
  time_t now = time(NULL);
  struct tm * tm = localtime(&now);
  strftime(stamp, sizeof(stamp), "%D %T", tm);

  //prepare the message
  snprintf(buf, sizeof(buf), "%s\t%u\t%s\n", stamp, getpid(), msg);

  return buf;
}

unsigned int get_number(){
  return license_obj->counter++;
}
