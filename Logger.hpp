#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <stdio.h>

#define trace()\
do      \
{       \
    printf("[TRACE] [%s : %d] \n", __PRETTY_FUNCTION__, __LINE__);\
}while(0);

#ifdef DEBUG
#define debug(fmt, msg...)\
do      \
{       \
    printf("[TRACE] [%s : %d]:::"fmt"\n", __PRETTY_FUNCTION__, __LINE__, ##msg);\
}while(0);

#else
#define debug(fmt, msg...) (void(0))
#endif

#define error(fmt, msg...)\
do      \
{       \
    printf("[ERROR] [%s : %d]:::"fmt"\n", __PRETTY_FUNCTION__, __LINE__, ##msg);\
}while(0);

#define info(fmt, msg...)\
do      \
{       \
    printf("[INFO] [%s : %d]:::"fmt"\n", __PRETTY_FUNCTION__, __LINE__, ##msg);\
}while(0);

#define report(fmt, msg...)\
do      \
{       \
    printf(fmt"\n", ##msg);\
}while(0);

#endif
