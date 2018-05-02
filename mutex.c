#include <sylvan.h>
#include <stdio.h>
#include <float.h>


int main()
{
    LACE_ME;
// 0 = auto-detect  number  of  Lace  workers
int n_workers = 0;
//initialize Lace with a deque size of 4M
lace_init(n_workers, 40960000);
lace_startup(0, NULL, NULL);

//initialize Sylvan's node table and operations cache
//with at least 2^20 entries, and at most 2^25 entries
sylvan_init_package(1LL<<20,1LL<<25,1LL<<20,1LL<<25);
sylvan_init_bdd();


BDD a = sylvan_and(sylvan_nithvar(1), sylvan_ithvar(2));




int i = 0;
char b[256];
snprintf(b, 256, "/tmp/sylvan/BDD-%d.dot", i);
FILE *f = fopen(b, "w+");
sylvan_fprintdot(f,a);
fclose(f);



printf("Hello, World\n");


//if Sylvan is compiled with -DSYLVAN_STATS=ON,
//then print statistics on stderr.
sylvan_stats_report(stderr);
//deinitialize sylvan
sylvan_quit();
//deinitialize Lace
lace_exit();
}	
