
#include "clock.h"

/***********************ʱ�ӳ�ʼ��*************************/
void clock_init()
{ 
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL  = CALDCO_1MHZ;                     //DCO = 1MHz
}
/************************************************************/