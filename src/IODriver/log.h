#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <typeinfo>


//#define LOG_ENABLE

#ifdef LOG_ENABLE
        #define LOG(M, ...)\
                fprintf(stderr, "[INFO] (%s: %s) " M "\n",\
                typeid(*this).name(),\
                __FUNCTION__, ##__VA_ARGS__);
#else
        #define LOG(M, ...)
#endif


#endif // LOG_H
