#include "expknap.h"



item_exp* br;        /* break item */
stype   wsb;        /* profit and wight sum up to break item */
stype   psb;
stype   c;          /* total capacity */
itype_exp   z;          /* lower bound */
item_exp* fsort;
item_exp* lsort;
istack  s;          /* sorted interval returned by partsort */

istack* ihead1, * stack1, * iend1;  /* interval stack, i < br */
istack* ihead2, * stack2, * iend2;  /* interval stack, i > br */
boolean** ehead, ** estack;         /* exception stack */
char* MASSAGE = 0;

FILE* trace;
long heur;
long dantzig;
long touch;
long redu;
long sorts;
long iterations;


long  sumdata()
{
	return iterations;
	//std::cout << iterations << std::endl;
	//static long n;
	//static long r;
	//static long t;
	//static long iterates = 0;
	//static long touched = 0;
	//static long simpred = 0;
	//static long coresize = 0;
	//static long greedygap = 0;
	//static long gap = 0;
	//static long tottime = 0;
	//static long zsum = 0;
	//static long csum = 0;
	//static double sqtime = 0.0;
	//double mean, variance, stddev;

	//mean = tottime / (1000 * (double)TESTS);
	//variance = sqtime / TESTS - mean * mean;
	//stddev = sqrt(variance);
	//fprintf(trace, "n           = %ld\n", n);
	//fprintf(trace, "r           = %ld\n", r);
	//fprintf(trace, "t           = %ld\n", t);
	//fprintf(trace, "iterations  = %.0lf\n", iterations);
	//fprintf(trace, "touched     = %.1lf\n", touched / (double)TESTS);
	//fprintf(trace, "touchedpct  = %.1lf\n", 100 * touched / ((double)n * TESTS));
	//fprintf(trace, "simpreduced = %.1lf\n", simpred / (double)TESTS);
	//fprintf(trace, "coresize    = %.0lf\n", coresize / (double)TESTS);
	//fprintf(trace, "corepct     = %.2lf\n", 100 * coresize / ((double)n * TESTS));
	//fprintf(trace, "greedygap   = %.1lf\n", greedygap / (double)TESTS);
	//fprintf(trace, "gap         = %.1lf\n", gap / (double)TESTS);
	//fprintf(trace, "zsum        = %.0lf\n", zsum / (double)1);
	//fprintf(trace, "csum        = %.0lf\n", csum / (double)1);
	//fprintf(trace, "time        = %.2lf\n", mean / (double)1);
	//fprintf(trace, "variance    = %.2lf\n", variance / (double)1);
	//fprintf(trace, "stddev      = %.2lf\n", stddev / (double)1);
	
}


/* ======================================================================
						 error
   ====================================================================== */

void error(char* str, ...)
{
	va_list args;

	va_start(args, str);
	vprintf(str, args); printf("\n");
	vfprintf(trace, str, args); fprintf(trace, "\n");
	va_end(args);
	printf("STOP !!!\n\n"); fprintf(trace, "STOP !!!\n\n");
	fclose(trace);
	exit(-1);
}


/* ======================================================================
				  palloc
   ====================================================================== */

void pfree__(void* p)
{
	if (p == NULL) error(MASSAGE);
	free(p);
}


void* palloc(size_t no, size_t each)
{
	long size;
	long* p;

	size = no * (long)each;
	if (size == 0) size = 1;
	if (size != (size_t)size) error(MASSAGE, size);
	p = (long*)malloc(size);
	if (p == NULL) error(MASSAGE, size);
	return p;
}


/* ======================================================================
				showitems
   ====================================================================== */

void showitems(item_exp* f, item_exp* l)
{
	item_exp* i;
	stype ps, ws;

	printf("showitems");
	ps = 0; ws = 0;
	for (i = f; i <= l; i++) {
		printf("%d: (%2hd,%2hd) %8lf %1hd\n",
			(int)((i + 1) - f), i->p, i->w, i->p / (double)i->w, *(i->x));
		if (i->x) { ps += i->p; ws += i->w; }
	}
	printf("showitems (%ld,%ld)", ps, ws);
}


