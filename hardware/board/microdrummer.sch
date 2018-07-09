EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:teensy
LIBS:Switch
LIBS:eurocad
LIBS:Miasma_IO_smd-cache
LIBS:microdrummer-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LED D1
U 1 1 5B39243B
P 6600 3750
F 0 "D1" H 6600 3850 50  0000 C CNN
F 1 "LED" H 6600 3650 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm_FlatTop" H 6600 3750 50  0001 C CNN
F 3 "" H 6600 3750 50  0001 C CNN
	1    6600 3750
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 5B392495
P 6600 4700
F 0 "D2" H 6600 4800 50  0000 C CNN
F 1 "LED" H 6600 4600 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm_FlatTop" H 6600 4700 50  0001 C CNN
F 3 "" H 6600 4700 50  0001 C CNN
	1    6600 4700
	1    0    0    -1  
$EndComp
$Comp
L LED D3
U 1 1 5B392584
P 6600 5650
F 0 "D3" H 6600 5750 50  0000 C CNN
F 1 "LED" H 6600 5550 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm_FlatTop" H 6600 5650 50  0001 C CNN
F 3 "" H 6600 5650 50  0001 C CNN
	1    6600 5650
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5B3931E0
P 6100 2800
F 0 "R1" V 6180 2800 50  0000 C CNN
F 1 "4.7k" V 6100 2800 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6030 2800 50  0001 C CNN
F 3 "" H 6100 2800 50  0001 C CNN
	1    6100 2800
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5B3936D1
P 6100 3250
F 0 "R2" V 6180 3250 50  0000 C CNN
F 1 "4.7k" V 6100 3250 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6030 3250 50  0001 C CNN
F 3 "" H 6100 3250 50  0001 C CNN
	1    6100 3250
	0    1    1    0   
$EndComp
NoConn ~ 10500 2450
NoConn ~ 10500 2550
NoConn ~ 10500 2650
NoConn ~ 10500 2750
NoConn ~ 10500 2850
NoConn ~ 10500 2950
NoConn ~ 10500 3050
NoConn ~ 10500 3150
NoConn ~ 10500 3250
NoConn ~ 10500 3350
NoConn ~ 10500 3450
NoConn ~ 10500 3550
NoConn ~ 10500 3650
NoConn ~ 10500 3750
NoConn ~ 10500 3850
NoConn ~ 10500 3950
NoConn ~ 10500 4150
NoConn ~ 10500 4050
NoConn ~ 10500 4250
NoConn ~ 10500 4350
NoConn ~ 10500 4450
NoConn ~ 10500 4650
NoConn ~ 8500 4950
NoConn ~ 8500 4850
NoConn ~ 8500 4750
NoConn ~ 8500 4550
NoConn ~ 8500 4250
NoConn ~ 8500 4150
NoConn ~ 8500 4050
NoConn ~ 8500 3850
NoConn ~ 8500 3650
NoConn ~ 8500 3550
NoConn ~ 8500 2650
NoConn ~ 8500 2550
NoConn ~ 1400 800 
NoConn ~ 1400 900 
NoConn ~ 1400 1100
NoConn ~ 1400 1500
NoConn ~ 700  1500
NoConn ~ 700  1100
NoConn ~ 700  900 
NoConn ~ 700  800 
$Comp
L SW_SPST SW1
U 1 1 5B3956B4
P 6600 2800
F 0 "SW1" H 6600 2925 50  0000 C CNN
F 1 "SW_SPST" H 6600 2700 50  0000 C CNN
F 2 "Buttons_Switches_THT:Push_E-Switch_KS01Q01" H 6600 2800 50  0001 C CNN
F 3 "" H 6600 2800 50  0001 C CNN
	1    6600 2800
	1    0    0    -1  
