#include <stdio.h>
#include <float.h>
#include <sylvan.h>

uint32_t c_var = 100;
uint32_t cp_var = 101;
uint32_t w_var = 200;
uint32_t wp_var = 201;
uint32_t f_var = 300;
uint32_t fp_var = 301;

BDD c;
BDD cp;
BDD w;
BDD wp;
BDD f;
BDD fp;

BDD varmap = sylvan_map_empty();
BDD reverse_varmap = sylvan_map_empty();
BDD varset = sylvan_set_empty();

void showBDD(BDD a) {
    int i = 0;
    char c[256];
    snprintf(c, 256, "/tmp/sylvan/BDD-%d.dot", i);
    FILE *f = fopen(c, "w+");
    sylvan_fprintdot(f,a);
    fclose(f);
}

void init() {
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

BDD not(BDD a) {
    return sylvan_not(a);
}

BDD and(BDD a, BDD b) {
    LACE_ME;
    return sylvan_and(a, b);
}

BDD is(BDD a, BDD b) {
    LACE_ME;
    return sylvan_equiv(a, b);
}

BDD next(BDD beginstate, BDD transition) {
    LACE_ME;
    BDD nextstate_not_renamed = sylvan_compose(sylvan_exists(and(beginstate, transition), varset), reverse_varmap);
    return nextstate_not_renamed;
}


int main()  {

    init();
    LACE_ME; //Waarom moet dit?

    //All variables as BDDs
    c = sylvan_ithvar(c_var);
    cp = sylvan_ithvar(cp_var);
    w = sylvan_ithvar(w_var);
    wp = sylvan_ithvar(wp_var);
    f = sylvan_ithvar(f_var);
    fp = sylvan_ithvar(fp_var);

    //Mapping of the leave trace
    // We say here that cp is the new version of the c_var variable
    reverse_varmap = mtbdd_map_add(reverse_varmap, cp_var, c);
    reverse_varmap = mtbdd_map_add(reverse_varmap, wp_var, w);
    reverse_varmap = mtbdd_map_add(reverse_varmap, fp_var, f);

    //Set of all variables which needs to be renamed

    varset = sylvan_set_add(varset, c_var);
    varset = sylvan_set_add(varset, w_var);
    varset = sylvan_set_add(varset, f_var);

    //Initial state
    BDD initial = and(not(c), and(w, not(f)));

    //-------------Transitions---------------

    //Leave transition
    BDD transleave = and(not(cp),and(is(w,wp),is(f,fp)));

    //Enter transition
    BDD transEnter = and(not(c), and(w, and(cp, and(not(wp), is(f, fp)))));    //TODO: Kan je een variable aantal args meegeven in een functie? Kan het met arrays?

    //Exit transition
    BDD exit = and(c, and(not(cp), and(not(is(f, fp)), is(w,wp))));

    //Restart transition
    BDD restart = and(f, and(not(is(w, wp)), and(not(fp), is(c,cp))));

    //--------------------------------------

    //BDD containing all states after the transition leave from the initial state

    showBDD(next(initial,transEnter));



    //BDD enter = and(not(c), and(w, and(cprime, and(not(wprime), is(f,fprime)))));








    //if Sylvan is compiled with -DSYLVAN_STATS=ON,
    //then print statistics on stderr.
    sylvan_stats_report(stderr);
    //deinitialize sylvan
    sylvan_quit();
    //deinitialize Lace
    lace_exit();
}	
