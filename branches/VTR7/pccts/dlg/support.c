/*
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * DLG 1.33
 * Will Cohen
 * With mods by Terence Parr; AHPCRC, University of Minnesota
 * 1989-1995
 */

#include <stdio.h>
#include <string.h>
#include "dlg.h"
#ifdef MEMCHK
#include "trax.h"
#else
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif /* __STDC__ */
#endif

int	err_found = 0;			/* indicates whether problem found */

internal_error(s,file,line)
char *s,*file;
int line;
{
	fprintf(stderr,s,file,line);
	exit(PCCTS_EXIT_FAILURE);
}

char *dlg_malloc(bytes,file,line)
int bytes;
char *file;
int line;
{
	char *t;

	t = (char *) malloc(bytes);
	if (!t){
		/* error */
		internal_error("%s(%d): unable to allocate memory\n",
			file,line);
	}
	return t;
}


char *dlg_calloc(n,bytes,file,line)
int n,bytes;
char *file;
int line;
{
	char *t;

	t = (char *) calloc(n,bytes);
	if (!t){
		/* error */
		internal_error("%s(%d): unable to allocate memory\n",
			file,line);
	}
	return t;
}


FILE *read_stream(name)
char *name;
{
	FILE *f;

	if (name){
		if (name[0] == '-') {
			fprintf(stderr, "dlg: invalid option: '%s'\n", name);
			f = NULL;
		}else{
			f = fopen(name, "r");
			if (f == NULL){
				/* couldn't open file */
				fprintf(stderr,
					"dlg: Warning: Can't read file %s.\n",
					name);
			}
		}
	}else{
		/* open stdin if nothing there */
		f = stdin;
	}
	return f;
}

FILE *write_stream(name)
char *name;
{
	FILE *f;

	if (name){
		if (name[0] == '-') {
			fprintf(stderr, "dlg: invalid option: '%s'\n", name);
			f = NULL;
		}else{
			f = fopen(OutMetaName(name), "w");
			if (f == NULL){
				/* couldn't open file */
				fprintf(stderr,
					"dlg: Warning: Can't write to file %s.\n",
					name);
			}
			else
#ifdef SPECIAL_FOPEN
                special_fopen_actions(OutMetaName(name));	/* MR1 */
#else
		;						/* MR1 */
#endif
		}
	}else{
		/* open stdout if nothing there */
		f = stdout;
	}
	return f;
}


void fatal(message,line_no)
char *message;
int line_no;
{
	fprintf(stderr,ErrHdr,
		(file_str[0] ? file_str[0] : "stdin"), line_no);
	fprintf(stderr, " Fatal: %s\n", message);
	exit(PCCTS_EXIT_FAILURE);
}

void error(message,line_no)
char *message;
int line_no;
{
	fprintf(stderr,ErrHdr,
		(file_str[0] ? file_str[0] : "stdin"), line_no);
	fprintf(stderr, " Error: %s\n", message);
	err_found = 1;
}

void warning(message,line_no)
char *message;
int line_no;
{
	fprintf(stderr,ErrHdr,
		(file_str[0] ? file_str[0] : "stdin"), line_no);
	fprintf(stderr, " Warning: %s\n", message);
}

char *
#ifdef __STDC__
OutMetaName(char *n)
#else
OutMetaName(n)
char *n;
#endif
{
	static char buf[200+1];

	if ( strcmp(outdir,TopDirectory)==0 ) return n;
	strcpy(buf, outdir);
	if ( strcmp(&buf[strlen(buf) - 1], DirectorySymbol ) )
		strcat(buf, DirectorySymbol);
	strcat(buf, n);
	return buf;
}
