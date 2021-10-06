#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#include "config.h"
#include "license.h"
#include "common.h"

int main(const int argc, char * argv[]){
  int i, sleep_time=0, num_repeats=0;
  char buf[20];

  /* convert arguemnts to int */
  sleep_time  = atoi(argv[1]);
  num_repeats = atoi(argv[2]);
  proc_id     = atoi(argv[3]);

  /* create the error string from program name */
  snprintf(err_prefix, sizeof(err_prefix), "%s: Error: ", argv[0]);

  /* attach to the license object */
  if(create_license(0) < 0){
    return EXIT_FAILURE;
  }

  for(i=0; i < num_repeats; i++){
    sleep(sleep_time);
    snprintf(buf, sizeof(buf), "%i %i", i, num_repeats);
    logmsg(make_msg(buf));
  }

  destroy_license(0);

  return EXIT_SUCCESS;
}
