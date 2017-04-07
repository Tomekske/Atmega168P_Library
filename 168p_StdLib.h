//-----------------------------------------------------------------------------------------------#
// Created by Joostens Tomek																	 # 			
// Library to control GPIO,ADC,TIMERS,INTERRUPTS,... with Atmega168P							 #
// Feel free to modify or use this software. 												     #
// You can send me a mail(joostenstomek@gmail.com) with your own adapted version of my software  #
// Version: V0.2																			     #
//-----------------------------------------------------------------------------------------------#

#include "defines.h" 						//includes defines 



//--------------------------------------------------------------------------------------------------------------//
// 											Pin configurations													//
//--------------------------------------------------------------------------------------------------------------//

/**
  * @brief  Function to configure pins as input or output
  * @param  pin: select pin which you want to configure
  * @param  state: select if pin is input or output
  * @retval None
  */
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

/**
  * @brief  Function to configure pins as high or low
  * @param  pin: select pin which you want to configure
  * @param  state: select if pin is low or high
  * @retval None
  */
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

/**
  * @brief  Function to read digital pin
  * @param  pin: select pin which you want to read
  * @retval 1 or 0
  */
int digitalRead(volatile uint8_t pin)
{
	volatile uint8_t newPin;				//declare a new variable

	if((pin >=2 && pin <= 7))				//checking if parameter is between a certain range [2-7]
	{
		return PIND & (1 << pin);			//checks if PIND0 is pressed
	}

	else if(pin >= 8 && pin <= 13) 			//checking if parameter is between a certain range [8-13]
	{
		newPin = pin - PORTCOUNT; 			//initialize variable
		return PINB & (1 << newPin);		//checks if PIND0 is pressed
	}		
}



//--------------------------------------------------------------------------------------------------------------//
// 											  Shiftregister				           								//
//--------------------------------------------------------------------------------------------------------------//

/**
  * @brief  Function to calculate the absolute pin postion 
  * @param  shiftregisterNumber: select which shiftregister to choose
  * @param  position: select pin which you want to read
  * @retval Absolute position
  */
int calculateShiftPosition(int shiftregisterNumber,int position)
{
	int absolutePosition = 0;
	return absolutePosition = ((shiftregisterNumber-1) * 8) + position;
}

/**
  * @brief  Function to shift pins from shiftregister
  * @param  shift: select which master shiftregister you want to use
  * @retval None
  */
void shiftOut(ShiftRegister shift)
{
	digitalWrite(shift.STCP_pin, LOW);
	for (int i = AMOUNT_OF_SHIFTREGISTERS * PORTCOUNT; i>=0; i--)
	{
		digitalWrite(shift.SHCP_pin, LOW);
		digitalWrite(shift.DS_pin, shift.reg[i]);
		digitalWrite(shift.SHCP_pin, HIGH);
	}
	digitalWrite(shift.STCP_pin, HIGH);
}

/**
  * @brief  Function to write date to a shiftregister. Counting starts from 0 to 7
  * @param  shift: select the master shiftregister you want to use
  * @param  shiftregisterNumber: select which shiftregister you want to use
  * @param  pin: select relative pin which you want to configure
  * @param  state: select if pin is low or high
  * @retval None
  */
 void registerWrite(ShiftRegister *shift,int shiftregisterNumber,int pin,int state)
{
	int absolutePosition = calculateShiftPosition(shiftregisterNumber,pin);
	shift->reg[absolutePosition] = state; // choose which pin you want to set high or low
	shiftOut(*shift); // shifting out data
}

/**
  * @brief  Function to set shiftregister outputs low
  * @param  shift: select the master shiftregister you want to use
  * @param  shiftregisterNumber: select which shiftregister you want to use
  * @retval None
  */
 void initShiftregister(ShiftRegister *shift,int shiftregisterNumber)
 {
	 for(int i = 0; i < 8;i++)
		registerWrite(shift,shiftregisterNumber,i,LOW);
 }



//--------------------------------------------------------------------------------------------------------------//
// 											   UART				           				        				//
//--------------------------------------------------------------------------------------------------------------//

/**
  * @brief  Function to setup UART
  * @param  baudrate: select communication speed
  * @retval None
  */
 void serialStart(int baudRate)
{
 	UBRR0H =  (unsigned char)(BRC(baudRate) >> 8);   // The UBRR0H contains the four most significant bits
 	UBRR0L =  (unsigned char)BRC(baudRate);          // The UBRR0H contains the four least significant bits

 	UCSR0B = (1<<RXEN0)|(1<<TXEN0);              // Enable receiver and transmiter
 	UCSR0C = (3<<UCSZ00) ;                       // Using 1 stopbit and 8 bit Character Size
}

/**
  * @brief  Function to send one byte over UART
  * @param  data: specify which byte you want to send
  * @retval None
  */
void sendChar(char data)
{
	while(!(UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer */
	UDR0 = data; /* Get and return received data from buffer */
}

/**
  * @brief  Function to send a whole string over UART
  * @param  data: specify which string you want to send
  * @retval None
  */
void serialWrite(char* sendString)
{
	while(*sendString != NULL)
	{
		sendChar(*sendString);
		sendString++;
	}
}

/**
  * @brief  Function to read incomming characters send over UART
  * @param  None
  * @retval received character
  */
char serialRead()
{
	 while ( !(UCSR0A & (1<<RXC0))); //Wait for data to be received
	 return UDR0; //Get and return received data from buffer 
}

/**
  * @brief  Function to read a whole incomming strings send over UART
  * @param  str: String you want to copy received data to
  * @retval None
  */
 void serialReadString(char *str)
 {
	char data;

	do
	{
		data = serialRead();
		*str = data;
		str++;
	}while(data != 13); //CR = enter
 }