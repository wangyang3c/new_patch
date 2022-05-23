EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	3050 2700 5150 2700
Wire Wire Line
	3050 2700 3050 3300
Wire Wire Line
	3900 2850 5150 2850
Wire Wire Line
	4750 3300 4750 3000
Wire Wire Line
	4750 3000 5150 3000
Wire Wire Line
	6500 3550 6350 3550
Wire Wire Line
	6350 3550 6350 3350
Wire Wire Line
	6350 4250 6350 4550
Wire Wire Line
	6350 4250 6500 4250
Wire Wire Line
	6350 4550 7250 4550
Wire Wire Line
	6350 3350 6150 3350
Connection ~ 6350 3350
Connection ~ 6350 4550
Text Notes 5050 6600 0    50   ~ 0
PSOC connection
Text GLabel 5150 2700 2    50   Input ~ 0
Rgain3
Text GLabel 5150 2850 2    50   Input ~ 0
Rgain4
$Comp
L Connector_Generic:Conn_01x10 J10
U 1 1 619AEC27
P 6150 5900
F 0 "J10" H 6100 6650 50  0000 C CNN
F 1 "Conn_01x10" H 6100 6550 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x10_P2.54mm_Horizontal" H 6150 5900 50  0001 C CNN
F 3 "~" H 6150 5900 50  0001 C CNN
	1    6150 5900
	0    -1   1    0   
$EndComp
Text GLabel 7300 3350 2    50   Input ~ 0
Inv1
Text GLabel 7300 3550 2    50   Input ~ 0
Out1
Text GLabel 7250 4550 2    50   Input ~ 0
Inv2
Text GLabel 7250 4250 2    50   Input ~ 0
Out2
Text GLabel 6450 5700 1    50   Input ~ 0
Out1
Text GLabel 6550 5700 1    50   Input ~ 0
Inv2
Text GLabel 6650 5700 1    50   Input ~ 0
Out2
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 619B2CD4
P 3250 3300
F 0 "J4" H 3330 3292 50  0000 L CNN
F 1 "Conn_01x02" H 3330 3201 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 3250 3300 50  0001 C CNN
F 3 "~" H 3250 3300 50  0001 C CNN
	1    3250 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 619B5DDB
P 4100 3300
F 0 "J5" H 4180 3292 50  0000 L CNN
F 1 "Conn_01x02" H 4180 3201 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 4100 3300 50  0001 C CNN
F 3 "~" H 4100 3300 50  0001 C CNN
	1    4100 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2850 3900 3300
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 619B6941
P 4950 3300
F 0 "J6" H 5030 3292 50  0000 L CNN
F 1 "Conn_01x02" H 5030 3201 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 4950 3300 50  0001 C CNN
F 3 "~" H 4950 3300 50  0001 C CNN
	1    4950 3300
	1    0    0    -1  
$EndComp
Text Notes 4350 4850 0    50   ~ 0
PSOC amplifier configuration
$Comp
L Connector_Generic:Conn_01x02 J7
U 1 1 619CCC2B
P 6500 3750
F 0 "J7" V 6550 3300 50  0000 L CNN
F 1 "Conn_01x02" V 6450 3100 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 6500 3750 50  0001 C CNN
F 3 "~" H 6500 3750 50  0001 C CNN
	1    6500 3750
	0    -1   1    0   
$EndComp
Wire Wire Line
	6600 3550 7300 3550
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 619CE2BD
P 6500 4050
F 0 "J8" V 6550 3600 50  0000 L CNN
F 1 "Conn_01x02" V 6450 3400 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 6500 4050 50  0001 C CNN
F 3 "~" H 6500 4050 50  0001 C CNN
	1    6500 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6600 4250 7250 4250
Wire Wire Line
	6350 3350 7300 3350
