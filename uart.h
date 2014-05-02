
//This flag is to be used by other modules to check and see if a new transmission has happened..
extern volatile unsigned int rx_flag;

//uart_init
//Sets up the UART interface via USCI
void uart_init(void);

//uart_getc
//Get a char from the UART. Waits till it gets one
unsigned char uart_getc();

//uart_gets
//Get a string of known length from the UART. Strings terminate when enter is pressed or string buffer fills
//Will return when all the chars are received or a carriage return (\r) is received. Waits for the data.
void uart_gets();

//uart_putc
//Sends a char to the UART. Will wait if the UART is busy
void uart_putc(unsigned char c);

//uart_puts
// Sends a string to the UART. Will wait if the UART is busy
void uart_puts(char *str);
