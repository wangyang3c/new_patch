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
$Comp
L Connector_Generic:Conn_02x10_Odd_Even J2
U 1 1 619790B7
P 2050 3750
F 0 "J2" H 2100 4400 50  0000 C CNN
F 1 "Conn_02x10" H 2100 4300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x10_P2.54mm_Vertical" H 2050 3750 50  0001 C CNN
F 3 "~" H 2050 3750 50  0001 C CNN
	1    2050 3750
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x10_Odd_Even J3
U 1 1 61B4BDE4
P 4400 2100
F 0 "J3" H 4400 2750 50  0000 L CNN
F 1 "Conn_02x10" H 4250 2650 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x10_P2.54mm_Vertical" H 4400 2100 50  0001 C CNN
F 3 "~" H 4400 2100 50  0001 C CNN
	1    4400 2100
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
F 0 "J1" H 1900 3000 50  0000 L CNN
F 1 "Conn_01x08" H 1750 2900 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 1950 2450 50  0001 C CNN
F 3 "~" H 1950 2450 50  0001 C CNN
	1    1950 2450
	-1   0    0    -1  
$EndComp
Text GLabel 7150 2600 0    50   Input ~ 0
REF2
Text GLabel 7150 2250 0    50   Input ~ 0
REF1
Connection ~ 9500 1850
Text GLabel 9500 1500 1    50   Input ~ 0
3.3V
Connection ~ 9500 2250
Wire Wire Line
	9500 2600 9500 2250
Wire Wire Line
	9500 2250 9500 1850
Wire Wire Line
	9500 1500 9500 1850
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J4
U 1 1 61F46A9F
P 4450 3750
F 0 "J4" H 4450 3100 50  0000 L CNN
F 1 "Conn_02x08" H 4250 3200 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x08_P2.54mm_Vertical" H 4450 3750 50  0001 C CNN
F 3 "~" H 4450 3750 50  0001 C CNN
	1    4450 3750
	-1   0    0    -1  
$EndComp
Text GLabel 1750 3450 0    50   Input ~ 0
ADC-
Text GLabel 2250 3550 2    50   Input ~ 0
Follower+
Text GLabel 2250 3650 2    50   Input ~ 0
Follower-
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 61F4F896
P 11050 4100
F 0 "J8" V 11100 3650 50  0000 L CNN
F 1 "Conn_01x02" V 11000 3450 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 11050 4100 50  0001 C CNN
F 3 "~" H 11050 4100 50  0001 C CNN
	1    11050 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 3850 10850 3850
Wire Wire Line
	10850 3850 10850 4100
Text GLabel 10850 4450 3    50   Input ~ 0
GND
Wire Wire Line
	10850 4450 10850 4200
Wire Wire Line
	10850 3850 11300 3850
Connection ~ 10850 3850
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 61F530B3
P 8600 4100
F 0 "J6" V 8650 3650 50  0000 L CNN
F 1 "Conn_01x02" V 8550 3450 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 8600 4100 50  0001 C CNN
F 3 "~" H 8600 4100 50  0001 C CNN
	1    8600 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 3850 8400 3850
Wire Wire Line
	8400 3850 8400 4100
Text GLabel 8400 4450 3    50   Input ~ 0
GND
Wire Wire Line
	8400 4450 8400 4200
Wire Wire Line
	8400 3850 8850 3850
Connection ~ 8400 3850
Text GLabel 8850 3850 2    50   Input ~ 0
ADC+
Text GLabel 11300 3850 2    50   Input ~ 0
ADC-
Text GLabel 7500 3850 0    50   Input ~ 0
Follower+
Text GLabel 9950 3850 0    50   Input ~ 0
Follower-
Wire Wire Line
	7650 1850 7150 1850
Wire Wire Line
	10300 3850 9950 3850
$Comp
L Connector_Generic:Conn_01x02 J7
U 1 1 61F4F402
P 10300 3650
F 0 "J7" V 10350 3200 50  0000 L CNN
F 1 "Conn_01x02" V 10250 3000 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 10300 3650 50  0001 C CNN
F 3 "~" H 10300 3650 50  0001 C CNN
	1    10300 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7850 3850 7500 3850
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 61F53099
P 7850 3650
F 0 "J5" V 7900 3200 50  0000 L CNN
F 1 "Conn_01x02" V 7800 3000 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 7850 3650 50  0001 C CNN
F 3 "~" H 7850 3650 50  0001 C CNN
	1    7850 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7950 1850 9500 1850
