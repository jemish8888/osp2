/* license functions */
int initlicense(void);

/* single license functions */
int getlicense(void);
int returnlicense(void);

/* multiple license functions */
int addtolicense(int n);
int removelicenses(int n);

/* log a message */
void logmsg(const char* msg);
