#include "msp430g2553.h"
#include "uart.h"

//Pin Assignements
#define LED BIT0	//LED to signal data transmission via UART
#define RXD BIT1	//RX for UART
#define TXD BIT2	//TX for UART

//
#define SMCLKRATE 8000000
#define BAUDRATE 9600
#define BRDIV16 ((16*SMCLKRATE)/BAUDRATE)
#define BRDIV (BRDIV16/16)
#define BRMOD ((BRDIV16-(16*BRDIV)+1)/16)
#define BRDIVHI (BRDIV/256)
#define BRDIVLO (BRDIV-BRDIVHI*256)


volatile unsigned int tx_flag;			//Mailbox Flag for the tx_char.
volatile unsigned char tx_char = 'd';	//This char is the most current char to go into the UART

volatile unsigned int rx_flag;			//Mailbox Flag for the rx_char.
volatile unsigned char rx_char = 'd';	//This char is the most current char to come out of the UART

//uart_init
//Sets up the UART interface via USCI
void uart_init(void)
{
	P1SEL = RXD + TXD;					//Setup the I/O
	P1SEL2 = RXD + TXD;

    P1DIR |= LED; 						//P1.0 red LED. Toggle when char received.
    P1OUT |= LED; 						//LED off

	UCA0CTL1 |= UCSSEL_2;

	UCA0BR0 = BRDIVLO;
	UCA0BR1 = BRDIVHI;
	UCA0MCTL = 2*BRMOD;
	UCA0CTL1 &= ~UCSWRST; 				//USCI state machine
	IE2 |= UCA0RXIE; 					//Enable USCI_A0 RX interrupt

	rx_flag = 0;						//Set rx_flag to 0
	tx_flag = 0;						//Set tx_flag to 0

	return;
}

//uart_getc
//Get a char from the UART. Waits till it gets one
unsigned char uart_getc()				//Waits for a valid char from the UART
{
	while (rx_flag == 0);		 		//Wait for rx_flag to be set
	rx_flag = 0;						//ACK rx_flag
    return rx_char;
}

//uart_gets
//Get a string of known length from the UART. Strings terminate when enter is pressed or string buffer fills
//Will return when all the chars are received or a carriage return (\r) is received. Waits for the data.
void uart_gets(char* Array, int length)
{
	unsigned int i = 0;

	while((i < length))					//Grab data till the array fills
	{
		Array[i] = uart_getc();
		if(Array[i] == '\r')				//If we receive a \r the master wants to end
		{
			for( ; i < length ; i++)		//fill the rest of the string with \0 nul. Overwrites the \r with \0
			{
				Array[i] = '\0';
			}
			break;
		}
		i++;
	}

    return;
}

//uart_putc
//Sends a char to the UART. Will wait if the UART is busy
void uart_putc(unsigned char c)
{
	tx_char = c;						//Put the char into the tx_char
	IE2 |= UCA0TXIE; 					//Enable USCI_A0 TX interrupt
	while(tx_flag == 1);				//Have to wait for the TX buffer
	tx_flag = 1;						//Reset the tx_flag
	return;
}

//uart_puts
//Sends a string to the UART. Will wait if the UART is busy
void uart_puts(char *str)				//Sends a String to the UART.
{
     while(*str) uart_putc(*str++);		//Advance though string till end
     return;
}


#pragma vector = USCIAB0TX_VECTOR		//UART TX USCI Interrupt
__interrupt void USCI0TX_ISR(void)
{
	UCA0TXBUF = tx_char;				//Copy char to the TX Buffer
	tx_flag = 0;						//ACK the tx_flag
	IE2 &= ~UCA0TXIE; 					//Turn off the interrupt to save CPU
}

#pragma vector = USCIAB0RX_VECTOR		//UART RX USCI Interrupt. This triggers when the USCI receives a char.
__interrupt void USCI0RX_ISR(void)
{
	rx_char = UCA0RXBUF;				//Copy from RX buffer, in doing so we ACK the interrupt as well
	rx_flag = 1;						//Set the rx_flag to 1
	P1OUT ^= LED;						//Notify that we received a char by toggling LED
}