Wire Wire Line
	7950 2250 9500 2250
Text Notes 1100 4300 0    63   ~ 0
P10.0\nP10.1\nP10.2\nP10.3\nP10.4\nP10.5\nP10.6\nP6.2\nP6.3\nP13.6
Text Notes 2800 4300 0    63   ~ 0
P9.0\nP9.1\nP9.2\nP9.3\nP9.4\nP9.5\nGND\nP9.6\nP9.7\nP13.7
Text Notes 5050 4200 0    63   ~ 0
P8.7\nP8.6\nP8.5\nP8.4\nP8.3\nP8.2\nP8.1\nP8.0
Text Notes 3600 4200 0    63   ~ 0
P0.2\nP5.6\nP5.5\nP5.4\nP5.3\nP5.2\nP5.1\nP5.0
Text Notes 4900 2650 0    63   ~ 0
P12.4\nP12.5\nP12.6\nGND\nP12.7\nP1.0\nP1.2\nP1.3\nP1.4\nP1.5
Text Notes 3500 2650 0    63   ~ 0
P6.0\nP6.1\nVREF\nGND\nP12.2\nP12.1\nP12.0\nP12.3\nP13.1\nP13.0
Text Notes 1550 2900 0    63   ~ 0
NC\nP6.VDD\nRST\nV3.3\nV5.0\nGND\nGND\nVIN
$Comp
L Connector_Generic:Conn_01x16 J9
U 1 1 61F421FD
P 11200 7000
F 0 "J9" V 11417 6946 50  0000 C CNN
F 1 "Conn_01x16" V 11326 6946 50  0000 C CNN
F 2 "Patch:FH19SC16S05SH09" H 11200 7000 50  0001 C CNN
F 3 "~" H 11200 7000 50  0001 C CNN
	1    11200 7000
	1    0    0    1   
$EndComp
Wire Wire Line
	11000 6300 8950 6300
Wire Wire Line
	11000 6400 9050 6400
Wire Wire Line
	11000 6500 9150 6500
Wire Wire Line
	11000 6600 9250 6600
Wire Wire Line
	11000 6800 9450 6800
Wire Wire Line
	11000 7600 10250 7600
Text GLabel 8050 6050 1    50   Input ~ 0
I15
Text GLabel 7950 6050 1    50   Input ~ 0
I14
Text GLabel 7850 6050 1    50   Input ~ 0
I13
Text GLabel 7750 6050 1    50   Input ~ 0
I12
Text GLabel 7650 6050 1    50   Input ~ 0
I11
Text GLabel 7550 6050 1    50   Input ~ 0
I10
Text GLabel 7450 6050 1    50   Input ~ 0
I9
Text GLabel 7350 6050 1    50   Input ~ 0
I8
Text GLabel 7250 6050 1    50   Input ~ 0
I7
Text GLabel 7150 6050 1    50   Input ~ 0
I6
Text GLabel 7050 6050 1    50   Input ~ 0
I5
Text GLabel 6950 6050 1    50   Input ~ 0
I4
Text GLabel 6850 6050 1    50   Input ~ 0
I3
Text GLabel 6750 6050 1    50   Input ~ 0
I2
Text GLabel 6650 6050 1    50   Input ~ 0
I1
Text GLabel 6550 6050 1    50   Input ~ 0
I0
Wire Wire Line
	11000 6200 8850 6200
Wire Wire Line
	6550 6050 6550 6200
Connection ~ 6550 6200
Wire Wire Line
	6550 6200 6350 6200
Wire Wire Line
	6650 6050 6650 6300
Connection ~ 6650 6300
Wire Wire Line
	6650 6300 6350 6300
Wire Wire Line
	6750 6050 6750 6400
Connection ~ 6750 6400
Wire Wire Line
	6750 6400 6350 6400
Wire Wire Line
	6850 6050 6850 6500
Connection ~ 6850 6500
Wire Wire Line
	6850 6500 6350 6500
Wire Wire Line
	6950 6050 6950 6600
Connection ~ 6950 6600
Wire Wire Line
	6950 6600 6350 6600
Wire Wire Line
	7050 6050 7050 6700
Connection ~ 7050 6700
Wire Wire Line
	7050 6700 6350 6700
Wire Wire Line
	7150 6050 7150 6800
Connection ~ 7150 6800
Wire Wire Line
	7150 6800 6350 6800
Wire Wire Line
	7250 6050 7250 6900
