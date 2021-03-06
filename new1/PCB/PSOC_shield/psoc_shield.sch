EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "Interface between PSOC and test platform"
Date "2021-11-11"
Rev "V1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 1300 1200 0    79   ~ 0
PSOC
Wire Notes Line
	1000 4900 4500 4900
Wire Notes Line
	1000 900  1000 4900
Wire Notes Line
	4500 900  1000 900 
Wire Notes Line
	5100 4900 5100 900 
$Comp
L Connector_Generic:Conn_02x10_Odd_Even J2
U 1 1 619790B7
P 2050 3750
F 0 "J2" H 2050 4450 50  0000 C CNN
F 1 "Conn_02x10" H 2050 4350 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x10_P2.54mm_Vertical" H 2050 3750 50  0001 C CNN
F 3 "~" H 2050 3750 50  0001 C CNN
	1    2050 3750
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J3
U 1 1 61B4BDE4
P 3650 2050
F 0 "J3" H 3730 2042 50  0000 L CNN
F 1 "Conn_01x10" H 3730 1951 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x10_P2.54mm_Vertical" H 3650 2050 50  0001 C CNN
F 3 "~" H 3650 2050 50  0001 C CNN
	1    3650 2050
	-1   0    0    -1  
$EndComp
Text GLabel 2150 2450 2    50   Input ~ 0
3.3V
Text GLabel 2150 2650 2    50   Input ~ 0
GND
$Comp
L Connector_Generic:Conn_01x08 J1
U 1 1 61B5435C
P 1950 2450
F 0 "J1" H 2030 2442 50  0000 L CNN
F 1 "Conn_01x08" H 2030 2351 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 1950 2450 50  0001 C CNN
F 3 "~" H 1950 2450 50  0001 C CNN
	1    1950 2450
	-1   0    0    -1  
$EndComp
Text GLabel 10950 5550 0    50   Input ~ 0
R_ref1
Text GLabel 10950 6300 0    50   Input ~ 0
R_ref3
Text GLabel 10950 5950 0    50   Input ~ 0
R_ref2
Text GLabel 10950 5550 0    50   Input ~ 0
R_ref1
Connection ~ 13300 5550
Text GLabel 13300 5200 1    50   Input ~ 0
3.3V
Connection ~ 13300 5950
Wire Wire Line
	13300 6300 13300 5950
Wire Wire Line
	13300 5950 13300 5550
$Comp
L Connector_Generic:Conn_01x02 J11
U 1 1 61B82266
P 11500 6100
F 0 "J11" V 11550 5650 50  0000 L CNN
F 1 "Conn_01x02" V 11450 5450 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 11500 6100 50  0001 C CNN
F 3 "~" H 11500 6100 50  0001 C CNN
	1    11500 6100
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J10
U 1 1 61B8200C
P 11500 5750
F 0 "J10" V 11550 5300 50  0000 L CNN
F 1 "Conn_01x02" V 11450 5100 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 11500 5750 50  0001 C CNN
F 3 "~" H 11500 5750 50  0001 C CNN
	1    11500 5750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	13300 5200 13300 5550
$Comp
L Connector_Generic:Conn_01x08 J4
U 1 1 61F46A9F
P 4450 3850
F 0 "J4" H 4530 3842 50  0000 L CNN
F 1 "Conn_01x08" H 4530 3751 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 4450 3850 50  0001 C CNN
F 3 "~" H 4450 3850 50  0001 C CNN
	1    4450 3850
	-1   0    0    1   
$EndComp
Text GLabel 2250 3350 2    47   Input ~ 0
I0
Text GLabel 2250 3450 2    47   Input ~ 0
I1
Text GLabel 2250 3750 2    47   Input ~ 0
I2
Text GLabel 2250 3850 2    47   Input ~ 0
I3
Text GLabel 2250 4050 2    47   Input ~ 0
I4
Text GLabel 2250 4150 2    47   Input ~ 0
I5
Text GLabel 2250 4250 2    47   Input ~ 0
I6
Text GLabel 4650 3550 2    47   Input ~ 0
I7
Text GLabel 1750 3450 0    47   Input ~ 0
O1
Text GLabel 1750 3550 0    47   Input ~ 0
O2
Text GLabel 1750 3650 0    47   Input ~ 0
O3
Text GLabel 1750 3750 0    47   Input ~ 0
O4
Text GLabel 1750 4050 0    47   Input ~ 0
O5
Text GLabel 1750 4150 0    47   Input ~ 0
O6
Text GLabel 1750 4250 0    47   Input ~ 0
O7
Text GLabel 1750 3350 0    47   Input ~ 0
O0
Text GLabel 4650 3950 2    50   Input ~ 0
R_ref3
Text GLabel 4650 4050 2    50   Input ~ 0
R_ref2
Text GLabel 4650 4150 2    50   Input ~ 0
R_ref1
Text GLabel 1750 3850 0    50   Input ~ 0
ADC+
Text GLabel 1750 3950 0    50   Input ~ 0
ADC-
Text GLabel 2250 3550 2    50   Input ~ 0
Follower+
Text GLabel 2250 3650 2    50   Input ~ 0
Follower-
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 61F4F896
P 9500 5700
F 0 "J8" V 9550 5250 50  0000 L CNN
F 1 "Conn_01x02" V 9450 5050 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 9500 5700 50  0001 C CNN
F 3 "~" H 9500 5700 50  0001 C CNN
	1    9500 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 5450 9300 5450
