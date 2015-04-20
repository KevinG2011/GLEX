//
//  errors.h
//  GLEX
//
//  Created by Li Jia on 15/4/20.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#ifndef GLEX_errors_h
#define GLEX_errors_h
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEBUG
	#define DPRINTF(arg) printf arg
#else
	#define DPRINTF(arg)
#endif

#define err_abort(code,text) do { \
	fprintf(stderr,"%s at \"%s\":%d: %s\n", \
			text,__FILE__,__LINE__,strerror(code)); \
	abort(); \
	} while (0)

#define errno_abort(text) do { \
	fprintf(stderr,"%s at \"%s\":%d: %s\n", \
			text,__FILE__,__LINE__,strerror(errno)); \
	abort(); \
	} while (0)
#endif
