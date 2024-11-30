
#include "comm.h"

short fastAtan2(long y, long x)
{
    long ax;
    long ay;

    if (x == 0)
    {
        x = 1;
    }

    if (y == 0)
    {
        return (x < 1) * 2048;
    }

    ax = labs(x);
    ay = labs(y);

    if (x > 0)
    {
        if (y > 0)
        {
            if (ax < ay)
            {
                return (1024 - ((ax * 512) / ay));
            }
            else
            {
                return ((ay * 512) / ax);
            }
        }
        else
        {
            if (ay < ax)
            {
                return (4096 - ((ay * 512) / ax));
            }
            else
            {
                return (((ax * 512) / ay) + 3072);
            }
        }
    }

    if (y > 0)
    {
        if (ax < ay)
        {
            return (((ax * 512) / ay) + 1024);
        }
        else
        {
            return (2048 - ((ay * 512) / ax));
        }
    }

    if (ay < ax)
    {
        return (((ay * 512) / ax) + 2048);
    }
    else
    {
        return (3072 - ((ax * 512) / ay));
    }
}
long labs(long val) {
    return val >= 0 ? val : 0 - val;
}