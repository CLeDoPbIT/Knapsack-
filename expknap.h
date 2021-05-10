/* ======================================================================
		 EXPKNAP.c, David Pisinger   march 1992, modified april 1994
   ====================================================================== */

   /* This is the C-code corresponding to the paper:
	*
	*   D. Pisinger
	*   An expanding-core algorithm for the exact 0-1 Knapsack Problem
	*   European Journal of Operational Research, 87 (1995), 175-187
	*
	* Further details on the project can also be found in
	*
	*   D. Pisinger
	*   Algorithms for Knapsack Problems
	*   Report 95/1, DIKU, University of Copenhagen
	*   Universitetsparken 1
	*   DK-2100 Copenhagen
	*
	* The current code is intended for performing extensive tests with
	* randomly generated instances. It should however be easy to derive
	* the "plain" expknap algorithm from the listing by stripping several
	* test routines.
	*
	* The code has been tested on a hp9000/735, and conforms with the
	* ANSI-C standard apart from some of the timing routines (which may
	* be removed). To compile the code use:
	*
	*   cc -Aa -O -o expknap expknap.c -lm
	*
	* The code is run by issuing the command
	*
	*   expknap n r type
	*
	* where n: number of items,
	*       r: range of coefficients,
	*       type: 1=uncorr., 2=weakly corr., 3=strongly corr., 4=subset sum
	* output will be appended to the file "trace.exp".
	*
	* Please do not re-distribute. A new copy can be obtained by contacting
	* the author at the adress below. Errors and questions are refered to:
	*
	*   David Pisinger, associate professor
	*   DIKU, University of Copenhagen,
	*   Universitetsparken 1,
	*   DK-2100 Copenhagen.
	*   e-mail: pisinger@diku.dk
	*   fax: +45 35 32 14 01
	*/


	/* ======================================================================
								  definitions
	   ====================================================================== */

#define TESTS    1 /* Nuber of test instances to be generated */
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <vector>
#include <string>

	   /* ======================================================================
							macros
		  ====================================================================== */

#define srand(x)     srand(x)
#define random(x)    (rand() % (x))

#define DET(a1, a2, b1, b2)        ((a1) * (long) (b2) - (a2) * (long) (b1))
#define SWAP(a, b)             { register item_exp t; t = *a; *a = *b; *b = t; }
#define NO(f,i)                                            ((int) ((i+1)-f))
#define TRUE  1
#define FALSE 0
#define SORTSTACK 200
#define _CRT_SECURE_NO_WARNINGS
/* ======================================================================
				 type declarations
   ====================================================================== */

typedef int   boolean; /* boolean variables */
typedef short itype_exp;   /* item profits and weights */
typedef long  stype;   /* sum of pofit or weight */

/* item as seen externally */
typedef struct {
	itype_exp   p;     /* profit */
	itype_exp   w;     /* weight */
	boolean x;     /* solution variable */
} exitem;

/* item as seen internally */
typedef struct {
	itype_exp   p;     /* profit */
	itype_exp   w;     /* weight */
	boolean* x;    /* pointer to original solution variable */
} item_exp;

/* interval stack */
typedef struct istk {
	item_exp* f;
	item_exp* l;
	stype   ws;
} istack;


/* ======================================================================
				 global variables
   ====================================================================== */



/* ======================================================================
				 test variables
   ====================================================================== */



/* ======================================================================
						timing routines for hp9000/735
   ====================================================================== */

#define  _INCLUDE_POSIX_SOURCE
#include <io.h>


   //struct tms timestart, timeend;

   //void starttime(void)
   //{
   //    times(&timestart);
   //}
   //
   //void endtime(long *time)
   //{
   //  double t1, t2;
   //  times(&timeend);
   //  t1 = (double) (timeend.tms_utime-timestart.tms_utime) / sysconf(_SC_CLK_TCK);
   //  t2 = (double) (timeend.tms_stime-timestart.tms_stime) / sysconf(_SC_CLK_TCK);
   //  *time = t1 * 1000;
   //}


   /* ======================================================================
									  sumdata
	  ====================================================================== */

void sumdata(int n1, int r1, int type1, long iterations, long heur, long z,
	long c, long dantzig, long touch, long redu, long sorts);


/* ======================================================================
						 error
   ====================================================================== */

void error(char* str, ...);
/* ======================================================================
				  palloc
   ====================================================================== */

void pfree__(void* p);

void* palloc(size_t no, size_t each);


/* ======================================================================
				showitems
   ====================================================================== */

void showitems(item_exp* f, item_exp* l);


/* ======================================================================
				maketest
   ====================================================================== */

stype maketest(exitem* f, exitem* l, int type, int r, int v);


/* ======================================================================
				testinstance
   ====================================================================== */

void testinstance(exitem** f, exitem** l, int n,
	int r, int type, int v);

/* ======================================================================
				freeinstance
   ====================================================================== */

void freeinstance(exitem* f);


/* ======================================================================
				definesolution
   ====================================================================== */

void definesolution(void);


/* ======================================================================
				checksol
   ====================================================================== */

long checksol(exitem* f, exitem* l, long c, long z);

/* ======================================================================
				pushe
   ====================================================================== */

void cleare();


void pushe(item_exp* i);


/* ======================================================================
				pushi
   ====================================================================== */

void pushi(istack** stack, item_exp* f, item_exp* l, stype ws);


/* =========================================================================
								   reduce
   ========================================================================= */

void reduce(item_exp** f, item_exp** l);


/* ======================================================================
				partsort
   ====================================================================== */

void partsort(item_exp* f, item_exp* l, stype ws);


/* ======================================================================
				sorti
   ====================================================================== */

boolean sorti(istack** stack);
/* returns TRUE if expansion succeeded, FALSE if no more intervals */



/* ======================================================================
				elebranch
   ====================================================================== */

short elebranch(itype_exp ps, itype_exp ws, item_exp* s, item_exp* t);


/* ======================================================================
				heuristic
   ====================================================================== */

stype heuristic(item_exp* f, item_exp* l);


/* ======================================================================
				expknap
   ====================================================================== */

stype expknap(exitem* f, exitem* l, stype cap);