#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{		
	Dot_matrix_main();										//������ �̸�
	
	
	while(1)
	{
		
	}
	return ;
}


