#include "../common/sysutil.hpp"

#include <sys/time.h>

timeval getTime() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}
