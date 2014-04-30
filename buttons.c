asm(" .length 10000");
asm(" .width 132");

#include "msp430g2553.h"

#define db_time 1500
#define BUTTON_BIT 0xF8         //Initialize all pins 3,4,5,6,7
#define UP_BUTTON 0x08          //Represent 0000 1000 <Pin P1.3>
#define LEFT_BUTTON 0x10        //Represent 0001 0000 <Pin P1.4>
#define RIGHT_BUTTON 0x20       //Represent 0010 0000 <Pin P1.5>
#define DOWN_BUTTON 0x40        //Represent 0100 0000 <Pin P1.6>
#define RESET_BUTTON 0x80		//Represent 1000 0000 <Pin P1.7>

volatile int db_counter = 0;
volatile int  up, left, right, down, reset; //These are the button toggles (1 = output, 0 = nothing)
volatile int ulrd [5] = {0, 0, 0, 0, 0};

//For Test and Debug Only
volatile int up_c = 0;
volatile int left_c = 0;
volatile int right_c = 0;
volatile int down_c = 0;
volatile int reset_c = 0;

void uplrToDpad(int Dpad[])
{
	int k;
	//for(k = 0; k < 4; k++)
	for(k = 0; k < 5; k++)
	{
		Dpad[k] = ulrd[k];
	}
	memset(ulrd,0,sizeof(ulrd));
}

//Initialize Watch dog timer
void init_wdt(){
	WDTCTL =(WDTPW +             // (bits 15-8) password
	                                // bit 7=0 => watchdog timer on
	                                // bit 6=0 => NMI on rising edge (not used here)
	                                // bit 5=0 => RST/NMI pin does a reset (not used here)
	               WDTTMSEL +        // (bit 4) select interval timer mode
	               WDTCNTCL +        // (bit 3) clear watchdog timer counter
	                        0        // bit 2=0 => SMCLK is the source
	                        +1        // bits 1-0 = 01 => source/8K
	                );
	    IE1 |= WDTIE;                // enable the WDT interrupt (in the system interrupt register IE1)

}

void init_button(void)
{
// All GPIO's are already inputs if we are coming in after a reset
        P1OUT |= BUTTON_BIT; // pullup
        P1REN |= BUTTON_BIT; // enable resistor
        P1IES |= BUTTON_BIT; // set for 1->0 transition
        P1IFG &= ~BUTTON_BIT;// clear interrupt flag
        P1IE  |= BUTTON_BIT; // enable interrupt
}

void interrupt button_handler()
{
// check that this is the correct interrupt
// (if not, it is an error, but there is no error handler)
        //If the Button is pushed, toggle it as 1. If released/not being pushed, it is 0
        if (db_counter == 0){
        if (P1IFG & UP_BUTTON){
                if (up == 1){
                        up = 0;
                }
                else{
                        up = 1;
                }
                P1IFG &= ~UP_BUTTON; // reset the interrupt flag
                P1IES ^= UP_BUTTON; //Activate this interrupt
        }

        if (P1IFG & LEFT_BUTTON){
                if (left == 1){
                        left = 0;
                }
                else{
                        left = 1;
                }
                P1IFG &= ~LEFT_BUTTON; // reset the interrupt flag
                P1IES ^= LEFT_BUTTON; //Activate this interrupt
        }

        if (P1IFG & RIGHT_BUTTON){
                if (right == 1){
                        right = 0;
                }
                else{
                        right = 1;
                }
                P1IFG &= ~RIGHT_BUTTON; // reset the interrupt flag
                P1IES ^= RIGHT_BUTTON; //Activate this interrupt
        }

        if (P1IFG & DOWN_BUTTON){
                if (down == 1){
                        down = 0;
                }
                else{
                        down = 1;
                }
                P1IFG &= ~DOWN_BUTTON; // reset the interrupt flag
                P1IES ^= DOWN_BUTTON; //Activate this interrupt
        }
        if (P1IFG & RESET_BUTTON){
                if (reset == 1){
                        reset = 0;
                }
                else{
                        reset = 1;
                }
                P1IFG &= ~RESET_BUTTON; // reset the interrupt flag
                P1IES ^= RESET_BUTTON; //Activate this interrupt
        }


//Depending on Button's pushed, make some noises
/*Button controls--------------------------------------------------------------------*/
        //
        if (up == 1 && left == 0 && right == 0 && down == 0 && reset == 0){
                up_c++;
                ulrd[0] = 1;
                db_counter = db_time;
        }
        else if (up == 0 && left == 1 && right == 0 && down == 0 && reset == 0){
                left_c++;
                ulrd[1] = 1;
                db_counter = db_time;
        }
        else if (up == 0 && left == 0 && right == 1 && down == 0 && reset == 0){
                right_c++;
                ulrd[2] = 1;
                db_counter = db_time;
        }
        else if (up == 0 && left == 0 && right == 0 && down == 1 && reset == 0){
                down_c++;
                ulrd[3] = 1;
                db_counter = db_time;
        }
        else if (up == 0 && left == 0 && right == 0 && down == 0 && reset == 1){
                down_c++;
                ulrd[4] = 1;
                db_counter = db_time;
        }
        else{
                db_counter = 0;
        }
        }
        else{
        	db_counter--;
        }
}
ISR_VECTOR(button_handler,".int02") 	// declare interrupt vector

void interrupt WDT_interval_handler(){
	if(db_counter>0){
		db_counter--;
	}
}
ISR_VECTOR(WDT_interval_handler, ".int10")

