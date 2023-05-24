
//Axis Definitions
#define AXIS_PUSHER	0
#define pAXIS_PUSHER ch0

#define AXIS_HEATER_PID	1
#define pAXIS_HEATER_PID ch1

#define AXIS_LOAD_PID	2
#define pAXIS_LOAD_PID ch2

//virtual bits to document the status of homing
#define BIT_EXECUTE_AGITATE_A 1056
#define INITIALIZED_BIT 	1119

//Gecko Drive "Enable Pin"
#define GECKO_ENBALE_PIN 45

/*------------------------------------------------------------------------------------
 * User variables definitions
--------------------------------------------------------------------------------------*/
#define UV_TEST_DURATION_MS	10
#define UV_TEST_ROT_ANGLE	11 
#define UV_TEST_ROT_SPEED	12
#define UV_TEST_ROT_ACCEL	13
#define UV_TEST_DWELL		14


#define UV_TC1_FAULT_STATUS   20
#define UV_TC1_NUM_READS       21
#define UV_TC2_FAULT_STATUS   22
#define UV_TC2_NUM_READS       23

#define UV_LOADCELL_SAMPLE_NUM 30
#define UV_LOADCELL_NEWTON_PER_COUNT 31

#define UV_PRINT_REPORT_MAIN_THREAD 50
#define UV_PRINT_REPORT_RTD_THREAD 51
#define UV_PRINT_REPORT_HX711_THREAD 52

//100-107 are uploaded as part of the bulk status register PC_COMM[0] through PC_COMM[7]
#define UV_TEMPERATURE_TC1 100
#define UV_TEMPERATURE_TC2 101
#define UV_PID_HEATER_OUTPUT 102

#define UV_PID_LOAD_OUTPUT 104
#define UV_LOADCELL_COUNTS 105
#define UV_LOADCELL_NEWTONS  106
#define UV_LOADCELL_TARE_CNTS  107

/*------------------------------------------------------------------------------------
 * Sensor Board SPI definitions
--------------------------------------------------------------------------------------*/
//old definitions commented out below, changed on 12.17.2021
//#define CLK_TC (JP6_P10)
//#define MISO_TC (JP6_P11)  	// with respect to KFLOP, MISO
//#define MOSI_TC (JP6_P12)  	// with respect to KFLOP, MOSI
//#define CS_TC1 (JP6_P13)
//#define CS_TC2 (JP6_P14)
#define CLK_TC (JP6_P5)
#define MISO_TC (JP6_P7)  	// with respect to KFLOP, MISO
#define MOSI_TC (JP6_P11)  	// with respect to KFLOP, MOSI
#define CS_TC1 (JP6_P13)
#define CS_TC2 (JP6_P15)

#define DATA_READY_TC1 (JP6_P14)
#define DATA_READY_TC2 (JP6_P16)

/*------------------------------------------------------------------------------------
 * Load Cell Amplifier Board definitions
--------------------------------------------------------------------------------------*/
#define HX711_CLK (JP4_P15)
#define HX711_DATA (JP4_P16)

/* These are for the custom PCB with two Adafruit MAX31856 Breakout boards connected 
   directly to the Kflop JP4 or JP6 header
   #define CLK (16)

   #define MISO (17)  	// with respect to KFLOP, MISO
   #define MOSI (18)  	// with respect to KFLOP, MOSI
   #define CS_TC1 (20)
   #define CS_TC2 (19)
   #define CS_PIXI (21)
/*

/*------------------------------------------------------------------------------------
 * I/O bit definitions
--------------------------------------------------------------------------------------*/
#define DO_HEATER_ON 27  // IO bit controls the heaters SSR

/*------------------------------------------------------------------------------------
 * MACROS
--------------------------------------------------------------------------------------*/
#define OPEN_GRIPPER_A SetBit(SVO_GRIPPER_A)
#define CLOSE_GRIPPER_A ClearBit(SVO_GRIPPER_A)

/*------------------------------------------------------------------------------------
 * MOTOR_POSITIONS
 TODO: Impliment these to control the start of pushing....
--------------------------------------------------------------------------------------*/
#define MP_RAPIDRISE_DROP_A 390
#define MP_RAPIDRISE_PICK_A 0
#define MP_STEADYSTATE_PICK_A 0
#define MP_STEADYSTATE_HEAT_A -400
#define MP_STEADYSTATE_DROP_A -700

#define MP_RAPIDRISE_HOMEOFFSET_A 95
#define MP_STEADYSTATE_HOMEOFFSET_A 650
#define MP_RAPIDRISE_HOMEOFFSET_B 0
#define MP_STEADYSTATE_HOMEOFFSET_B 0