Wire Wire Line
	6350 6900 7250 6900
Connection ~ 7250 6900
Wire Wire Line
	7250 6900 9550 6900
Wire Wire Line
	7350 6050 7350 7000
Wire Wire Line
	6350 7000 7350 7000
Connection ~ 7350 7000
Wire Wire Line
	7350 7000 9650 7000
Wire Wire Line
	7450 6050 7450 7100
Wire Wire Line
	6350 7100 7450 7100
Connection ~ 7450 7100
Wire Wire Line
	7450 7100 9750 7100
Wire Wire Line
	7550 6050 7550 7200
Wire Wire Line
	6350 7200 7550 7200
Connection ~ 7550 7200
Wire Wire Line
	7650 6050 7650 7300
Wire Wire Line
	6350 7300 7650 7300
Connection ~ 7650 7300
Wire Wire Line
	7650 7300 9950 7300
Wire Wire Line
	7750 6050 7750 7400
Wire Wire Line
	6350 7400 7750 7400
Connection ~ 7750 7400
Wire Wire Line
	7750 7400 10050 7400
Wire Wire Line
	7850 6050 7850 7500
Wire Wire Line
	6350 7500 7850 7500
Connection ~ 7850 7500
Wire Wire Line
	7850 7500 10150 7500
Wire Wire Line
	7950 6050 7950 7600
Connection ~ 7950 7600
Wire Wire Line
	7950 7600 6350 7600
Wire Wire Line
	8050 6050 8050 7700
Wire Wire Line
	6350 7700 8050 7700
Connection ~ 8050 7700
Wire Wire Line
	8050 7700 10350 7700
Text GLabel 10350 6050 1    50   Input ~ 0
O15
Text GLabel 10250 6050 1    50   Input ~ 0
O14
Text GLabel 10150 6050 1    50   Input ~ 0
O13
Text GLabel 10050 6050 1    50   Input ~ 0
O12
Text GLabel 9950 6050 1    50   Input ~ 0
O11
Text GLabel 9850 6050 1    50   Input ~ 0
O10
Text GLabel 9750 6050 1    50   Input ~ 0
O9
Text GLabel 9650 6050 1    50   Input ~ 0
O8
Text GLabel 9550 6050 1    50   Input ~ 0
O7
Text GLabel 9450 6050 1    50   Input ~ 0
O6
Text GLabel 9350 6050 1    50   Input ~ 0
O5
Text GLabel 9250 6050 1    50   Input ~ 0
O4
Text GLabel 9150 6050 1    50   Input ~ 0
O3
Text GLabel 9050 6050 1    50   Input ~ 0
O2
Text GLabel 8950 6050 1    50   Input ~ 0
O1
Text GLabel 8850 6050 1    50   Input ~ 0
O0
Wire Wire Line
	8850 6050 8850 6200
Connection ~ 8850 6200
Wire Wire Line
	8850 6200 6550 6200
Wire Wire Line
	8950 6050 8950 6300
Connection ~ 8950 6300
Wire Wire Line
	8950 6300 6650 6300
Wire Wire Line
	9050 6050 9050 6400
Connection ~ 9050 6400
Wire Wire Line
	9050 6400 6750 6400
Wire Wire Line
	9150 6050 9150 6500
Connection ~ 9150 6500
Wire Wire Line
	9150 6500 6850 6500
Wire Wire Line
	9250 6050 9250 6600
Connection ~ 9250 6600
Wire Wire Line
	9250 6600 6950 6600
Wire Wire Line
	9350 6050 9350 6700
Wire Wire Line
	7050 6700 9350 6700
Connection ~ 9350 6700
Wire Wire Line
	9350 6700 11000 6700
Wire Wire Line
	9450 6050 9450 6800
Connection ~ 9450 6800
Wire Wire Line
	9450 6800 7150 6800
Wire Wire Line
	9550 6050 9550 6900
Connection ~ 9550 6900
Wire Wire Line
	9550 6900 11000 6900
Wire Wire Line
	9650 6050 9650 7000
Connection ~ 9650 7000
Wire Wire Line
	9650 7000 11000 7000
Wire Wire Line
	9750 6050 9750 7100
Connection ~ 9750 7100
Wire Wire Line
	9750 7100 11000 7100
Wire Wire Line
	9850 6050 9850 7200
Wire Wire Line
	7550 7200 9850 7200
Connection ~ 9850 7200
Wire Wire Line
	9850 7200 11000 7200