/* ======================================================================
				maketest
   ====================================================================== */

stype maketest(exitem* f, exitem* l, int type, int r, int v)
{
	register exitem* j;
	register stype sum;
	stype c;
	itype_exp r1;

	sum = 0; r1 = r / 10;
	for (j = f; j <= l; j++) {

		j->w = random(r) + 1;
		switch (type) {
		case 1: j->p = random(r) + 1;
			break;
		case 2: j->p = random(2 * r1 + 1) + j->w - r1;
			if (j->p <= 0) j->p = 1;
			break;
		case 3: j->p = j->w + 10;
			break;
		case 4: j->p = j->w;
			break;
		}

		sum += j->w;
	}
	c = sum / 2;
	return c;
}


/* ======================================================================
				testinstance
   ====================================================================== */

void testinstance(exitem** f, exitem** l, int n,
	int r, int type, int v)
{
	exitem* a;

	/* allocate space for test example */
	a = (exitem*)palloc(n, sizeof(exitem));
	*f = a; *l = &a[n - 1];

	/* make test instance */
	c = maketest(*f, *l, type, r, v);
}


/* ======================================================================
				freeinstance
   ====================================================================== */

void freeinstance(exitem* f)
{
	pfree__(f);
}


/* ======================================================================
				definesolution
   ====================================================================== */

void definesolution(void)
{
	register boolean** j;

	for (j = ehead; j < estack; j++)
	{
		**j = 1 - **j;
	}
}


/* ======================================================================
				checksol
   ====================================================================== */

long checksol(exitem* f, exitem* l, long c, long z)
{
	register exitem* i;
	register stype sump, sumw;

	sump = 0; sumw = 0;
	for (i = f; i <= l; i++) {
		if (i->x) { sump += i->p; sumw += i->w; }
	}
	/* printf("sump %ld, z %ld, sumw %ld, c %ld\n", sump, z, sumw, c); */
	if ((sumw > c) || (sump != z)) error(MASSAGE);
	return sump;
}


/* ======================================================================
				pushe
   ====================================================================== */

void cleare()
{
	estack = ehead;
}


void pushe(item_exp* i)
{
	*estack = i->x;
	estack++;
}


/* ======================================================================
				pushi
   ====================================================================== */

void pushi(istack** stack, item_exp* f, item_exp* l, stype ws)
{
	register istack* pos;
	pos = *stack;
	pos->f = f; pos->l = l; pos->ws = ws;
	(*stack)++;
	if ((pos == iend1) || (pos == iend2)) error(MASSAGE);
}


/* =========================================================================
								   reduce
   ========================================================================= */

void reduce(item_exp** f, item_exp** l)
{
	register item_exp* i, * j, * k;
	register itype_exp pb, wb;
	register stype q;

	pb = br->p; wb = br->w;
	q = DET(z + 1, c - wsb, pb, wb);
	i = *f; j = *l;
	if (i <= br) {
		k = fsort - 1;
		while (i <= j) {
			touch++;
			if (DET(-j->p, -j->w, pb, wb) < q) {
				redu++;
				SWAP(i, j); i++;       /* not feasible */
			}
			else {
				SWAP(j, k); j--; k--;  /* feasible */
			}
		}
		if (k == fsort - 1) { SWAP(*f, k); k--; } /* return at least one */
		*l = fsort - 1; *f = k + 1;
	}
	else {
		k = lsort + 1;
		while (i <= j) {
			touch++;
			if (DET(i->p, i->w, pb, wb) < q) {
				redu++;
				SWAP(i, j); j--;       /* not feasible */
			}
			else {
				SWAP(i, k); i++; k++;  /* feasible */
			}
		}
		if (k == lsort + 1) { SWAP(*l, k); k++; }  /* return at least one */
		*f = lsort + 1; *l = k - 1;
	}
}


