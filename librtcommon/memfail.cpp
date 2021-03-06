/***************************************************************************
Copyright (c) 2002,2003,2004 Donald Wayne Carr 

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rtcommon.h"

/*********************************************************************/

void malloc_failure(const char *file, int line)
{
  /*  Routine for malloc failure. */
  /***  Here is the plain printf version *****/
  printf("Malloc failed: %s line %d\n", file, line);
  printf("Program aborting\n");
  char buf[10];
  fgets(buf, sizeof(buf), stdin);
  /********/
  exit(0);
}

/********************************************************************/

void common_new_handler(void)
{
  /* The common new new handler. */

  /*******/
  printf("Call to new failed\n");
  printf("Program aborting\n");
  char buf[10];
  fgets(buf, sizeof(buf), stdin);
  /*****/
  exit(0);
}

