#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <stdio.h>

#define trace()\
    printf("[TRACE] [%s : %d] \n", __PRETTY_FUNCTION__, __LINE__);

#ifdef DEBUG
#define debug(fmt, msg...)\
    printf("[TRACE] [%s : %d]:::"fmt"\n", __PRETTY_FUNCTION__, __LINE__, ##msg);
#else
#define debug(fmt, msg...)
#endif

#define error(fmt, msg...)\
    printf("[ERROR] [%s : %d]:::"fmt"\n", __PRETTY_FUNCTION__, __LINE__, ##msg);

#define info(fmt, msg...)\
    printf("[INFO] [%s : %d]:::"fmt"\n", __PRETTY_FUNCTION__, __LINE__, ##msg);

#define report(fmt, msg...)\
    printf(fmt"\n", ##msg);

#endif