/* ======================================================================
				partsort
   ====================================================================== */

void partsort(item_exp* f, item_exp* l, stype ws)
{
	register itype_exp mp, mw;
	register item_exp* i, * j, * m = 0;
	register stype wi;
	int d;

	d = l - f + 1;
	if (d > 1) {
		m = f + d / 2;
		if (DET(f->p, f->w, m->p, m->w) < 0) SWAP(f, m);
		if (d > 2) {
			if (DET(m->p, m->w, l->p, l->w) < 0) {
				SWAP(m, l);
				if (DET(f->p, f->w, m->p, m->w) < 0) SWAP(f, m);
			}
		}
	}
	if (d <= 3) {
		s.f = f; s.l = l; s.ws = ws;
	}
	else {
		mp = m->p; mw = m->w;
		i = f; j = l; wi = ws;
		for (;;) {
			do { wi += i->w; i++; } while (DET(i->p, i->w, mp, mw) > 0);
			do { j--; } while (DET(j->p, j->w, mp, mw) < 0);
			if (i > j) break;
			SWAP(i, j);
		}
		if (wi > c) {
			pushi(&stack2, i, l, wi); partsort(f, i - 1, ws);
		}
		else {
			pushi(&stack1, f, i - 1, ws); partsort(i, l, wi);
		}
	}
}


/* ======================================================================
				sorti
   ====================================================================== */

boolean sorti(istack** stack)
/* returns TRUE if expansion succeeded, FALSE if no more intervals */
{
	register istack* pos;

	if ((*stack == ihead1) || (*stack == ihead2)) return FALSE;
	(*stack)--;
	pos = *stack;
	reduce(&(pos->f), &(pos->l));
	partsort(pos->f, pos->l, pos->ws);
	if (s.f < fsort) fsort = s.f;
	if (s.l > lsort) lsort = s.l;
	return TRUE;
}


/* ======================================================================
				elebranch
   ====================================================================== */


short elebranch(itype_exp ps, itype_exp ws, item_exp* s, item_exp* t)
{
	short improved;
	//std::cout << iterations << " " << z << " "<<  z + psb << " " << t->w << " "<< s->w <<std::endl;
	iterations++;
	improved = FALSE;
	if (ws <= 0) {
		if (ps > z) {
			improved = TRUE;
			z = ps;
			cleare();
		}
		for (;;) {
			if (t > lsort) { if (!sorti(&stack2)) break; }
			if (DET(ps - (z + 1), ws, t->p, t->w) < 0) break;
			if (elebranch(ps + t->p, ws + t->w, s, t + 1)) {
				improved = TRUE; pushe(t);
			}
			t++;
		}
	}
	else {
		for (;;) {
			if (s < fsort) { if (!sorti(&stack1)) break; }
			if (DET(ps - (z + 1), ws, s->p, s->w) < 0) break;
			if (elebranch(ps - s->p, ws - s->w, s - 1, t)) {
				improved = TRUE; pushe(s);
			}
			s--;
		}
	}
	return improved;
}


/* ======================================================================
				heuristic
   ====================================================================== */

stype heuristic(item_exp* f, item_exp* l)
{
	register item_exp* i;
	register stype ps, ws;
	register itype_exp r, z, pb, dz;

	ps = 0; ws = c;
	for (i = f; i->w <= ws; i++) {
		ws -= i->w;
		ps += i->p;
		*(i->x) = 1;
	}
	br = i; wsb = c - ws; psb = ps;

	/* determine dantzig bound, and use it as upper bound on z */
	dz = (c - wsb) * br->p / br->w;
	dantzig = psb + dz;

	/* define initial solution */
	cleare();
	z = 0; if (z == dz) return z;

	/* forward greedy solution */
	r = c - wsb;
	for (i = br; i <= l; i++) {
		if ((i->w <= r) && (i->p > z)) {
			cleare(); pushe(i);
			z = i->p; if (z == dz) return z;
		}
	}

	/* backward greedy solution */
	r = (wsb + br->w) - c; pb = br->p;
	for (i = br - 1; i >= f; i--) {
		if ((i->w >= r) && (pb - i->p > z)) {
			cleare(); pushe(br); pushe(i);
			z = pb - i->p; if (z == dz) return z;
		}
	}

	return z;
}


