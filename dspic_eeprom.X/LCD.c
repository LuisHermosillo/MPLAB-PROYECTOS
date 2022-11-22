#include "LCD.h"
#include <p30f4013.h>

/*******************************************************************
Function Name:          IniciaLCD
Description:            This function configure the IO pins  
                        and initializes the LCD controller
                        the lcdtype should contain data on the type 
                        of interface and number of lines of display.
Input parameters:       char lcdtype 
Return value:           none
********************************************************************/
void iniciaLCD(char lcdType)
{
	/* Allow a delay for POR.(minimum of 15ms) */
	DelayPORLCD();	
	
	/*Initialize the data port/control pins to zero */
	DATA_PIN_7 = 0;
	DATA_PIN_6 = 0;
	DATA_PIN_5 = 0;
	DATA_PIN_4 = 0;
	
	# ifdef EIGHT_BIT_INTERFACE
		DATA_PIN_3 = 0;
		DATA_PIN_2 = 0;
		DATA_PIN_1 = 0;
		DATA_PIN_0 = 0;
	#endif
	
	RW_PIN = 0;
	RS_PIN = 0;
	E_PIN = 0;
	
	/* Configure the data pins as outputs */
	TRIS_DATA_PIN_7 = 0;
	TRIS_DATA_PIN_6 = 0;
	TRIS_DATA_PIN_5 = 0;
	TRIS_DATA_PIN_4 = 0;
	
	#ifdef EIGHT_BIT_INTERFACE
		TRIS_DATA_PIN_3 = 0;
		TRIS_DATA_PIN_2 = 0;
		TRIS_DATA_PIN_1 = 0;
		TRIS_DATA_PIN_0 = 0;
	#endif

	/* Make all control pins as outputs */
	TRIS_RW = 0;
	TRIS_RS = 0;
	TRIS_E = 0;
 
	writeCmdLCD(0x30);
	DelayPORLCD();
	writeCmdLCD(0x30);
	Delay200usLCD();
	writeCmdLCD(0x30);
	Delay200usLCD();
	writeCmdLCD(lcdType&0xF0);


	/* First Step  - Function Set as specified by lcdType*/
	writeCmdLCD(lcdType); 
	
	DelayLCD();

	/* Second Step - Display ON/OFF CONTROL*/
	writeCmdLCD(DON & BLINK_OFF & CURSOR_OFF);

	DelayLCD();
	
	/* Third Step - LCD Clear	*/	
	writeCmdLCD(CLEAR_LCD);

	DelayLCD();
	
	/*Fourth Step - Entry Mode Set	*/
	writeCmdLCD(ENTIRE_SHIFT_OFF & INCR_MODE);

   } /* end of function */


/*******************************************************************
Function Name :         setDDRamAddr
Description   :         This function sets the display data address 
                        of the LCD controller.
                        The user must first check if the controller 
                        is busy or not by calling the busyLCD() 
                        function.
Input parameters:       char address
Return value:           void
********************************************************************/

