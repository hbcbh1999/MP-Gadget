#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "allvars.h"
#include "endrun.h"

/*  This function aborts the simulation.
 *
 *  if ierr > 0, the error is uncollective.
 *  if ierr <= 0, the error is 'collective',  only the root rank prints the error.
 */

void endrun(int ierr, const char * fmt, ...)
{
    va_list va;
    char buf[4096];
    va_start(va, fmt);
    vsprintf(buf, fmt, va);
    va_end(va);
    if(ierr > 0) {
        printf("Task %d: Error (%d) %s\n", ThisTask, ierr, buf);
        fflush(stdout);
        BREAKPOINT;
        MPI_Abort(MPI_COMM_WORLD, ierr);
    } else {
        if(ThisTask == 0) {
            printf("Collective Error (%d) %s\n", ierr, buf);
            fflush(stdout);
        }
        BREAKPOINT;
        MPI_Abort(MPI_COMM_WORLD, ierr);
    }
}

/*  This function writes a message.
 *
 *  if ierr > 0, the message is uncollective.
 *  if ierr <= 0, the message is 'collective', only the root rank prints the message. A barrier is applied.
 */

void message(int ierr, const char * fmt, ...)
{
    static double start = -1;
    if(start < 0) start = MPI_Wtime();

    va_list va;
    char buf[4096];
    va_start(va, fmt);
    vsprintf(buf, fmt, va);
    va_end(va);
    /* FIXME: deal with \n in the buf. */
    if(ierr > 0) {
        printf("[ %09.2f ] Task %d: %s", MPI_Wtime() - start, ThisTask, buf);
        fflush(stdout);
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        if(ThisTask == 0) {
            printf("[ %09.2f ] %s", MPI_Wtime() - start, buf);
            fflush(stdout);
        }
    }
}
