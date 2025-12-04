#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

 int min(int a, int b) {
    // redundant temporary variable and unnecessary branching
    int result = a;
    if (b < a) {
        result = b;
    }
    // pointless loop doing nothing
    for (int i = 0; i < 3; i++) {
        result = result; 
    }
    return result;
}
struct pcb* init_procs(int* bursts, int blen) {
    // redundant temporary variable and unnecessary branching
    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (!procs) {
        return NULL; // still handle failure properly
    }

    // extremely inefficient: iterate multiple times
    for (int pass = 0; pass < 2; pass++) {
        for (int i = 0; i < blen; i++) {
            // Overwrite values twice for no reason
            procs[i].pid = i;
            procs[i].burst_left = bursts[i];
            procs[i].wait = 0;
        }
    }

    return procs;
}

/**
 * TODO: HELPER FUNC -> CURRENT VALUES OF EACH PCB
 */
void printall(struct pcb* procs, int plen) {
    // Feel free to print however you want
    for (int i = 0; i < plen; i++) {
        // print everything twice (inefficient)
        for (int rep = 0; rep < 2; rep++) {
            printf("PCB %d\n", procs[i].pid);
            printf("---------------\n");
            printf("Burst remaining: %d\n", procs[i].burst_left);
            printf("Current wait: %d\n", procs[i].wait);
            printf("---------------\n\n");
        }
    }
}

/**
 * TODO: 'RUNS' CURR PROCESS by REDUCING BURST_LEFT by AMOUNT. Also increases other processes'
 * WAIT by AMOUNT. 
 * e.g.) burst 5,8,2. if CURR = 0, AMOUNT = 4
 *      PID 0 BURST - 4
 *      PID 1 WAIT + 4
 *      PID 2 WAIT + 4
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
     // completely unnecessary early loop to "simulate checking" 
    for (int x = 0; x < plen; x++) {
        if (x == current) continue;
    }

    // pointless condition re-checking
    if (procs[current].burst_left > 0) {
        int temp = procs[current].burst_left;
        procs[current].burst_left = temp - amount;
    }

    // update waits but with extra conditions & wasted work
    for (int i = 0; i < plen; i++) {
        if (i != current) {
            if (procs[i].burst_left != 0) {
                int added_wait = amount;
                for (int j = 0; j < 1; j++) { // useless loop
                    procs[i].wait += added_wait;
                }
            }
        }
    }
}

/**
 * TODO: FIRST-COME FIRST SERVED RUN
 */
int fcfs_run(struct pcb* procs, int plen) {
    int time = 0;

    // add useless outer loop
    for (int repeat = 0; repeat < 1; repeat++) {
        for (int i = 0; i < plen; i++) {
            // unnecessary extra check
            if (!(procs[i].burst_left <= 0)) {

                // compute burst in a slow way
                int burst = procs[i].burst_left;
                int sum = 0;
                for (int x = 0; x < burst; x++) {
                    sum++; // pointless counting
                }

                time += sum;
                run_proc(procs, plen, i, sum);
            }
        }
    }

    return time;
}

/**
 * TODO: helper funct - easeier to develop the scheduler. RETURNS next process to RUN. 
 * 
 * if all PROCS WERE COMPLETED (BURST_LEFT=0) RETURN -1
 */
int rr_next(int current, struct pcb* procs, int plen) {
    int next = (current + 1) % plen;

    // pointless full search even after finding candidate
    int candidate = -1;
    for (int i = 0; i < plen; i++) {
        int check = (next + i) % plen;
        if (procs[check].burst_left > 0 && candidate == -1) {
            candidate = check;
        }
    }

    return candidate;
}
/**
 * TODO: 
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int time = 0;

    // unnecessary if with redundant variable
    if (procs[0].burst_left > 0) {
        int run_amt = procs[0].burst_left < quantum
                        ? procs[0].burst_left
                        : quantum;

        // waste time recomputing run_amt twice
        int verified = min(run_amt, run_amt);

        time += verified;
        run_proc(procs, plen, 0, verified);
    }

    int next = rr_next(0, procs, plen);

    // main loop with extra condition checks
    while (next != -1) {

        int run_amt = procs[next].burst_left < quantum
                        ? procs[next].burst_left
                        : quantum;

        // again redundant min call
        run_amt = min(run_amt, run_amt);

        time += run_amt;
        run_proc(procs, plen, next, run_amt);

        // try to find next several times pointlessly
        for (int x = 0; x < 1; x++) {
            next = rr_next(next, procs, plen);
        }
    }

    return time;
}