void setDDRamAddr( char address)
{
    RW_PIN = 0;   /* enable write */
    RS_PIN = 0;   /* select Instruction Reg */

    /* configure the common data pins as output */
    TRIS_DATA_PIN_7 = 0;
    TRIS_DATA_PIN_6 = 0;
    TRIS_DATA_PIN_5 = 0;
    TRIS_DATA_PIN_4 = 0;

    /*DB7 must be 1 for setting DDRam ADDR */
    DATA_PIN_7 = 1;

    /* address is only of 7 bits in length */
    DATA_PIN_6 = (unsigned int) ((address & 0x40)>>6); 
    DATA_PIN_5 = (unsigned int) ((address & 0x20)>>5);
    DATA_PIN_4 = (unsigned int) ((address & 0x10)>>4);

# ifdef EIGHT_BIT_INTERFACE
    /* configure the other pins as output */
    TRIS_DATA_PIN_3 = 0;
    TRIS_DATA_PIN_2 = 0;
    TRIS_DATA_PIN_1 = 0;
    TRIS_DATA_PIN_0 = 0;

    DATA_PIN_3 = (unsigned int)(( address & 0x08)>>3);
    DATA_PIN_2 = (unsigned int)(( address & 0x04)>>2);
    DATA_PIN_1 = (unsigned int)((address & 0x02)>>1);
    DATA_PIN_0 = (unsigned int)(( address & 0x01)>>0);

    E_PIN = 1;    /* enable read/write */
    DelayEPulseWidth();
    E_PIN = 0;
# else
    E_PIN = 1;
    DelayEPulseWidth();
    E_PIN = 0;
    RW_PIN =0;    /* enable write */
    RS_PIN =0;    /* select Instruction Reg */

    DATA_PIN_7 = (unsigned int)((address & 0x08)>>3);
    DATA_PIN_6 = (unsigned int)((address& 0x04)>>2);
    DATA_PIN_5 = (unsigned int)((address & 0x02)>>1);
    DATA_PIN_4 = (unsigned int)((address & 0x01)>>0);

    E_PIN = 1;    /* enable read/write */
    DelayEPulseWidth();
    E_PIN =0;
#endif

    /* configure all pins to input mode */
    TRIS_DATA_PIN_7 = 1;
    TRIS_DATA_PIN_6 = 1;
    TRIS_DATA_PIN_5 = 1;
    TRIS_DATA_PIN_4 = 1;
# ifdef EIGHT_BIT_INTERFACE
    TRIS_DATA_PIN_3 = 1;
    TRIS_DATA_PIN_2 = 1;
    TRIS_DATA_PIN_1 = 1;
    TRIS_DATA_PIN_0 = 1;
#endif
	Delay200usLCD();
}  /*  end of function */


/*********************************************************************
Function Name:             BusyLCD
Description:               This function returns the busy status
                           of the LCD Controller.The Busy flag is
                           is MS bit when the Instruction Reg is read
Input parameters:          none
Return value:              char- returns 1 if Busy flag is set, else 0
*********************************************************************/

char busyLCD(void)
{
     /* Configure all pins as input */
     TRIS_DATA_PIN_7 = 1;
     TRIS_DATA_PIN_6 = 1;
     TRIS_DATA_PIN_5 = 1;
     TRIS_DATA_PIN_4 = 1;

# ifdef EIGHT_BIT_INTERFACE
     TRIS_DATA_PIN_3 = 1;
     TRIS_DATA_PIN_2 = 1;
     TRIS_DATA_PIN_1 = 1;
     TRIS_DATA_PIN_0 = 1;
#endif

     RW_PIN = 1;           /* for read operation */
     RS_PIN = 0;           /* for selecting AC for read */
     DelayEPulseWidth();

# ifdef EIGHT_BIT_INTERFACE
     E_PIN = 1;            /* for enabling read/write */
   DelayEPulseWidth();
    

     if(READ_PIN_7)        /* busy flag */
     {
         E_PIN = 0;
         Delay600nsLCD();
          return 1;
     }
     else
     {
          E_PIN = 0;
         Delay600nsLCD();
          return 0;
     }
#endif

 /* FOUR_BIT_INTERFACE   */ 

     E_PIN = 1;            /* for enabling read/write */
     DelayEPulseWidth();

     if(READ_PIN_7)        /* busy flag */
     {
          E_PIN = 0;       /* disable read/write */
		  Delay600nsLCD();
          return 1;
     }
     else
     {
          E_PIN = 0;       /* disable read/write */
		  Delay600nsLCD();
          return 0;
     }

} /* end of function */




/*******************************************************************
Function Name   :   ReadAddrLCD
Description     :   This function reads a byte which is a 7 bit 
                    address and the BF forms the eight bit .
                    The user must first check the busy status 
                    of the LCD cntrlr by calling the BusyXLCD()
Input parameters:   none
Return value    :   char address
********************************************************************/

