struct license_object {
  /* number of available licenses */
  int nlicenses;

  /* bakery algorithm variables */
  unsigned int counter;
  unsigned char entering[PROC_LIMIT];
  unsigned int number[PROC_LIMIT];
};

/* perror prefix */
extern char err_prefix[100];
/* signal handler flag for interruption */
extern int is_signalled;
/* process ID, unique for every process */
extern int proc_id;

/* License object pointer to shared memory */
extern struct license_object * license_obj;

/* create shared memory license object*/
int create_license(const int n);
/* destroy and cler the shared memory object */
int destroy_license();

const char * make_msg(const char * msg);

/* get a number used in bakery algorithm for each process */
unsigned int get_number();