$EndComp
$Comp
L SW_SPST SW2
U 1 1 5B395716
P 6600 3250
F 0 "SW2" H 6600 3375 50  0000 C CNN
F 1 "SW_SPST" H 6600 3150 50  0000 C CNN
F 2 "Buttons_Switches_THT:Push_E-Switch_KS01Q01" H 6600 3250 50  0001 C CNN
F 3 "" H 6600 3250 50  0001 C CNN
	1    6600 3250
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5B3A8414
P 6100 3750
F 0 "R3" V 6180 3750 50  0000 C CNN
F 1 "150" V 6100 3750 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6030 3750 50  0001 C CNN
F 3 "" H 6100 3750 50  0001 C CNN
	1    6100 3750
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 5B3A866E
P 6100 4700
F 0 "R4" V 6180 4700 50  0000 C CNN
F 1 "150" V 6100 4700 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6030 4700 50  0001 C CNN
F 3 "" H 6100 4700 50  0001 C CNN
	1    6100 4700
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 5B3A871D
P 6100 5650
F 0 "R5" V 6180 5650 50  0000 C CNN
F 1 "150" V 6100 5650 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6030 5650 50  0001 C CNN
F 3 "" H 6100 5650 50  0001 C CNN
	1    6100 5650
	0    1    1    0   
$EndComp
$Comp
L EURO_PWR_2x8 H1
U 1 1 5B3AB3E1
P 1050 1150
F 0 "H1" H 1050 700 60  0000 C CNN
F 1 "EURO_PWR_2x8" H 1050 1600 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x08_Pitch2.54mm" H 1150 1300 60  0001 C CNN
F 3 "" H 1150 1300 60  0000 C CNN
	1    1050 1150
	1    0    0    -1  
$EndComp
$Comp
L Thonk J1
U 1 1 5B3AC17B
P 6700 2300
F 0 "J1" H 6700 2300 60  0000 C CNN
F 1 "Thonk" H 6700 2450 60  0000 C CNN
F 2 "zetaohm:THONKICONN" H 6700 2300 60  0001 C CNN
F 3 "" H 6700 2300 60  0000 C CNN
	1    6700 2300
	1    0    0    -1  
$EndComp
$Comp
L Thonk J2
U 1 1 5B3AC5C6
P 6700 4300
F 0 "J2" H 6700 4300 60  0000 C CNN
F 1 "Thonk" H 6700 4450 60  0000 C CNN
F 2 "zetaohm:THONKICONN" H 6700 4300 60  0001 C CNN
F 3 "" H 6700 4300 60  0000 C CNN
	1    6700 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 1000 10800 1000
Wire Wire Line
	10800 1000 10800 4550
Wire Wire Line
	10800 4550 10500 4550
Wire Wire Line
	1400 1200 8500 1200
Wire Wire Line
	8500 1200 8500 2450
Wire Wire Line
	6400 2800 6250 2800
Wire Wire Line
	6250 3250 6400 3250
Connection ~ 5700 1200
Wire Wire Line
	5700 2800 5950 2800
Connection ~ 5700 3750
Wire Wire Line
	7000 2400 8400 2400
Wire Wire Line
	8400 2400 8400 2750
Wire Wire Line
	8400 2750 8500 2750
Wire Wire Line
	6400 2800 6400 2600
Wire Wire Line
	6400 2600 8350 2600
Wire Wire Line
	8350 2600 8350 2850
Wire Wire Line
	8350 2850 8500 2850
Connection ~ 5700 2800
Wire Wire Line
	5700 3250 5950 3250
Connection ~ 5700 3250
Wire Wire Line
	6750 4700 8200 4700
Wire Wire Line
	6750 3750 8100 3750
Wire Wire Line
	5700 1200 5700 5650
Wire Wire Line
	6450 3750 6250 3750
Wire Wire Line
	5950 3750 5700 3750
Wire Wire Line
	6450 4700 6250 4700
Wire Wire Line
	6450 5650 6250 5650
Wire Wire Line
	7000 2000 7000 2300
Wire Wire Line
	7000 2000 5700 2000
Connection ~ 5700 2000
Connection ~ 7000 2200
Wire Wire Line
	7000 4000 7000 4300
Connection ~ 7000 4200
Wire Wire Line
	8150 4400 7000 4400
Wire Wire Line
	8300 5650 6750 5650
Wire Wire Line
	5700 5650 5950 5650
