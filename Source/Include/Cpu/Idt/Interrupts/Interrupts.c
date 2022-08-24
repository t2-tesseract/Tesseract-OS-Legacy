#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void CDefaultInt(void){
    
}

void ClockInt(void)
{
        static int Tic = 0;
        static int Second = 0;
        Tic++;
        if (Tic % 100 == 0) {
                Second++;
                Tic = 0;
        }
}