char readAddrLCD(void)
{
    char address;
    char temp[7];
	int i;

    /* Configure all pins as input */
    TRIS_DATA_PIN_7 = 1;
    TRIS_DATA_PIN_6 = 1;
    TRIS_DATA_PIN_5 = 1;
    TRIS_DATA_PIN_4 = 1;
# ifdef EIGHT_BIT_INTERFACE
    TRIS_DATA_PIN_3 = 1;
    TRIS_DATA_PIN_2 = 1;
    TRIS_DATA_PIN_1 = 1;
    TRIS_DATA_PIN_0 = 1;
#endif
    
    RW_PIN = 1;                 /* for read operation */
    RS_PIN = 0;                 /* for selecting AC for read */
    DelayEPulseWidth();
    E_PIN = 1;                  /* for enabling read/write */
  DelayEPulseWidth();

    temp[6] = (char) READ_PIN_6;
    temp[6] = temp[6] << 6;
    temp[5] = (char) READ_PIN_5;
    temp[5] = temp[5] << 5;
    temp[4] = (char) READ_PIN_4;
    temp[4] = temp[4] << 4;

#ifdef EIGHT_BIT_INTERFACE
    temp[3] = (char) READ_PIN_3;
    temp[3] = temp[3] << 3;
    temp[2] = (char) READ_PIN_2;
    temp[2] = temp[2] << 2;
    temp[1] = (char) READ_PIN_1;
    temp[1] = temp[1] << 1;
    temp[0] = (char) READ_PIN_0;
    E_PIN = 0;                  /* disable read/write */
#else
    E_PIN = 0;                  /* disable read/write */

    RW_PIN = 1;                 /* for read operation */
    RS_PIN = 0;                 /* for selecting AC for read */
    E_PIN = 1;                  /* for enabling read/write */
   DelayEPulseWidth();
    temp[3] = (char) READ_PIN_7;
    temp[3] = temp[3] << 3;
    temp[2] = (char) READ_PIN_6;
    temp[2] = temp[2] << 2;
    temp[1] = (char) READ_PIN_5;
    temp[1] = temp[1] << 1;
    temp[0] = (char) READ_PIN_4;
    E_PIN = 0;                  /* disable read/write */
#endif
    for (i=0;i<8;i++)
    {
        address |= temp[i];
    }

    return address;
} 

/*******************************************************************
Function Name   :  ReadDataLCD
Description     :  This function read the data byte from the 
                   LCD controller. The user must first check 
                   the busy status of the LCD controller by 
                   calling the BusyXLCD() function.The data 
                   read can be from DDRam or CGRam  depending 
                   on the previous SetXXRamAddr( char) that was
                   called.
Input parameters:  none
Return value    :  char data
********************************************************************/

char readDataLCD(void)
{
    char data;
    char temp[8];
	int i;

    /* Configure all pins as input */
    TRIS_DATA_PIN_7 = 1;
    TRIS_DATA_PIN_6 = 1;
    TRIS_DATA_PIN_5 = 1;
    TRIS_DATA_PIN_4 = 1;

# ifdef EIGHT_BIT_INTERFACE
    TRIS_DATA_PIN_3 = 1;
    TRIS_DATA_PIN_2 = 1;
    TRIS_DATA_PIN_1 = 1;
    TRIS_DATA_PIN_0 = 1;
#endif

    RW_PIN = 1;                 /* for read operation */
    RS_PIN = 1;                 /* for selecting Data Reg for read */
    DelayEPulseWidth();
    E_PIN = 1;                  /* for enabling read/write */
    DelayEPulseWidth();
    temp[7] = (char) READ_PIN_7;
    temp[7] = temp[7] << 7;
    temp[6] = (char) READ_PIN_6;
    temp[6] = temp[6] << 6;
    temp[5] = (char) READ_PIN_5;
    temp[5] = temp[5] << 5;
    temp[4] = (char) READ_PIN_4;
    temp[4] = temp[4] << 4;

#ifdef EIGHT_BIT_INTERFACE
    temp[3]  = (char) READ_PIN_3;
    temp[3] = temp[3] << 3;
    temp[2]  = (char) READ_PIN_2;
    temp[2] = temp[2] << 2;
    temp[1]  = (char) READ_PIN_1;
    temp[1] = temp[1] << 1;
    temp[0]  = (char) READ_PIN_0;
    E_PIN =0;                   /* disable read/write */

#else
    E_PIN = 0;                  /* disable read/write */
    RW_PIN = 1;                 /* for read operation */
    RS_PIN = 1;                 /* for selecting DR for read */
    E_PIN = 1;                  /* for enabling read/write */
    DelayEPulseWidth();

    temp[3] = (char) READ_PIN_7;
    temp[3] = temp[3] << 3;
    temp[2] = (char) READ_PIN_6;
    temp[2] = temp[2] << 2;
    temp[1] = (char) READ_PIN_5;
    temp[1] = temp[1] << 1;
    temp[0] = (char) READ_PIN_4;

    E_PIN = 0;                  /* disable read/write */
#endif
    for (i=0;i<8;i++)
    {
        data |= temp[i];
    }

    return data;
} /* end of function */


