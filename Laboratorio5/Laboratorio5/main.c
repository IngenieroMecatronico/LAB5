/*
 * Lab 5 Progra de Microcontroladores
 * Created: 7/04/2024 18:52:36
 * Author : Juan Fernando Maldonado Menéndez
 * Descripción: Movimiento de servomotores PWM con potenciometro ADC.
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM1/PWM1.h"


volatile	uint8_t valoradc = 0; //Variable donde guardare el valor ADC.
volatile	uint8_t overflow = 0; //Variable donde guardo mi contador para el DUTYCYCLE del led.

//***************************************************************************************************************************************************************
// Setup:
//***************************************************************************************************************************************************************

void TIMER0(){
	UCSR0B = 0;
	//Configuro que mi timer este en modo hasta 0xFF.
	TCCR0A = 0;//|= (1<<WGM01);
	//Con un prescaler de 1024 para tener 16.32 ms de periodo.
	TCCR0B |= (1<<CS01);//(1<<CS02)|(1<<CS00);
	//Habilito que existan interrupciones
	TIMSK0 |= (1<<TOIE0);
	//Establezco que mi puerto D son salidas.
	DDRD = 0xFF;
	PORTD = 0;
	
}
void setupPWM1(void) {
	cli();
	// Configura los pines de los botones como entrada con pull-up
	UCSR0B = 0;
	//Configurando el pin 1 del puerto B como salida. (OC1A)
	DDRB |= (1<<DDB1);
	TCCR1A = 0;
	TCCR1B = 0;
	//Configuramos el modo y el prescaler.
	TCCR1A |= (1<<COM1A1);//|(1<<COM1A0); //Configuramos en modo rápido no invertido.
	// Configuramos el modo Fast para que llegue al OCR1A
	TCCR1A |= (1<<WGM11);//|(1<<WGM10);
	TCCR1B |= (1<<WGM13)|(1<<WGM12);
	//Configuro mi prescaler a 8, debido a que mi periodo máximo es de 32.77 ms y necesito solo 20 ms para mover el servo.
	TCCR1B |= (1<<CS11);
	//Escribo 39999 a ICR1 dividido entre H y L
	ICR1H = 0x9C;
	ICR1L = 0x3F;
	sei();
}

void setupPWM2(void) {
	cli();
	// Configura los pines de los botones como entrada con pull-up
	UCSR0B = 0;
	//Configurando el pin 3 del puerto B como salida. (OC1A)
	DDRB |= (1<<DDB3);
	TCCR2A = 0;
	TCCR2B = 0;
	//Configuramos el modo y el prescaler.
	TCCR2A |= (1<<COM2A1);//|(1<<COM1A0); //Configuramos en modo rápido no invertido.
	// Configuramos el modo Fast para que llegue al OxFF
	TCCR2A |= (1<<WGM21)|(1<<WGM20);
	//Configuro mi prescaler a 1024, debido a que mi periodo máximo es de 18 ms y necesito solo 20 ms para mover el servo.
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);
	sei();
}
	
void InitADC1(void){
	ADMUX |= (1 << REFS0);      //Estoy configurando a que funcione con AVCC.
	ADMUX &= ~(1 << REFS1);
	//Justificacion a la izquierda.
	ADMUX |= (1 << ADLAR); 
	//Apagamos la salida digital de PINC0
	DIDR0 |= (1 << ADC0D);
	//Configuración del canal 0 ya que allí esta mi potenciómetro.
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX0); 
	ADCSRA = 0;
	//Habilitamos la bandera del ADC
	ADCSRA |= (1 << ADIE);
	//Habilitamos el prescaler a 128.
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
	//Habilitamos el ADC
	ADCSRA |= (1 << ADEN);
	
	
}

void InitADC2(void){
	ADMUX |= (1 << REFS0);      //Estoy configurando a que funcione con AVCC.
	ADMUX &= ~(1 << REFS1);
	//Justificacion a la izquierda.
	ADMUX |= (1 << ADLAR);
	//Apagamos la salida digital de PINC1
	DIDR0 |= (1 << ADC1D);
	//Configuración del canal 0 ya que allí esta mi potenciómetro.
	ADMUX &= ~(1 << MUX1);
	ADMUX |= (1 << MUX0);
	ADCSRA = 0;
	//Habilitamos la bandera del ADC
	ADCSRA |= (1 << ADIE);
	//Habilitamos el prescaler a 128.
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
	//Habilitamos el ADC
	ADCSRA |= (1 << ADEN);
	
	
}
void InitADC3(void){
	ADMUX |= (1 << REFS0);      //Estoy configurando a que funcione con AVCC.
	ADMUX &= ~(1 << REFS1);
	//Justificacion a la izquierda.
	ADMUX |= (1 << ADLAR);
	//Apagamos la salida digital de PINC1
	DIDR0 |= (1 << ADC2D);
	//Configuración del canal 2 ya que allí esta mi potenciómetro.
	//ADMUX &= ~(1 << MUX0);
	ADMUX |= (1 << MUX1);
	ADCSRA = 0;
	//Habilitamos la bandera del ADC
	ADCSRA |= (1 << ADIE);
	//Habilitamos el prescaler a 128.
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
	//Habilitamos el ADC
	ADCSRA |= (1 << ADEN);
	
	
}
//***************************************************************************************************************************************************************
// Loop principal:
//***************************************************************************************************************************************************************
int main(void) {
	setupPWM1(); // Inicializa todo.
	setupPWM2();
	//Configuro mi LED.
	TIMER0();
	//Configuro mi ADC.
	InitADC1();
	InitADC2();
	InitADC3();
	DDRB = 0xFF;
	ADCSRA |= (1<<ADSC);   //Habilito la interrupcion de ADC.
	while (1) {
		//PORTD = 0;
		PWM1(4, valoradc, overflow);
		_delay_ms(10);
		PWM1(1, valoradc, overflow);
		_delay_ms(10);
		PWM1(0, valoradc, overflow);
		_delay_ms(10);
	}
	return 0;
}


//***************************************************************************************************************************************************************
// Interrupciones:
//***************************************************************************************************************************************************************

ISR(ADC_vect){
	valoradc = ADCH;
	ADCSRA |= (1<<ADIF); // Apaga la bandera de interrupción del ADC
}

ISR(TIMER0_OVF_vect){
	TIFR0 |= (1<<TOV0);  // Indico que hubo overflow.
}
