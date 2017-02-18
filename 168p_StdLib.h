//-----------------------------------------------------------------------------------------------#
//																							     #
// Created by Joostens Tomek																	 # 			
// Library to control GPIO,ADC,TIMERS,INTERRUPTS,... with Atmega328P							 #
// Feel free to modify or use this software. 												     #
// You can send me a mail(joostenstomek@gmail.com) with your own adapted version of my software  #
// Version: V0.2																			     #
//-----------------------------------------------------------------------------------------------#


#include "defines.h" 						//includes defines 

/* Function to configure pins as input or output */
void pinMode(volatile uint8_t pin,volatile uint8_t state)
{
	volatile uint8_t newPin; 				//declare a new variable

	if((pin >=2 && pin <= 7)) 				//checking if parameter is between a certain range [2-7]
	{
		if(state == 1) 					    //if pin state is 1 then we set pin as an output
			DDRD|= (1 << pin); 			    //pin high
		else 						        //else we set pin as an input
			DDRD &= ~(1 << pin); 			//pin low
	}

	else if(pin >= 8 && pin <= 12) 			//checking if parameter is between a certain range [8-13]
	{
		newPin = pin - PORTCOUNT; 			//initialize variable
		
		if(state == 1)					    //if pin state is 1 then we set pin as an output
			DDRB |= (1 << newPin); 			//pin output
		else 						        //else we set pin as an input
			DDRB &= ~(1 << newPin); 		//pin input
	}
}

/* Function to configure pins as high or low */
void digitalWrite(volatile uint8_t pin,volatile uint8_t state)
{
	volatile uint8_t newPin;				//declare a new variable

	if((pin >=2 && pin <= 7))				//checking if parameter is between a certain range [2-7]
	{
		if(state == 1)						//if pin state is 1 then we set pin as a high pin
			PORTD|= (1 << pin); 			//pin high
		else								//else we set pin as a low pin
			PORTD &= ~(1 << pin); 			//pin low
	}

	else if(pin >= 8 && pin <= 12) 			//checking if parameter is between a certain range [8-13]
	{
		newPin = pin - PORTCOUNT; 			//initialize variable

		if(state == 1) 						//if pin state is 1 then we set pin as a high pin
			PORTB |= (1 << newPin); 		//pin high
		else 								//else we set pin as a low pin
			PORTB &= ~(1 << newPin); 		//pin low
	}
}

/* Function to readdigital pins as high or low */
int digitalRead(volatile uint8_t pin)
{
	volatile uint8_t newPin;				//declare a new variable

	if((pin >=2 && pin <= 7))				//checking if parameter is between a certain range [2-7]
	{
		return PIND & (1 << pin);			//checks if PIND0 is pressed
	}

	else if(pin >= 8 && pin <= 12) 			//checking if parameter is between a certain range [8-13]
	{
		newPin = pin - PORTCOUNT; 			//initialize variable
		return PINB & (1 << newPin);		//checks if PIND0 is pressed
	}		
}

