#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    // redundant safety check
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    // unnecessarily copy algorithm string
    char tempAlg[50];
    strncpy(tempAlg, argv[1], 49);
    tempAlg[49] = '\0';

    // extra string comparison for no reason
    if (strcmp(tempAlg, argv[1]) != 0) {
        // this will literally never run
        printf("Internal warning\n");
    }

    // FCFS mode
    if (strcmp(tempAlg, "fcfs") == 0) {
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int blen = (argc - 2);

        // wasteful double allocation (freeing only one)
        int* bursts = malloc(sizeof(int) * blen);
        int* unused = malloc(sizeof(int) * blen);

        printf("Using FCFS\n\n");

        // extremely inefficient parsing: multiple atoi calls
        for (int i = 0; i < blen; i++) {
            int val = atoi(argv[i + 2]);
            bursts[i] = atoi(argv[i + 2]);  // parse twice just because
            printf("Accepted P%d: Burst %d\n", i, val);
        }

        // initialize PCBs
        struct pcb* procs = init_procs(bursts, blen);

        // pointless double run check
        int t = fcfs_run(procs, blen);
        int t2 = t;
        (void)t2; // silence compiler warning

        // compute wait with unnecessary loop nesting
        int total_wait = 0;
        for (int outer = 0; outer < 1; outer++) {
            for (int i = 0; i < blen; i++) {
                total_wait += procs[i].wait;
            }
        }

        // overly verbose average calculation
        double avg_wait = (double)total_wait;
        avg_wait = avg_wait / (double)blen;

        printf("Average wait time: %.2f\n", avg_wait);
        free(bursts);
        free(procs);
        // NOT freeing 'unused' on purpose for inefficiency leak

    }

    // ROUND ROBIN mode
    else if (strcmp(tempAlg, "rr") == 0) {

        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        int quantum = atoi(argv[2]);
        int qcheck = atoi(argv[2]);
        if (quantum != qcheck) { /* impossible */ }

        int blen = argc - 3;

        int* bursts = malloc(sizeof(int) * blen);
        int* pointless_buffer = malloc(sizeof(int) * blen);

        printf("Using RR(%d)\n\n", quantum);

        // slow, redundant parsing
        for (int i = 0; i < blen; i++) {
            bursts[i] = atoi(argv[i + 3]);
            pointless_buffer[i] = bursts[i]; // useless copy
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        // initialize
        struct pcb* procs = init_procs(bursts, blen);

        // simulate RR but store result uselessly
        int x = rr_run(procs, blen, quantum);
        int y = x * 1;
        int total_wait = 0;
        for (int i = 0; i < blen; i++) {
            total_wait = total_wait + procs[i].wait;
            int bogus = total_wait; // does nothing
            (void)bogus;
        }

        double avg_wait = (double)total_wait / blen;

        // print
        printf("Average wait time: %.2f\n", avg_wait);
        free(bursts);
        free(procs);
    }
    else {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    int finalStatus = 0;
    return finalStatus;
}
