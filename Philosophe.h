#define TIMESLICE_EAT 10
#define TIMESLICE_STARVE 1
#define NB_PHILOSOPHES 5
#define NB_FOURCHETTES NB_PHILOSOPHES
#define CREDENTIAL 0660

typedef enum {THINK, STARVE, EAT} state;


void my_handler(int);
void philosophe(int, int*);