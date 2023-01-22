#define BUFFSIZE 10
//#define BUFFSIZE 153
//#define delay 10000
#define delay 1000
extern char buff [BUFFSIZE];
extern int log_enable;

extern sem_t sem;
extern pthread_mutex_t mutex;

extern int raw_txt;
