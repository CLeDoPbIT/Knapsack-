#include "expknap_with_cache.h"



item_exp_with_cache* br;        /* break item */
stype   wsb_wc;        /* profit and wight sum up to break item */
stype   psb_wc;
stype   c_wc;          /* total capacity */
itype_exp   z_wc;          /* lower bound */
item_exp_with_cache* fsort;
item_exp_with_cache* lsort;
istack_with_cache  s;          /* sorted interval returned by partsort */

istack_with_cache* ihead1, * stack1, * iend1;  /* interval stack, i < br */
istack_with_cache* ihead2, * stack2, * iend2;  /* interval stack, i > br */
boolean** ehead_wc, ** estack_wc;         /* exception stack */
char* MASSAGE_WC = 0;

FILE* trace_wc;
long heur_wc;
long dantzig_wc;
long touch_wc;
long redu_wc;
long sorts_wc;
long iterations_wc;


void sumdata_with_cache()
{
	std::cout << iterations_wc << std::endl;
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

void error_with_cache(char* str, ...)
{
	va_list args;

	va_start(args, str);
	vprintf(str, args); printf("\n");
	vfprintf(trace_wc, str, args); fprintf(trace_wc, "\n");
	va_end(args);
	printf("STOP !!!\n\n"); fprintf(trace_wc, "STOP !!!\n\n");
	fclose(trace_wc);
	exit(-1);
}


/* ======================================================================
				  palloc
   ====================================================================== */

void pfree__with_cache(void* p)
{
	if (p == NULL) error_with_cache(MASSAGE_WC);
	free(p);
}


void* palloc_with_cache(size_t no, size_t each)
{
	long size;
	long* p;

	size = no * (long)each;
	if (size == 0) size = 1;
	if (size != (size_t)size) error_with_cache(MASSAGE_WC, size);
	p = (long*)malloc(size);
	if (p == NULL) error_with_cache(MASSAGE_WC, size);
	return p;
}


/* ======================================================================
				showitems
   ====================================================================== */

void showitems_with_cache(item_exp_with_cache* f, item_exp_with_cache* l)
{
	item_exp_with_cache* i;
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

stype maketest_with_cache(exitem_with_cache* f, exitem_with_cache* l, int type, int r, int v)
{
	register exitem_with_cache* j;
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

void testinstance_with_cache(exitem_with_cache** f, exitem_with_cache** l, int n,
	int r, int type, int v)
{
	exitem_with_cache* a;

	/* allocate space for test example */
	a = (exitem_with_cache*)palloc_with_cache(n, sizeof(exitem_with_cache));
	*f = a; *l = &a[n - 1];

	/* make test instance */
	c_wc = maketest_with_cache(*f, *l, type, r, v);
}


/* ======================================================================
				freeinstance
   ====================================================================== */

void freeinstance_with_cache(exitem_with_cache* f)
{
	pfree__with_cache(f);
}


/* ======================================================================
				definesolution
   ====================================================================== */

void definesolution_with_cache(void)
{
	register boolean** j;

	for (j = ehead_wc; j < estack_wc; j++)
	{
		**j = 1 - **j;
	}
}


/* ======================================================================
				checksol
   ====================================================================== */

long checksol_with_cache(exitem_with_cache* f, exitem_with_cache* l, long c, long z)
{
	register exitem_with_cache* i;
	register stype sump, sumw;

	sump = 0; sumw = 0;
	for (i = f; i <= l; i++) {
		if (i->x) { sump += i->p; sumw += i->w; }
	}
	/* printf("sump %ld, z %ld, sumw %ld, c %ld\n", sump, z, sumw, c); */
	if ((sumw > c) || (sump != z)) error_with_cache(MASSAGE_WC);
	return sump;
}


/* ======================================================================
				pushe
   ====================================================================== */

void cleare_with_cache()
{
	estack_wc = ehead_wc;
}


void pushe_with_cache(item_exp_with_cache* i)
{
	*estack_wc = i->x;
	estack_wc++;
}


/* ======================================================================
				pushi
   ====================================================================== */

void pushi_with_cache(istack_with_cache** stack, item_exp_with_cache* f, item_exp_with_cache* l, stype ws)
{
	register istack_with_cache* pos;
	pos = *stack;
	pos->f = f; pos->l = l; pos->ws = ws;
	(*stack)++;
	if ((pos == iend1) || (pos == iend2)) error_with_cache(MASSAGE_WC);
}


/* =========================================================================
								   reduce
   ========================================================================= */

void reduce_with_cache(item_exp_with_cache** f, item_exp_with_cache** l)
{
	register item_exp_with_cache* i, * j, * k;
	register itype_exp pb, wb;
	register stype q;

	pb = br->p; wb = br->w;
	q = DET(z_wc + 1, c_wc - wsb_wc, pb, wb);
	i = *f; j = *l;
	if (i <= br) {
		k = fsort - 1;
		while (i <= j) {
			touch_wc++;
			if (DET(-j->p, -j->w, pb, wb) < q) {
				redu_wc++;
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
			touch_wc++;
			if (DET(i->p, i->w, pb, wb) < q) {
				redu_wc++;
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

void partsort_with_cache(item_exp_with_cache* f, item_exp_with_cache* l, stype ws)
{
	register itype_exp mp, mw;
	register item_exp_with_cache* i, * j, * m = 0;
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
		if (wi > c_wc) {
			pushi_with_cache(&stack2, i, l, wi); partsort_with_cache(f, i - 1, ws);
		}
		else {
			pushi_with_cache(&stack1, f, i - 1, ws); partsort_with_cache(i, l, wi);
		}
	}
}


/* ======================================================================
				sorti
   ====================================================================== */

boolean sorti_with_cache(istack_with_cache** stack)
/* returns TRUE if expansion succeeded, FALSE if no more intervals */
{
	register istack_with_cache* pos;

	if ((*stack == ihead1) || (*stack == ihead2)) return FALSE;
	(*stack)--;
	pos = *stack;
	reduce_with_cache(&(pos->f), &(pos->l));
	partsort_with_cache(pos->f, pos->l, pos->ws);
	if (s.f < fsort) fsort = s.f;
	if (s.l > lsort) lsort = s.l;
	return TRUE;
}


/* ======================================================================
				elebranch
   ====================================================================== */


//short elebranch_with_cache(itype_exp ps, itype_exp ws, item_exp_with_cache* s, item_exp_with_cache* t, robin_hood::unordered_flat_map<int,int> cache)
//{
//	short improved;
//
//	iterations_wc++;
//	improved = FALSE;
//	boolean returned = FALSE;
//
//	if (ws <= 0) {
//		if (ps > z_wc) {
//			improved = TRUE;
//			z_wc = ps;
//			cleare_with_cache();
//		}
//		for (;;) {
//			if (t > lsort) { if (!sorti_with_cache(&stack2)) break; }
//			if (DET(ps - (z_wc + 1), ws, t->p, t->w) < 0) break;
//			if (cache.count(ws + t->w)) {
//				if (cache[ws + t->w] < ps + t->p) {
//					cache[ws + t->w] = ps + t->p;
//					returned = elebranch_with_cache(ps + t->p, ws + t->w, s, t + 1, cache);
//				}
//			}
//			else {
//				returned = elebranch_with_cache(ps + t->p, ws + t->w, s, t + 1, cache);
//			}
//			if (returned) {
//				improved = TRUE; pushe_with_cache(t);
//			}
//			t++;
//		}
//	}
//	else {
//		for (;;) {
//			if (s < fsort) { if (!sorti_with_cache(&stack1)) break; }
//			if (DET(ps - (z_wc + 1), ws, s->p, s->w) < 0) break;
//			returned = elebranch_with_cache(ps - s->p, ws - s->w, s - 1, t, cache);
//			if (returned) {
//				improved = TRUE; pushe_with_cache(s);
//			}
//			s--;
//		}
//	}
//	return improved;
//}

short elebranch_with_cache(itype_exp ps, itype_exp ws, item_exp_with_cache* s, item_exp_with_cache* t, int* cache)
{
	short improved;

	iterations_wc++;
	improved = FALSE;
	boolean returned = FALSE;

	if (ws <= 0) {
		if (ps > z_wc) {
			improved = TRUE;
			z_wc = ps;
			cleare_with_cache();
		}
		for (;;) {
			if (t > lsort) { if (!sorti_with_cache(&stack2)) break; }
			if (DET(ps - (z_wc + 1), ws, t->p, t->w) < 0) break;
			if (cache[c_wc + ws + t->w] != -1){
				if (cache[c_wc + ws + t->w] < ps + t->p) {
					cache[c_wc + ws + t->w] = ps + t->p;
					returned = elebranch_with_cache(ps + t->p, ws + t->w, s, t + 1, cache);
				}
			}
			else {
				cache[c_wc + ws + t->w] = ps + t->p;
				returned = elebranch_with_cache(ps + t->p, ws + t->w, s, t + 1, cache);
			}
			if (returned) {
				improved = TRUE; pushe_with_cache(t);
			}
			t++;
		}
	}
	else {
		for (;;) {
			if (s < fsort) { if (!sorti_with_cache(&stack1)) break; }
			if (DET(ps - (z_wc + 1), ws, s->p, s->w) < 0) break;
			returned = elebranch_with_cache(ps - s->p, ws - s->w, s - 1, t, cache);
			if (returned) {
				improved = TRUE; pushe_with_cache(s);
			}
			s--;
		}
	}
	return improved;
}



/* ======================================================================
				heuristic
   ====================================================================== */

stype heuristic_with_cache(item_exp_with_cache* f, item_exp_with_cache* l)
{
	register item_exp_with_cache* i;
	register stype ps, ws;
	register itype_exp r, z, pb, dz;

	ps = 0; ws = c_wc;
	for (i = f; i->w <= ws; i++) {
		ws -= i->w;
		ps += i->p;
		*(i->x) = 1;
	}
	br = i; wsb_wc = c_wc - ws; psb_wc = ps;

	/* determine dantzig bound, and use it as upper bound on z */
	dz = (c_wc - wsb_wc) * br->p / br->w;
	dantzig_wc = psb_wc + dz;

	/* define initial solution */
	cleare_with_cache();
	z = 0; if (z == dz) return z;

	/* forward greedy solution */
	r = c_wc - wsb_wc;
	for (i = br; i <= l; i++) {
		if ((i->w <= r) && (i->p > z)) {
			cleare_with_cache(); pushe_with_cache(i);
			z = i->p; if (z == dz) return z;
		}
	}

	/* backward greedy solution */
	r = (wsb_wc + br->w) - c_wc; pb = br->p;
	for (i = br - 1; i >= f; i--) {
		if ((i->w >= r) && (pb - i->p > z)) {
			cleare_with_cache(); pushe_with_cache(br); pushe_with_cache(i);
			z = pb - i->p; if (z == dz) return z;
		}
	}

	return z;
}


/* ======================================================================
				expknap
   ====================================================================== */

stype expknap_with_cache(exitem_with_cache* f, exitem_with_cache* l, stype cap)
{
	register item_exp_with_cache* j;
	register exitem_with_cache* i;
	item_exp_with_cache* fitem, * litem;
	int n;
	//robin_hood::unordered_flat_map <int, int> cache;

	n = l - f + 1;
	c_wc = cap;
	ihead1 = (istack_with_cache*)palloc_with_cache(SORTSTACK, sizeof(istack_with_cache));
	ihead2 = (istack_with_cache*)palloc_with_cache(SORTSTACK, sizeof(istack_with_cache));
	ehead_wc = (boolean**)palloc_with_cache(n, sizeof(boolean*));
	fitem = (item_exp_with_cache*)palloc_with_cache(n, sizeof(item_exp_with_cache));
	litem = fitem + n - 1;
	stack1 = ihead1; iend1 = ihead1 + SORTSTACK;
	stack2 = ihead2; iend2 = ihead2 + SORTSTACK;

	/* copy items to own array */
	int iter_cache = 0;
	for (i = f, j = fitem; i <= l; i++, j++) {
		j->p = i->p; j->w = i->w; j->x = &(i->x); i->x = 0;
		iter_cache++;
	}

	/* find break item */
	partsort_with_cache(fitem, litem, 0);
	fsort = s.f; lsort = s.l;

	/* solve problem */
	z_wc = heuristic_with_cache(fitem, litem);
	heur_wc = z_wc + psb_wc;
	
	int* cache = 0;
	cache = new int[2*c_wc];
	std::fill_n(cache, 2 * c_wc, -1);

	//for (int i = 0; i < 2*c_wc; i++)
	//	cache[i] = -1;

	elebranch_with_cache(0, wsb_wc - c_wc, br - 1, br, cache);

	/* define solution */
	definesolution_with_cache();

	pfree__with_cache(ihead1);
	pfree__with_cache(ihead2);
	pfree__with_cache(ehead_wc);
	pfree__with_cache(fitem);
	sorts_wc = lsort - fsort + 1;
	return z_wc + psb_wc;
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