Wire Wire Line
	9300 5450 9300 5700
Text GLabel 4650 3450 2    50   Input ~ 0
GND
Text GLabel 9300 6050 3    50   Input ~ 0
GND
Wire Wire Line
	9300 6050 9300 5800
Wire Wire Line
	9300 5450 9750 5450
Connection ~ 9300 5450
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 61F530B3
P 7050 5700
F 0 "J6" V 7100 5250 50  0000 L CNN
F 1 "Conn_01x02" V 7000 5050 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 7050 5700 50  0001 C CNN
F 3 "~" H 7050 5700 50  0001 C CNN
	1    7050 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 5450 6850 5450
Wire Wire Line
	6850 5450 6850 5700
Text GLabel 6850 6050 3    50   Input ~ 0
GND
Wire Wire Line
	6850 6050 6850 5800
Wire Wire Line
	6850 5450 7300 5450
Connection ~ 6850 5450
Text GLabel 7300 5450 2    50   Input ~ 0
ADC+
Text GLabel 9750 5450 2    50   Input ~ 0
ADC-
Text GLabel 5950 5450 0    50   Input ~ 0
Follower+
Text GLabel 8400 5450 0    50   Input ~ 0
Follower-
Wire Wire Line
	11500 5550 10950 5550
Wire Wire Line
	11500 5950 10950 5950
Wire Wire Line
	11500 6300 10950 6300
$Comp
L Connector_Generic:Conn_01x02 J9
U 1 1 619C6EDF
P 11500 5350
F 0 "J9" V 11550 4900 50  0000 L CNN
F 1 "Conn_01x02" V 11450 4700 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 11500 5350 50  0001 C CNN
F 3 "~" H 11500 5350 50  0001 C CNN
	1    11500 5350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8750 5450 8400 5450
$Comp
L Connector_Generic:Conn_01x02 J7
U 1 1 61F4F402
P 8750 5250
F 0 "J7" V 8800 4800 50  0000 L CNN
F 1 "Conn_01x02" V 8700 4600 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 8750 5250 50  0001 C CNN
F 3 "~" H 8750 5250 50  0001 C CNN
	1    8750 5250
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x16 J12
U 1 1 61F421FD
P 14550 5900
F 0 "J12" V 14767 5846 50  0000 C CNN
F 1 "Conn_01x16" V 14676 5846 50  0000 C CNN
F 2 "Patch:FH19SC16S05SH09" H 14550 5900 50  0001 C CNN
F 3 "~" H 14550 5900 50  0001 C CNN
	1    14550 5900
	-1   0    0    1   
$EndComp
Wire Wire Line
	6300 5450 5950 5450
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 61F53099
P 6300 5250
F 0 "J5" V 6350 4800 50  0000 L CNN
F 1 "Conn_01x02" V 6250 4600 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 6300 5250 50  0001 C CNN
F 3 "~" H 6300 5250 50  0001 C CNN
	1    6300 5250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11600 5550 13300 5550
Wire Wire Line
	11600 5950 13300 5950
Wire Wire Line
	11600 6300 13300 6300
Text GLabel 14750 5900 2    47   Input ~ 0
O0
Text GLabel 14750 6600 2    47   Input ~ 0
O7
Text GLabel 14750 6500 2    47   Input ~ 0
O6
Text GLabel 14750 6400 2    47   Input ~ 0
O5
Text GLabel 14750 6300 2    47   Input ~ 0
O4
Text GLabel 14750 6200 2    47   Input ~ 0
O3
Text GLabel 14750 6100 2    47   Input ~ 0
O2
Text GLabel 14750 6000 2    47   Input ~ 0
O1
Text GLabel 14750 5100 2    47   Input ~ 0
I0
Text GLabel 14750 5200 2    47   Input ~ 0
I1
Text GLabel 14750 5300 2    47   Input ~ 0
I2
Text GLabel 14750 5400 2    47   Input ~ 0
I3
Text GLabel 14750 5500 2    47   Input ~ 0
I4
Text GLabel 14750 5600 2    47   Input ~ 0
I5
Text GLabel 14750 5700 2    47   Input ~ 0
I6
Text GLabel 14750 5800 2    47   Input ~ 0
I7
$EndSCHEMATC
