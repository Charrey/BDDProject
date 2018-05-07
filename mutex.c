#include <stdio.h>
#include <float.h>
#include <sylvan.h>
#include <stdarg.h>
#include <stdbool.h>

uint32_t c_var = 200;
uint32_t cp_var = 201;
uint32_t w_var = 100;
uint32_t wp_var = 101;
uint32_t f_var = 300;
uint32_t fp_var = 301;

BDD c;
BDD cp;
BDD w;
BDD wp;
BDD f;
BDD fp;

BDD reverse_varmap = sylvan_map_empty();
BDD varset = sylvan_set_empty();

BDD initial;
BDD transLeave;
BDD transEnter;
BDD transExit;
BDD transRestart;

void showBDD(BDD a) {
    int i = 0;
    char c[256];
    snprintf(c, 256, "/tmp/sylvan/BDD-%d.dot", i);
    FILE *f = fopen(c, "w+");
    sylvan_fprintdot(f,a);
    fclose(f);
}

void init_sylvan_lace() {
    // 0 = auto-detect  number  of  Lace  workers
    int n_workers = 0;
    //initialize Lace with a deque size of 4M
    lace_init(n_workers, 40960000);
    lace_startup(0, NULL, NULL);
    LACE_ME;

    //initialize Sylvan's node table and operations cache
    //with at least 2^20 entries, and at most 2^25 entries
    sylvan_init_package(1LL<<20,1LL<<25,1LL<<20,1LL<<25);
    sylvan_init_bdd();
}

BDD and(int count, ...) {
    LACE_ME;
    //initialize starting BDD
    BDD res = sylvan_true;
    //Declare list of variable args
    va_list ap;
    //Start feeding args
    va_start(ap, count);
    int i;
    for(i=0; i<count; i++)
        //feed arguments and AND them to init BDD
        res = sylvan_and(res, va_arg(ap, BDD));
    //End feeding args
    va_end(ap);
    return res;
}

BDD or(int count, ...) {
    LACE_ME;
    //initialize starting BDD
    BDD res = sylvan_false;
    //Declare list of variable args
    va_list ap;
    //Start feeding args
    va_start(ap, count);
    int i;
    for(i=0; i<count; i++)
        //feed arguments and AND them to init BDD
        res = sylvan_or(res, va_arg(ap, BDD));
    //End feeding args
    va_end(ap);
    return res;
}

BDD not(BDD a) {
    return sylvan_not(a);
}


BDD is(BDD a, BDD b) {
    LACE_ME;
    return sylvan_equiv(a, b);
}

void init_mutex() {
    LACE_ME;
    //All variables as BDDs
    c = sylvan_ithvar(c_var);
    cp = sylvan_ithvar(cp_var);
    w = sylvan_ithvar(w_var);
    wp = sylvan_ithvar(wp_var);
    f = sylvan_ithvar(f_var);
    fp = sylvan_ithvar(fp_var);

    // We say here that we need to rename cp to c
    reverse_varmap = mtbdd_map_add(reverse_varmap, cp_var, c);
    reverse_varmap = mtbdd_map_add(reverse_varmap, wp_var, w);
    reverse_varmap = mtbdd_map_add(reverse_varmap, fp_var, f);

    //Set of all variables which needs to be renamed
    varset = sylvan_set_add(varset, c_var);
    varset = sylvan_set_add(varset, w_var);
    varset = sylvan_set_add(varset, f_var);

    //Initial state
    initial = and(3,not(c), w, not(f));

    //-------------Transitions---------------

    //Leave transition
    transLeave = and(3, not(cp), is(w,wp), is(f,fp));

    //Enter transition
    transEnter = and(5, not(c), w, cp, not(wp), is(f, fp));

    //Exit transition
    transExit = and(4, c, not(cp), not(is(f, fp)), is(w,wp));

    //Restart transition
    transRestart = and(4, f, not(is(w, wp)), not(fp), is(c,cp));

    //--------------------------------------
}

BDD next(BDD beginstate, BDD transition) {
    LACE_ME;
    BDD nextstate = sylvan_compose(sylvan_exists(and(2, beginstate, transition), varset), reverse_varmap);
    return nextstate;
}

bool check(int count, BDD toCheck, ...) {
  //TODO HANS (and all, then check if equals sylvan_false)
}


int main()  {

    init_sylvan_lace();
    LACE_ME; //Waarom moet dit?
    init_mutex();

    BDD transAny = or(4, transRestart, transLeave, transEnter, transExit);


    //BDD containing all states after the transition leave from the initial state
    //showBDD(next(initial,transLeave));
    BDD myBDD = initial;
    int counter = 0;
    for (int i = 0; i < 3; ++i) {
        myBDD = next(myBDD, transAny);
    }

    BDD check = and(4, myBDD, not(c), not(w), f);

    showBDD(transAny);
    char str[12];
    sprintf(str, "%d", sylvan_nodecount(transAny));

    //if Sylvan is compiled with -DSYLVAN_STATS=ON,
    //then print statistics on stderr.
    sylvan_stats_report(stderr);
    //deinitialize sylvan
    sylvan_quit();
    //deinitialize Lace
    lace_exit();
}	