$Comp
L Thonk J3
U 1 1 5B3AC9BD
P 6700 5250
F 0 "J3" H 6700 5250 60  0000 C CNN
F 1 "Thonk" H 6700 5400 60  0000 C CNN
F 2 "zetaohm:THONKICONN" H 6700 5250 60  0001 C CNN
F 3 "" H 6700 5250 60  0000 C CNN
	1    6700 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4950 7000 5250
Wire Wire Line
	5700 4950 7000 4950
Connection ~ 5700 4950
Connection ~ 7000 5150
Wire Wire Line
	1400 1200 1400 1400
Connection ~ 1400 1300
Wire Wire Line
	700  1200 700  1400
Connection ~ 700  1300
Wire Wire Line
	700  1400 600  1400
Wire Wire Line
	600  1400 600  1700
Wire Wire Line
	600  1700 1500 1700
Wire Wire Line
	1500 1700 1500 1400
Wire Wire Line
	1500 1400 1400 1400
Wire Wire Line
	700  1000 600  1000
Wire Wire Line
	600  1000 600  600 
Wire Wire Line
	600  600  1500 600 
Wire Wire Line
	1500 600  1500 1000
Connection ~ 1500 1000
NoConn ~ 8500 2950
$Comp
L Teensy3.2 U1
U 1 1 5B3923DD
P 9500 3750
F 0 "U1" H 9500 5250 60  0000 C CNN
F 1 "Teensy3.2" H 9500 2250 60  0000 C CNN
F 2 "teensy:Teensy30_31_32_LC_stdvgHeaders" H 9500 2950 60  0001 C CNN
F 3 "" H 9500 2950 60  0000 C CNN
	1    9500 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3050 8350 4450
Wire Wire Line
	6400 3050 8350 3050
Wire Wire Line
	6400 3250 6400 3050
Wire Wire Line
	5600 5900 8400 5900
Wire Wire Line
	5600 2950 5600 5900
Wire Wire Line
	5600 3450 6800 3450
Wire Wire Line
	6800 3450 6800 3250
Wire Wire Line
	6800 2800 6800 2950
Wire Wire Line
	6800 2950 5600 2950
Connection ~ 5600 3450
Text Label 7600 2400 0    60   ~ 0
SigIn
Text Label 7600 2600 0    60   ~ 0
BtnIn
Text Label 7600 3050 0    60   ~ 0
BtnClr
Text Label 7600 3750 0    60   ~ 0
SigLed
Text Label 7600 4700 0    60   ~ 0
GatLed
Text Label 7600 4400 0    60   ~ 0
GatOut
Text Label 7600 5650 0    60   ~ 0
TrgLed
Text Label 7600 5350 0    60   ~ 0
TrgOut
Wire Wire Line
	7000 4000 5700 4000
Connection ~ 5700 4000
Wire Wire Line
	5950 4700 5700 4700
Connection ~ 5700 4700
Wire Wire Line
	8250 5350 7000 5350
Wire Wire Line
	8400 5900 8400 3950
Wire Wire Line
	8400 3950 8500 3950
NoConn ~ 10500 4750
Wire Wire Line
	8350 4450 8500 4450
Wire Wire Line
	8500 4650 8300 4650
Wire Wire Line
	8300 4650 8300 5650
Wire Wire Line
	8500 5050 8250 5050
Wire Wire Line
	8250 5050 8250 5350
Wire Wire Line
	10500 5050 10500 5400
Wire Wire Line
	10500 5400 8200 5400
Wire Wire Line
	8200 5400 8200 4700
Wire Wire Line
	10500 4950 10550 4950
Wire Wire Line
	10550 4950 10550 5450
Wire Wire Line
	10550 5450 8150 5450
Wire Wire Line
	8150 5450 8150 4400
Wire Wire Line
	10500 4850 10600 4850
Wire Wire Line
	10600 4850 10600 5500
Wire Wire Line
	10600 5500 8100 5500
Wire Wire Line
	8100 5500 8100 3750
NoConn ~ 8500 4350
NoConn ~ 8500 3750
NoConn ~ 8500 3450
NoConn ~ 8500 3350
NoConn ~ 8500 3250
NoConn ~ 8500 3150
NoConn ~ 8500 3050
$EndSCHEMATC