Wire Wire Line
	9950 6050 9950 7300
Connection ~ 9950 7300
Wire Wire Line
	9950 7300 11000 7300
Wire Wire Line
	10050 6050 10050 7400
Connection ~ 10050 7400
Wire Wire Line
	10050 7400 11000 7400
Wire Wire Line
	10150 6050 10150 7500
Connection ~ 10150 7500
Wire Wire Line
	10150 7500 11000 7500
Wire Wire Line
	10250 6050 10250 7600
Connection ~ 10250 7600
Wire Wire Line
	10250 7600 7950 7600
Wire Wire Line
	10350 6050 10350 7700
Connection ~ 10350 7700
Wire Wire Line
	10350 7700 11000 7700
Text GLabel 2250 3350 2    50   Input ~ 0
I0
Text GLabel 2250 3450 2    50   Input ~ 0
I1
Text GLabel 1750 3350 0    50   Input ~ 0
ADC+
Text GLabel 2250 3750 2    50   Input ~ 0
I2
Text GLabel 2250 3850 2    50   Input ~ 0
I3
Text GLabel 2250 4050 2    50   Input ~ 0
I4
Text GLabel 2250 4150 2    50   Input ~ 0
I5
Text GLabel 2250 4250 2    50   Input ~ 0
I6
Text GLabel 1750 4250 0    50   Input ~ 0
I7
Text GLabel 1750 4150 0    50   Input ~ 0
I8
Text GLabel 1750 4050 0    50   Input ~ 0
I9
Text GLabel 1750 3950 0    50   Input ~ 0
I10
Text GLabel 1750 3850 0    50   Input ~ 0
I11
Text GLabel 1750 3750 0    50   Input ~ 0
I12
Text GLabel 1750 3650 0    50   Input ~ 0
I13
Text GLabel 1750 3550 0    50   Input ~ 0
I14
Text GLabel 4100 2600 0    50   Input ~ 0
I15
Text GLabel 7150 1850 0    50   Input ~ 0
REF0
Text GLabel 4100 2500 0    50   Input ~ 0
REF0
Text GLabel 4600 1900 2    50   Input ~ 0
REF1
Text GLabel 4600 2100 2    50   Input ~ 0
REF2
Text GLabel 4150 3450 0    50   Input ~ 0
O0
Text GLabel 4150 3550 0    50   Input ~ 0
O1
Text GLabel 4150 3650 0    50   Input ~ 0
O2
Text GLabel 4150 3750 0    50   Input ~ 0
O3
Text GLabel 4650 4150 2    50   Input ~ 0
O4
Text GLabel 4650 4050 2    50   Input ~ 0
O5
Text GLabel 4650 3950 2    50   Input ~ 0
O6
Text GLabel 4650 3850 2    50   Input ~ 0
O7
Text GLabel 4650 3750 2    50   Input ~ 0
O8
Text GLabel 4650 3650 2    50   Input ~ 0
O9
Text GLabel 4650 3550 2    50   Input ~ 0
O10
Text GLabel 4650 3450 2    50   Input ~ 0
O11
Text GLabel 4600 2600 2    50   Input ~ 0
O12
Text GLabel 4600 2500 2    50   Input ~ 0
O13
Text GLabel 4600 2400 2    50   Input ~ 0
O14
Text GLabel 4600 2300 2    50   Input ~ 0
O15
Text GLabel 4600 2200 2    50   Input ~ 0
GND
$Comp
L Device:R R1
U 1 1 62782A78
P 7800 1850
F 0 "R1" V 7593 1850 50  0000 C CNN
F 1 "10K" V 7684 1850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7730 1850 50  0001 C CNN
F 3 "~" H 7800 1850 50  0001 C CNN
	1    7800 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 6278AFAF
P 7800 2250
F 0 "R2" V 7593 2250 50  0000 C CNN
F 1 "100K" V 7684 2250 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7730 2250 50  0001 C CNN
F 3 "~" H 7800 2250 50  0001 C CNN
	1    7800 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 2250 7150 2250
$Comp
L Device:R R3
U 1 1 62793AE9
P 7800 2600
F 0 "R3" V 7593 2600 50  0000 C CNN
F 1 "1M" V 7684 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7730 2600 50  0001 C CNN
F 3 "~" H 7800 2600 50  0001 C CNN
	1    7800 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	7950 2600 9500 2600
Wire Wire Line
	7650 2600 7150 2600
$EndSCHEMATC
