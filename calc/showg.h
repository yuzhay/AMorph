#pragma once
#include "windows.h"

/*
 Version 1.1: Fixed sparse6 input for powers of 2.  May 9, 1998.
 Version 1.3: Declared errno according to ISO C.  August 22, 1998.
 Version 1.4: Change name of getline() so that broken compilers
                which define the GNU function without being asked
		don't cause a conflict.   June 16, 2006.
 Version 1.5: Use function prototypes.  Avoid errno.  Sep 19, 2007.
 Version 1.6: Very minor tweaks.  Hope you all have string.h. Sep 6, 2009.
*/

#define USAGE "showg [-p#:#l#o#Ftq] [-a|-A|-c|-d|-e] [infile [outfile]]"

#define HELPTEXT \
" Write graphs in human-readable format.\n\
\n\
   infile is the input file in graph6 or sparse6 format\n\
   outfile is the output file\n\
   Defaults are standard input and standard output.\n\
\n\
    -p#, -p#:#, -p#-# : only display one graph or a sequence of\n\
          graphs.  The first graph is number 1.  A second number\n\
          which is empty or zero means infinity.\n\
\n\
    -a  : write the adjacency matrix\n\
    -A  : same as -a with a space between entries\n\
    -d  : write output to satisfy dreadnaut\n\
    -c  : write compact dreadnaut form with minimal line-breaks\n\
    -e  : write a list of edges, preceded by the order and the\n\
          number of edges\n\
\n\
    -o# : specify number of first vertex (default is 0)\n\
    -t  : write upper triangle only (affects -a, -A, -d and default)\n\
    -F  : write a form-feed after each graph except the last\n\
    -l# : specify screen width limit (default 78, 0 means no limit)\n\
          This is not currently implemented with -a or -A.\n\
    -q  : suppress auxiliary output\n\
\n\
    -a, -A, -c, -d and -e are incompatible.\n"

#include <stdio.h>
#include <string.h>

/* gtools.h : General header for gtools programs. */

#ifndef MAXN 
#define MAXN  0
#endif
#define G6LEN(n)  (((n)*((n)-1)/2+5)/6+(n<=SMALLN?1:4))
		/* Exact length of graph6 code except for \n\0 */

/* Remove errno: too hard to get portable without configuration
 * #if defined(__unix) || defined(__unix__) || defined(unix) || \
    defined(__ppc__)
#include <errno.h>
#else
int errno = 0;
#endif
#define ABORT(msg) {if (errno != 0) perror(msg); exit(1);}
*/

extern long ftell(FILE*);

#define BIAS6 63
#define MAXBYTE 126
#define SMALLN 62
#define TOPBIT6 32
#define C6MASK 63

#define GRAPH6_HEADER ">>graph6<<"
#define SPARSE6_HEADER ">>sparse6<<"

#define GRAPH6         1
#define SPARSE6        2
#define UNKNOWN_TYPE 256
#define HAS_HEADER   512

#define ARG_OK 0
#define ARG_MISSING 1
#define ARG_TOOBIG 2
#define ARG_ILLEGAL 3

#define MAXARG 2000000000L
#define NOLIMIT (MAXARG+1L)

#define SWBOOLEAN(c,bool) if (sw==c) bool=TRUE;
#define SWINT(c,bool,val,id) if (sw==c) \
        {bool=TRUE;arg_int(&arg,&val,id);}
#define SWRANGE(c,bool,val1,val2,id) if (sw==c) \
	{bool=TRUE;arg_range(&arg,&val1,&val2,id);}

#define FREES free
#define ALLOCS calloc

#define DYNALLSTAT(type,name,name_sz) static type *name; static size_t name_sz=0
#define DYNALLOC1(type,name,name_sz,sz,msg) \
 if ((size_t)(sz) > name_sz) \
 { if (name_sz) FREES(name); name_sz = (sz); \
 if ((name=(type*)ALLOCS(sz,sizeof(type))) == NULL) alloc_error(msg);}
#define DYNALLOC2(type,name,name_sz,sz1,sz2,msg) \
 if ((size_t)(sz1)*(size_t)(sz2) > name_sz) \
 { if (name_sz) FREES(name); name_sz = (size_t)(sz1)*(size_t)(sz2); \
 if ((name=(type*)ALLOCS((sz1),(sz2)*sizeof(type))) == NULL) alloc_error(msg);}
#define DYNFREE(name,name_sz) if (name_sz) {FREES(name); name_sz = 0;}
#define DYNREALLOC(type,name,name_sz,sz,msg) \
 {if ((size_t)(sz) > name_sz) \
 { if ((name = (type*)realloc(name,(sz)*sizeof(type))) == NULL) \
      alloc_error(msg); else name_sz = (sz);}}

#define alloc_error gt_abort

#ifdef __STDC__
	#include <stddef.h>
	#include <stdlib.h>
#else
	#include <sys/types.h>
	extern char *calloc();
	extern char *malloc();
	extern char *realloc();
#endif

#ifdef __alpha
	typedef unsigned int setword;
#else
	typedef unsigned long setword;
#endif
typedef setword set;
typedef setword graph;
//typedef int boolean;

static setword bit[32]=
  {020000000000,010000000000,04000000000,02000000000,
   01000000000,0400000000,0200000000,0100000000,040000000,
   020000000,010000000,04000000,02000000,01000000,0400000,
   0200000,0100000,040000,020000,010000,04000,02000,01000,
   0400,0200,0100,040,020,010,04,02,01};
static int leftbit[] =
  {8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,
   3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int labelorg = 0;

#define WORDSIZE 32
#define FIRSTBIT(x) ((x) & 037777600000 ? ((x) & 037700000000 ? \
                     leftbit[((x)>>24) & 0377] : 8+leftbit[(x)>>16]) \
                    : ((x) & 0177400 ? 16+leftbit[(x)>>8] : 24+leftbit[x]))
#define BITMASK(x)  (017777777777 >> (x)) /* setword whose rightmost
  WORDSIZE-x-1 (numbered) bits are 1 and the rest 0 (0 <= x < WORDSIZE) */
#define TIMESWORDSIZE(w) ((w)<<5)
#define SETWD(pos) ((pos)>>5)
#define SETBT(pos) ((pos)&037)
#define ISELEMENT(setadd,pos)  (((setadd)[SETWD(pos)] & bit[SETBT(pos)]) != 0)
#define ADDELEMENT(setadd,pos)  ((setadd)[SETWD(pos)] |= bit[SETBT(pos)])
#define GRAPHROW(g,v,m) ((set*)(g) + (long)(v) * (long)(m))

#define FALSE 0
#define TRUE  1

/************************************************************************/
#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#ifndef SEEK_CUR
#define SEEK_CUR SEEK_CURRENT
#endif

static long ogf_linelen;

/************************************************************************/


/************************************************************************/

#define LABELORG 0   /* number of first vertex (any integer >= 0) */
#define LINELEN 78   /* max characters per line (0 = no limit) */

static FILE *infile,*outfile;
static long nin;
extern int labelorg;


/*opens and positions a file for reading graphs.*/
 FILE* opengraphfile(char *filename, int *codetype, int assumefixed, long position);

/* read graph into nauty format */
graph* readg(FILE *f, graph *g, int reqm, int *pm, int *pn);
