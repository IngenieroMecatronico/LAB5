/*/*
 * PWM1.c
 *
 * Created: 15/04/2024 01:44:55
 *  Author: ferma
 */ 
#include "PWM1.h"

int PWM1(uint8_t numeropot, uint8_t valoradc, uint8_t overflow){
	uint16_t valortotal;
	if (numeropot == 1)
	{
		ADMUX = (ADMUX & 0xF0) | numeropot;
		ADCSRA |= (1<<ADSC);
		valortotal = (14.12*valoradc)+1198;//mapeo(valoradc, 0, 255, 19, 39); // Establece el ancho de pulso del servo
		OCR1AH = (valortotal & 0xFF00) >> 8;
		OCR1AL |= valortotal & 0x00FF;
	}
	else if (numeropot == 0)
	{
		ADMUX = (ADMUX & 0xF0) | numeropot;
		ADCSRA |= (1<<ADSC);
		OCR2A = (0.094*valoradc)+6.7;
		//while (ADCSRA & (1 << ADSC));
	}
	else if (numeropot == 4)
		{
			//valoradc = ADCH;
			ADMUX = (ADMUX & 0xF0) | numeropot;
			ADCSRA |= (1<<ADSC);
			if (overflow == valoradc)//valoradc)
			{
				PORTD = 0;
				overflow = 0;
				//_delay_ms(10);
			}
			else
			{
				PORTD = 0xF4;
			}
		}
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

int LED(uint8_t pot, uint8_t valoradc, uint8_t overflow){//uint8_t PRUEBA;
	if (pot == 2)
	{
		//valoradc = ADCH;
		ADMUX = (ADMUX & 0xF2);
		ADCSRA |= (1<<ADSC);
		if (overflow == valoradc)//valoradc)
		{
		   PORTD = 0;
		   overflow = 0;
		   //_delay_ms(10);
		}
		else 
		{
			PORTD = 0xF4;
		}
	}
	while (ADCSRA & (1 << ADSC));
	return ADC;
}