/*******************************************************************
Function Name :         WriteCmdLCD
Description:            This function write a command to the 
                        Instruction Register of the LCD controller.
                        The user must first check if the controller 
                        is busy or not by calling the BusyXLCD()
                        function.
Input parameters:       char cmd
Return value:           void
********************************************************************/
void writeCmdLCD(char cmd)
{

	RW_PIN = 0;   /* enable write */
	RS_PIN = 0;   /* select Instruction Reg */
	
	/* configure the common data pins as output */
	TRIS_DATA_PIN_7 = 0;
	TRIS_DATA_PIN_6 = 0;
	TRIS_DATA_PIN_5 = 0;
	TRIS_DATA_PIN_4 = 0;
	
	DATA_PIN_7 = (unsigned int)((cmd & 0x80)>>7);
	DATA_PIN_6 = (unsigned int)((cmd & 0x40)>>6);
	DATA_PIN_5 = (unsigned int)((cmd & 0x20)>>5);
	DATA_PIN_4 = (unsigned int)((cmd & 0x10)>>4);
	
	# ifdef EIGHT_BIT_INTERFACE
		/* configure the other pins as output */
		TRIS_DATA_PIN_3 = 0;
		TRIS_DATA_PIN_2 = 0;
		TRIS_DATA_PIN_1 = 0;
		TRIS_DATA_PIN_0 = 0;
		
		DATA_PIN_3 = (unsigned int)((cmd & 0x08)>>3);
		DATA_PIN_2 = (unsigned int)((cmd & 0x04)>>2);
		DATA_PIN_1 = (unsigned int)((cmd & 0x02)>>1);
		DATA_PIN_0 = (unsigned int)(cmd & 0x01);
		
		E_PIN = 1;
		DelayEPulseWidth();
		E_PIN = 0;
	# else
		E_PIN = 1;
		DelayEPulseWidth();
		E_PIN = 0;
		
		RW_PIN = 0;  /* enable write */
		RS_PIN = 0;  /* select Instruction Reg */
		
		DATA_PIN_7 = (unsigned int)((cmd & 0x08)>>3);
		DATA_PIN_6 = (unsigned int)((cmd & 0x04)>>2);
		DATA_PIN_5 = (unsigned int)((cmd & 0x02)>>1);
		DATA_PIN_4 = (unsigned int)(cmd & 0x01);
		
		E_PIN = 1;
		DelayEPulseWidth();
		E_PIN = 0;
	#endif
	
	while(busyLCD());

}  /* end of function */


