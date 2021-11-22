EESchema Schematic File Version 4
EELAYER 30 0
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
L Device:Rotary_Encoder_Switch SW1
U 1 1 611886A8
P 5650 3900
F 0 "SW1" H 5650 4267 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 5650 4176 50  0000 C CNN
F 2 "chillpizza:Rotary_Encoder_Switched_PEC11R" H 5500 4060 50  0001 C CNN
F 3 "~" H 5650 4160 50  0001 C CNN
	1    5650 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_SwitchT J3
U 1 1 611893F6
P 5650 2250
F 0 "J3" H 5682 2575 50  0000 C CNN
F 1 "AudioJack2_SwitchT" H 5682 2484 50  0000 C CNN
F 2 "audiojacks:Jack_3.5mm_QingPu_WQP-PJ3410_Vertical" H 5650 2250 50  0001 C CNN
F 3 "~" H 5650 2250 50  0001 C CNN
	1    5650 2250
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2_SwitchT J4
U 1 1 611896BC
P 5650 2950
F 0 "J4" H 5682 3275 50  0000 C CNN
F 1 "AudioJack2_SwitchT" H 5682 3184 50  0000 C CNN
F 2 "audiojacks:Jack_3.5mm_QingPu_WQP-PJ3410_Vertical" H 5650 2950 50  0001 C CNN
F 3 "~" H 5650 2950 50  0001 C CNN
	1    5650 2950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Female J2
U 1 1 619B10D2
P 7450 4700
F 0 "J2" H 7478 4726 50  0000 L CNN
F 1 "Conn_01x05_Female" H 7478 4635 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7450 4700 50  0001 C CNN
F 3 "~" H 7450 4700 50  0001 C CNN
	1    7450 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 2550 7250 2250
Wire Wire Line
	7250 2250 5850 2250
Wire Wire Line
	7250 2650 6000 2650
Wire Wire Line
	6000 2650 6000 2950
Wire Wire Line
	6000 2950 5850 2950
$Comp
L power:GND #PWR0101
U 1 1 619B9CF0
P 6250 2150
F 0 "#PWR0101" H 6250 1900 50  0001 C CNN
F 1 "GND" H 6255 1977 50  0000 C CNN
F 2 "" H 6250 2150 50  0001 C CNN
F 3 "" H 6250 2150 50  0001 C CNN
	1    6250 2150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 619BA8CD
P 6250 2550
F 0 "#PWR0102" H 6250 2300 50  0001 C CNN
F 1 "GND" H 6255 2377 50  0000 C CNN
F 2 "" H 6250 2550 50  0001 C CNN
F 3 "" H 6250 2550 50  0001 C CNN
	1    6250 2550
	-1   0    0    1   
$EndComp
Wire Wire Line
	5850 2150 6250 2150
Wire Wire Line
	6250 2550 5900 2550
Wire Wire Line
	5900 2550 5900 2850
Wire Wire Line
	5900 2850 5850 2850
NoConn ~ 5850 2350
NoConn ~ 5850 3050
Wire Wire Line
	7250 2750 6750 2750
Wire Wire Line
	6750 2750 6750 4800
Wire Wire Line
	6750 4800 7250 4800
Wire Wire Line
	7250 2850 6200 2850
Wire Wire Line
	6200 2850 6200 3250
Wire Wire Line
	6200 3250 4950 3250
Wire Wire Line
	4950 3250 4950 4000
Wire Wire Line
	4950 4000 5350 4000
Wire Wire Line
	7250 2950 6300 2950
Wire Wire Line
	6300 2950 6300 3350
Wire Wire Line
	6300 3350 5050 3350
Wire Wire Line
	5050 3350 5050 3900
Wire Wire Line
	5050 3900 5350 3900
Wire Wire Line
	7250 3050 6400 3050
Wire Wire Line
	6400 3050 6400 3450
Wire Wire Line
	6400 3450 5150 3450
Wire Wire Line
	5150 3450 5150 3800
Wire Wire Line
	5150 3800 5350 3800
Wire Wire Line
	7250 3150 6500 3150
Wire Wire Line
	6500 3150 6500 3800
Wire Wire Line
	6500 3800 5950 3800
Wire Wire Line
	7250 3250 6850 3250
Wire Wire Line
	6850 3250 6850 4700
Wire Wire Line
	6850 4700 7250 4700
Wire Wire Line
	7250 3350 6950 3350
Wire Wire Line
	6950 3350 6950 4600
Wire Wire Line
	6950 4600 7250 4600
$Comp
L Connector:Conn_01x11_Female J1
U 1 1 619BFF5F
P 7450 3050
F 0 "J1" H 7478 3076 50  0000 L CNN
F 1 "Conn_01x11_Female" H 7478 2985 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x11_P2.54mm_Horizontal" H 7450 3050 50  0001 C CNN
F 3 "~" H 7450 3050 50  0001 C CNN
	1    7450 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 619C3012
P 7050 3550
F 0 "#PWR0103" H 7050 3300 50  0001 C CNN
F 1 "GND" H 7055 3377 50  0000 C CNN
F 2 "" H 7050 3550 50  0001 C CNN
F 3 "" H 7050 3550 50  0001 C CNN
	1    7050 3550
	1    0    0    -1  
$EndComp
$Comp
L power:VDD #PWR0104
U 1 1 619C3562
P 7250 3550
F 0 "#PWR0104" H 7250 3400 50  0001 C CNN
F 1 "VDD" H 7268 3723 50  0000 C CNN
F 2 "" H 7250 3550 50  0001 C CNN
F 3 "" H 7250 3550 50  0001 C CNN
	1    7250 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	7250 3450 7050 3450
Wire Wire Line
	7050 3450 7050 3550
$Comp
L power:VDD #PWR0105
U 1 1 619C42CF
P 7050 4500
F 0 "#PWR0105" H 7050 4350 50  0001 C CNN
F 1 "VDD" H 7067 4673 50  0000 C CNN
F 2 "" H 7050 4500 50  0001 C CNN
F 3 "" H 7050 4500 50  0001 C CNN
	1    7050 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 619C52F5
P 7050 4900
F 0 "#PWR0106" H 7050 4650 50  0001 C CNN
F 1 "GND" H 7055 4727 50  0000 C CNN
F 2 "" H 7050 4900 50  0001 C CNN
F 3 "" H 7050 4900 50  0001 C CNN
	1    7050 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 4500 7050 4500
Wire Wire Line
	7250 4900 7050 4900
$Comp
L power:GND #PWR0107
U 1 1 619C8244
P 6100 4000
F 0 "#PWR0107" H 6100 3750 50  0001 C CNN
F 1 "GND" H 6105 3827 50  0000 C CNN
F 2 "" H 6100 4000 50  0001 C CNN
F 3 "" H 6100 4000 50  0001 C CNN
	1    6100 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 4000 6100 4000
$EndSCHEMATC
