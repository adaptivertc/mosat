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


/*********************************************************************

rtcommon.h

Header file for procedures in common.lib.

*********************************************************************/

#ifndef __RTCOMMON_INC__
#define __RTCOMMON_INC__

//#include <io.h>

#define FAILED (-1)
#define SUCCESS (0)

bool is_a_space(char c);
void rtrim(char *str);
void ltrim(char *str);
void strip_quotes(char *str);
char *find_str(char *place_to_look, char *string_to_find);
void space_pad(char *str, int n);

void malloc_failure(char *file, int line);
void common_new_handler(void);
void strip_trailing_zeros(char *str);
void delete_cr(char *str);
char *temp_name(char *base, char *ext);
char *get_bgi_dir(void);
char *safe_strcpy(char *dest, const char *src, int size_dest);
char *safe_strcat(char *dest, const char *src, int size_dest);

//#define file_exists(fn) (0 == access(fn, 0))
//bool file_exists(char *filename);


int first_file(char *path, char *file_name);
int first_file(char *path, char *file_name, int attr);
int next_file(char *file_name);

#define MALLOC_CHECK(p) if((p) == NULL) malloc_failure(__FILE__, __LINE__)

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned long uint32;
typedef long int32;

//#define goto(x,y) move(y,x)

#define dir_exists(f) (0 == access(f,F_OK))
#define file_exists(f) (0 == access(f,F_OK))

#endif