/*******************************************************************
Function Name:            WriteDataLCD
Description:              This function writes a data byte into the 
                          DDRam/CGRam of the LCD controller.The user
                          must first check the busy  status of the 
                          LCD controller  by calling the BusyXLCD()
                          function.The data written can go to the 
                          DDRam or CGRam depending on the previous
                          SetXXRamAddr( char) that was  called.
Input parameters:         char data
Return value:             void
********************************************************************/
//int Leonard1 = 0;
void writeDataLCD( char data)
{
    RW_PIN = 0;  /* enable write */
    RS_PIN = 1;  /* select Data Reg */
                     
    /* configure the common data pins as output */
    TRIS_DATA_PIN_7 = 0;
    TRIS_DATA_PIN_6 = 0;
    TRIS_DATA_PIN_5 = 0;
    TRIS_DATA_PIN_4 = 0;

    DATA_PIN_7 = (unsigned int)((data & 0x80)>>7);
    DATA_PIN_6 = (unsigned int)((data & 0x40)>>6);
    DATA_PIN_5 = (unsigned int)((data & 0x20)>>5);
    DATA_PIN_4 = (unsigned int)((data & 0x10)>>4);

# ifdef EIGHT_BIT_INTERFACE
    /* configure the other pins as output */
    TRIS_DATA_PIN_3 = 0;
    TRIS_DATA_PIN_2 = 0;
    TRIS_DATA_PIN_1 = 0;
    TRIS_DATA_PIN_0 = 0;

    DATA_PIN_3 = (unsigned int)((data & 0x08)>>3);
    DATA_PIN_2 = (unsigned int)((data & 0x04)>>2);
    DATA_PIN_1 = (unsigned int)((data & 0x02)>>1);
    DATA_PIN_0 = (unsigned int)(data & 0x01);

    E_PIN = 1;
    DelayEPulseWidth();
    E_PIN = 0;

# else
    E_PIN = 1;
   DelayEPulseWidth();
    E_PIN = 0;

    RW_PIN = 0;   /* enable write */
    RS_PIN = 1;   /* select Data Reg */
    DATA_PIN_7 = (unsigned int)((data & 0x08)>>3);
    DATA_PIN_6 = (unsigned int)((data & 0x04)>>2);
    DATA_PIN_5 = (unsigned int)((data & 0x02)>>1);
    DATA_PIN_4 = (unsigned int)(data & 0x01);

    E_PIN = 1;
   DelayEPulseWidth( );
    E_PIN = 0;
    
#endif

    /*configure all pins to input mode*/
    TRIS_DATA_PIN_7 = 1;
    TRIS_DATA_PIN_6 = 1;
    TRIS_DATA_PIN_5 = 1;
    TRIS_DATA_PIN_4 = 1;
# ifdef EIGHT_BIT_INTERFACE
    TRIS_DATA_PIN_3 = 1;
    TRIS_DATA_PIN_2 = 1;
    TRIS_DATA_PIN_1 = 1;
    TRIS_DATA_PIN_0 = 1;
#endif
    RS_PIN = 0; 


while(busyLCD());
}  /* end of function */



/*******************************************************************
Function Name:          PutsLCD
Description:            This function writes a string of characters 
                        into the LCD controller.it stops transmission
                        before the null character.(which is not sent).
Input parameters:       char  *buffer 
Return value:           void
********************************************************************/

void putsLCD(char *buffer)
{

    while(*buffer != '\0')
    {
        writeDataLCD(*buffer); /* calling another function */
                                /* to write each char to the lcd module */
        buffer++;
    }
} /* end of function */


/*************** LIBRERIAS DE RETARDO *******************************************/

void DelayEPulseWidth(void)   	/* provides delay for 18Tcy */
{
	/* This is the width of the E signal		*/
	/* required by the LCD controlller			*/
	int i;
	for(i=0;i<50;i++)
		asm("clrwdt");
}

void DelayPORLCD(void) 	/* provides delay for atleast 15msec */
{
    /* Give a delay of atleast 15 msec after power up	*/
   long i;
    for(i=0;i < 80000;i++)
        asm("clrwdt");
}

void DelayLCD(void)  		/* provides delay for atleast 5msec */
{
  long i;
    for(i=0;i < 9000;i++)
        asm("clrwdt");
}

void Delay600nsLCD(void)		/* provides delay for 600ns */
{
	/* This delay is required during the read operation	*/
	
    int i;
    for(i=0;i<10;i++)
        asm("clrwdt");
}

void Delay200usLCD(void)	/* provides delay for 200uS */
{
    int i;
    for(i=0;i<1000;i++)
        asm("clrwdt");
}