/* ======================================================================
				expknap
   ====================================================================== */

std::pair <long, long> expknap(exitem* f, exitem* l, stype cap)
{
	register item_exp* j;
	register exitem* i;
	item_exp* fitem, * litem;
	int n;

	n = l - f + 1;
	c = cap;
	ihead1 = (istack*)palloc(SORTSTACK, sizeof(istack));
	ihead2 = (istack*)palloc(SORTSTACK, sizeof(istack));
	ehead = (boolean**)palloc(n, sizeof(boolean*));
	fitem = (item_exp*)palloc(n, sizeof(item_exp));
	litem = fitem + n - 1;
	stack1 = ihead1; iend1 = ihead1 + SORTSTACK;
	stack2 = ihead2; iend2 = ihead2 + SORTSTACK;

	/* copy items to own array */
	for (i = f, j = fitem; i <= l; i++, j++) {
		j->p = i->p; j->w = i->w; j->x = &(i->x); i->x = 0;
	}

	/* find break item */
	partsort(fitem, litem, 0);
	fsort = s.f; lsort = s.l;

	/* solve problem */
	z = heuristic(fitem, litem);
	heur = z + psb;

	int* cache = 0;
	cache = new int[2 * c];
	//std::fill_n(cache, 2 * c, -1);
	memset(cache, -1, 2 * c);


	elebranch(0, wsb - c, br - 1, br);

	/* define solution */
	definesolution();

	pfree__(ihead1);
	pfree__(ihead2);
	pfree__(ehead);
	pfree__(fitem);
	sorts = lsort - fsort + 1;
	return std::make_pair(z + psb, iterations);
}


/* ======================================================================
				main
   ====================================================================== */

//void main(int argc, char* argv[])
//{
//	exitem* f, * l;
//	stype z;
//	long time;
//	int n, r, type, v;
//
//	//if (argc == 4) {
//	//	n = atoi(argv[1]);
//	//	r = atoi(argv[2]);
//	//	type = atoi(argv[3]);
//	//	printf("Expknap %d, %d, %d\n", n, r, type);
//	//}
//	//else {
//	//	printf("Expknap\n");
//	//	printf("n = ");
//	//	scanf("%d", &n);
//	//	printf("r = ");
//	//	scanf("%d", &r);
//	//	printf("t = ");
//	//	scanf("%d", &type);
//	//}
//
//	//trace = fopen("trace.exp", "a");
//
//	n = 5;
//	r = 11;
//	type = 1;
//
//	for (v = 1; v <= TESTS; v++) {
//		srand(v);
//		iterations = 0;
//		redu = 0;
//		touch = 0;
//		testinstance(&f, &l, n, r, type, v);
//
//		std::vector<exitem> task;
//		for (exitem* e = f; e <= l; e++)
//		{
//			task.push_back(*e);
//		}
//
//		// starttime();
//		z = expknap(f, l, c);
//
//		std::vector<exitem> sol;
//		for (exitem* e = f; e <= l; e++)
//		{
//			sol.push_back(*e);
//		}
//		//  endtime(&time);
//
//		  /* check solution */
//		checksol(f, l, c, z);
//
//		freeinstance(f);
//		sumdata(n, r, type, iterations, heur, z,
//			c, dantzig, touch, redu, sorts);
//	}
//	sumdata(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//	fclose(trace);
//}