Text GLabel 5150 3000 2    50   Input ~ 0
Rgain5
Text GLabel 6150 3350 0    50   Input ~ 0
RgainSelect
Text GLabel 5850 5700 1    50   Input ~ 0
Rgain2
Text GLabel 5750 5700 1    50   Input ~ 0
Rgain1
Text GLabel 5950 5700 1    50   Input ~ 0
Rgain3
Text GLabel 6350 5700 1    50   Input ~ 0
Inv1
Text GLabel 6250 5700 1    50   Input ~ 0
RgainSelect
Text GLabel 7850 5700 1    50   Input ~ 0
4PM-
Text GLabel 7750 5700 1    50   Input ~ 0
4PM+
Text GLabel 7650 5700 1    50   Input ~ 0
ForceBridge-
Text GLabel 7550 5700 1    50   Input ~ 0
ForceBridge+
Text GLabel 7450 5700 1    50   Input ~ 0
SenseOther
Text GLabel 7350 5700 1    50   Input ~ 0
SensePosition
$Comp
L Connector_Generic:Conn_01x06 J11
U 1 1 619BE468
P 7550 5900
F 0 "J11" V 7422 5512 50  0000 R CNN
F 1 "Conn_01x06" V 7513 5512 50  0000 R CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Horizontal" H 7550 5900 50  0001 C CNN
F 3 "~" H 7550 5900 50  0001 C CNN
	1    7550 5900
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J9
U 1 1 6197ABC5
P 4850 5900
F 0 "J9" H 4768 6217 50  0000 C CNN
F 1 "Conn_01x04" H 4768 6126 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Horizontal" H 4850 5900 50  0001 C CNN
F 3 "~" H 4850 5900 50  0001 C CNN
	1    4850 5900
	0    -1   1    0   
$EndComp
Text GLabel 4750 5700 1    50   Input ~ 0
3.3V
Text GLabel 4850 5700 1    50   Input ~ 0
GND
Text GLabel 4950 5700 1    50   Input ~ 0
SCL
Text GLabel 5050 5700 1    50   Input ~ 0
SDA
Text Notes 7950 1450 0    50   ~ 0
Stretch arduino connection
$Comp
L Connector_Generic:Conn_01x10 J1
U 1 1 61979DA6
P 5900 1250
F 0 "J1" H 5818 525 50  0000 C CNN
F 1 "Conn_01x10" H 5818 616 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x10_P2.54mm_Horizontal" H 5900 1250 50  0001 C CNN
F 3 "~" H 5900 1250 50  0001 C CNN
	1    5900 1250
	0    1    -1   0   
$EndComp
Text GLabel 5400 1450 3    50   Input ~ 0
SensePosition
Text GLabel 5500 1450 3    50   Input ~ 0
SenseOther
Text GLabel 5600 1450 3    50   Input ~ 0
ForceBridge+
Text GLabel 5700 1450 3    50   Input ~ 0
ForceBridge-
Text GLabel 5800 1450 3    50   Input ~ 0
4PM+
Text GLabel 5900 1450 3    50   Input ~ 0
4PM-
Text GLabel 6000 1450 3    50   Input ~ 0
3.3V
Text GLabel 6100 1450 3    50   Input ~ 0
GND
Text GLabel 6200 1450 3    50   Input ~ 0
SCL
Text GLabel 6300 1450 3    50   Input ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 619C8A16
P 2450 3300
F 0 "J3" H 2530 3292 50  0000 L CNN
F 1 "Conn_01x02" H 2530 3201 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 2450 3300 50  0001 C CNN
F 3 "~" H 2450 3300 50  0001 C CNN
	1    2450 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 619C8CD9
P 1600 3300
F 0 "J2" H 1680 3292 50  0000 L CNN
F 1 "Conn_01x02" H 1680 3201 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 1600 3300 50  0001 C CNN
F 3 "~" H 1600 3300 50  0001 C CNN
	1    1600 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 3300 2250 2500
Wire Wire Line
	1400 3300 1400 2250
Wire Wire Line
	1400 2250 5100 2250
Wire Wire Line
	2250 2500 5100 2500
Text GLabel 5100 2250 2    50   Input ~ 0
Rgain1
Text GLabel 5100 2500 2    50   Input ~ 0
Rgain2
Text GLabel 6050 5700 1    50   Input ~ 0
Rgain4
Text GLabel 6150 5700 1    50   Input ~ 0
Rgain5
Wire Wire Line
	4750 3400 4750 4550
Connection ~ 4750 4550
Wire Wire Line
	4750 4550 6350 4550
Wire Wire Line
	3900 3400 3900 4550
Connection ~ 3900 4550
Wire Wire Line
	3900 4550 4750 4550
Wire Wire Line
	3050 3400 3050 4550
Connection ~ 3050 4550
Wire Wire Line
	3050 4550 3900 4550
Wire Wire Line
	2250 3400 2250 4550
Wire Wire Line
	1400 4550 2250 4550
Connection ~ 2250 4550
Wire Wire Line
	2250 4550 3050 4550
Wire Wire Line
	1400 3400 1400 4550
$EndSCHEMATC
