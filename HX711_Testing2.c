/* 
  This program services reading the HX711 Load Cell interface board.
  It continously reads the load value and sets persistance variables with the data.
  This way, the program can run on an independent thread and share the sensor data
  with a main program running on a seperate thread
*/


#include "KMotionDef.h"		// kmotion includ file
#include "KflopIOdefs.h"	// names for kflop IO, i.e. #define JP4_P5 (16)
#include "IO_Map.h"			// IO pin mapping definitions

//Convinient access to the persistant user varibales that are shared accross threads
int *load_counts = &persist.UserData[UV_LOADCELL_COUNTS];;
float *LoadCell_Newton = (float *)&persist.UserData[UV_LOADCELL_NEWTONS];
float *NewtonPerCount = (float *)&persist.UserData[UV_LOADCELL_NEWTON_PER_COUNT];


//function prototypes here
//-------------------------------------------------------------------------------------------------------
//I absolutely need this prototype in here!!!
unsigned long readLoadCounts(void);

int counter=0;
double lastTime=0;


main()
{
	printf("Started Load Cell Monitoring Thread\n");
	
	// inititalize the scale factor between ADC counts and load in KG
	// the PC applications should store and update this
	*NewtonPerCount=0.00016370714;
	
	//initialize the load cell sample counter
	persist.UserData[UV_LOADCELL_SAMPLE_NUM]=0;
	persist.UserData[UV_PRINT_REPORT_HX711_THREAD]=1;
	
	// initialize the IO directions for the MAX Thermocouple SPI interface
    SetBitDirection(HX711_CLK, 1);		// make serial clock pin an output
    ClearBit(HX711_CLK);				// idle state of clock is low
    SetBitDirection(HX711_DATA, 0);		// ensure serial data pin is an input
    
    Delay_sec(0.1);						// probably should remove this delay
    
    lastTime=Time_sec();
     
    int i;
    for (;;)
    {
        //Delay_sec(1.0);  		// loop ~every 1000 milliseconds
        int lc_counts = readLoadCounts();  //invert the counts so a positive value for tension on load cell
		int tared_counts = lc_counts - persist.UserData[UV_LOADCELL_TARE_CNTS];  //tare it out
		float Newtons = tared_counts * (*NewtonPerCount);  //caclulate kg off of tared value
		
		//update the user variabls for access from other threads
		*load_counts = lc_counts;							// update the persist load cell Raw ADC counts variable
		*LoadCell_Newton = Newtons;								// update the persist load cell KG variable
		persist.UserData[UV_LOADCELL_SAMPLE_NUM] += 1;	// increment read counter for main loop diagnostics
		counter+=1;
		
		if(Time_sec()-lastTime >=1)
		{
			if(persist.UserData[UV_PRINT_REPORT_HX711_THREAD]){        
				printf("ADC= %d | Load= %f N | Samples= %d \n",load_counts, Newtons, counter);
			}		
			counter=0;
			lastTime=Time_sec();
		}
    }
}

/* Read Load Counts
// ****************************************************************************************************************
The load cell is connected to an HX711 amplified board with a 24bit ADC and internal amplifier with gain of 128
The data from the amplifier board is a signed (2's compliment) 24bit number.
When 5V supply is used at the AVDD pin, the internal 128 gain gives a full-scale differential input voltage of �20mV
The load cell has a 2mV/Volt output at full scale (50Kg), so at 5Volt supply:
	50Kg = 10mV = (2^23 ADCcounts/20mV)*10mV = 4,194,304 ADC counts

The data is clocked out of a serial data pin using a synchronous clock pin. The clock is idle low, and each low-high-low
transition will transfer 1 bit out (MSB).  The total data frame is 25 clocks to read channel A with gain of 128.

We could also issue 26 clocks for Channel B, or 27 clocks for reading Channel A with a gain of 64.

Important Note: per the data sheet, ther is a 50us Max time limit on serial clock high time
// I have found that if you don't put in a waitnexttimeslice to ensure the routing doesn't 
// get interuppted when sclk is high, I will have occasional erroneous readings!!!
// ****************************************************************************************************************/
int readLoadCounts(void){
	
	unsigned long Count;
	unsigned char i;
	int retVal;

	ClearBit(HX711_CLK);		// ADSK=0;
	Count=0;
	
	//wait for data bit to go low, indicating we have a reading to retrieve
	while(ReadBit(HX711_DATA));
	
	//do I need a small delay here?
	
	//now shift in the data
	for (i=0;i<24;i++){
		// the HX711 datasheet specs the max time the serial clock can be high is 50us
		// to ensure that we don't get interrupted with the clock high, wait here for the next
		// thread time slice
		if(i==0 || i==13)
		{
			WaitNextTimeSlice();	//so we don't violate the 50us Max Clock High time (testing this)	
		}
		SetBit(HX711_CLK);		// ADSK=1;
		Count=Count<<1;
		ClearBit(HX711_CLK);	// ADSK=0;
		if(ReadBit(HX711_DATA)) Count++;
	}
	
	//now issue one last serial clock to set input chA / 128 gain.
	SetBit(HX711_CLK);			// ADSK=1;
	
	//if the top bit is set, we have a negative reading, get the twos compliment
	if(Count & (1<<23))
	{
		retVal=Count-16777216;
	}
	else retVal=Count;
	//Count=Count^0x800000;
	ClearBit(HX711_CLK);		// ADSK=0;
	return(retVal);
}



//****************************************************************************************************************
// software spi transfer, fast
// uses FPGA calls instead of SetBit,ClearBit because they are significantly faster.  Could be optimized for speed
//further by not using for loops, but at the cost of much longer, harder to read code.
//****************************************************************************************************************
/*
unsigned int spiTxferFastBits(struct SPItype *spi, int data, int bitCount) {
	int i;
	unsigned int dataIn=0;
	
	//int pSS=spi->CS_pin;
	int pMOSI=spi->MOSI_pin;
	int pMISO=spi->MISO_pin;
	int pSCK=spi->CLK_pin;

	//take chip select low to activate SPI communications
   	//FPGA(BIT_CLR+(pSS>>3)) = ~(1<<(pSS&7)); 
   	
   	for(i=(bitCount-1); i>=0; i--) {
		//set the MOSI pin level with the current data bit
       	if (data & (1<<i))
			FPGA(BIT_SET+(pMOSI>>3)) = 1<<(pMOSI&7);
		else 
			FPGA(BIT_CLR+(pMOSI>>3)) = ~(1<<(pMOSI&7));
		
		//take the clock high to trigger reading
		FPGA(BIT_SET+(pSCK>>3)) = 1<<(pSCK&7);
		
		//read in the data bit on the MISO pin
		dataIn = (dataIn << 1) | ((FPGA(BIT_READ+(pMISO>>3))>>(pMISO&7)) & 1);
		
		//take the clock signal low
		FPGA(BIT_CLR+(pSCK>>3)) = ~(1<<(pSCK&7));
	}
	
	return dataIn;
}
*/