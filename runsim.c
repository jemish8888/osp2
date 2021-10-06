#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <time.h>

#include "license.h"
#include "config.h"
#include "common.h"

/* PID of the testsim process */
static pid_t testsim_pid = 0;
/* processes counters for started and exited */
static int proc_started = 0, proc_exited = 0;

static void docommand(char * cmdline){
  char process_id[10];
  int status;

  /* create the arguments for testsim process */
  char *name = strtok(cmdline, " ");
  char *sleep_time  = strtok(NULL, " ");
  char *num_repeats = strtok(NULL, " ");
  snprintf(process_id, sizeof(process_id), "%d", proc_id);

  const pid_t pid = fork();
  switch(pid){
    case -1:
      perror(err_prefix);
      break;

    case 0: /* do child runs the process */
      execl(name, name, sleep_time, num_repeats, process_id, NULL);
      perror(err_prefix);
      break;

      default:
      /* save PID in case we get interrupted */
      testsim_pid = pid;
      /* do process waits for it to finish */
      waitpid(pid, &status, 0);
      break;
  }
  /* process or child exists here */
  exit(EXIT_SUCCESS);
}

static void do_wait(const int flags){
  int status;
  while(waitpid(-1, &status, flags) > 0){
    proc_exited++;
    returnlicense();
    fprintf(stderr, "P%d: %d/%d processes exited\n", getpid(), proc_exited, proc_started);
  }
}

static void on_interrupt(const int sig){
  sigset_t mask, oldmask;

  /* block signals */
  sigfillset(&mask);
  sigprocmask(SIG_SETMASK, &mask, &oldmask);

  if(sig == SIGINT){
    /* raise the interrupt flag */
    is_signalled = 1;
    fprintf(stderr, "P%d: Ctrl-C received\n", getpid());
  }else if(sig == SIGALRM){
    /* send termination signal to all child processes */
    kill(0, SIGTERM);

  }else if(sig == SIGTERM){
    /* raise the interrupt flag */
    is_signalled = 1;
    fprintf(stderr, "P%d: SIGTERM received\n", getpid());
    if(testsim_pid > 0){
      kill(testsim_pid, SIGTERM);
    }
  }else if(sig == SIGCHLD){
    do_wait(WNOHANG);
  }

  //unblock signals
  sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

int main(const int argc, char * const argv[]){
  char line[MAX_CANON];
  /* number of children and licenses */
  int num_licenses = 0;
  struct sigaction sa;

  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = on_interrupt;
  if( (sigaction(SIGINT, &sa, NULL) == -1) ||
      (sigaction(SIGTERM, &sa, NULL) == -1) ||
      (sigaction(SIGCHLD, &sa, NULL) == -1) ||
      (sigaction(SIGALRM, &sa, NULL) == -1)){
     perror("sigaction");
     return EXIT_FAILURE;
  }

  sa.sa_handler = SIG_IGN;
  if(sigaction(SIGTERM, &sa, NULL) == -1){
     perror("sigaction");
     return EXIT_FAILURE;
  }

  alarm(TIME_LIMIT);

  /* check number of arguments*/
  if(argc != 2){
    fprintf(stderr, "Usage: runsim <nunm_licenses>\n");
    return EXIT_FAILURE;
  }

  /* set number of licenses from argument */
  num_licenses = atoi(argv[1]);
  if((num_licenses <= 0) || (num_licenses > PROC_LIMIT)){
    fprintf(stderr, "Error: Invalid number of licenses %d. Has to be in range [1; %d]\n", num_licenses, PROC_LIMIT);
    return EXIT_FAILURE;
  }

  /* create the error string from program name */
  snprintf(err_prefix, sizeof(err_prefix), "%s: Error: ", argv[0]);

  /* create the license object */
  if(create_license(num_licenses) < 0){
    return EXIT_FAILURE;
  }

  /* while we have commands and available licenses */
  while(fgets(line, sizeof(line), stdin) != NULL){

    if(line[0] == '\n'){ //if its an empty line
      continue; //skip it
    }
    int ll = strlen(line);

    if(line[ll-1] == '\n'){
      line[--ll] = '\0';
    }

    const int lid = getlicense(); //child license ID
    if(lid == -1){
      break;
    }
    fprintf(stderr, "P%d: Launching child on license %d\n", getpid(), lid);

    const int process_id = get_number();
    //fork a child
    const pid_t pid = fork();
    if(pid == -1){
      perror(err_prefix);
      break;
    }else if(pid == 0){
      /* copy our process id */
      proc_id = process_id;

      /* close input, its used only by runsim */
      fclose(stdin);

      /* ignore interrupt and alarm */
      sa.sa_handler = SIG_IGN;
      sigaction(SIGINT, &sa, NULL);
      sigaction(SIGALRM, &sa, NULL);

      /* default for child signals */
      sa.sa_handler = SIG_DFL;
      sigaction(SIGCHLD, &sa, NULL);

      /* process termination signals */
      sa.sa_handler = on_interrupt;
      sigaction(SIGTERM, &sa, NULL);

      /* run the command from input line */
      docommand(line);

    }else{
      /* increment the process counter */
      ++proc_started;
    }
  }

  /* wait for any processes left */
  while(proc_exited < proc_started){
    do_wait(0);
  }

  logmsg(make_msg("runsim terminated"));

  destroy_license(num_licenses);

  return EXIT_SUCCESS;
}
