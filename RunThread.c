#include "KMotionDef.h"

void main() 
{
	printf("Begining Blink");
	
	while(1)
	{
		ClearBit(46);
		Delay_sec(0.1);
		SetBit(46);
		Delay_sec(0.1);
	}
}