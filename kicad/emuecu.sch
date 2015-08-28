EESchema Schematic File Version 2
LIBS:decent
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
LIBS:special
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
LIBS:arduino
LIBS:mosfets
LIBS:mpx
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "EmuECU"
Date "28 aug 2015"
Rev "0.1"
Comp "Author: Matthew Ridley"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ARDUINO_NANO U1
U 1 1 55D063F1
P 4550 2900
F 0 "U1" H 5950 1000 70  0000 C CNN
F 1 "ARDUINO_NANO" H 6200 900 70  0000 C CNN
F 2 "DIL30" H 5200 950 60  0000 C CNN
F 3 "~" H 5450 1950 60  0000 C CNN
	1    4550 2900
	1    0    0    -1  
$EndComp
$Comp
L IRL540N Q1
U 1 1 55D06733
P 8400 4750
F 0 "Q1" H 8400 4602 40  0000 R CNN
F 1 "IRL540N" H 8400 4899 40  0000 R CNN
F 2 "TO220" H 8221 4851 29  0000 C CNN
F 3 "" H 8400 4750 60  0000 C CNN
	1    8400 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3300 6750 1650
Wire Wire Line
	6750 4800 7400 4800
$Comp
L GND #PWR01
U 1 1 55D07A7F
P 7200 5450
F 0 "#PWR01" H 7200 5450 30  0001 C CNN
F 1 "GND" H 7200 5380 30  0001 C CNN
F 2 "" H 7200 5450 60  0000 C CNN
F 3 "" H 7200 5450 60  0000 C CNN
	1    7200 5450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 55D08290
P 8500 5450
F 0 "#PWR02" H 8500 5450 30  0001 C CNN
F 1 "GND" H 8500 5380 30  0001 C CNN
F 2 "" H 8500 5450 60  0000 C CNN
F 3 "" H 8500 5450 60  0000 C CNN
	1    8500 5450
	1    0    0    -1  
$EndComp
$Comp
L TVS T1
U 1 1 55D085F9
P 8250 5150
F 0 "T1" H 8250 5250 50  0000 C CNN
F 1 "TVS(6KE6CA8)" H 8250 5050 40  0000 C CNN
F 2 "~" H 8250 5150 60  0000 C CNN
F 3 "~" H 8250 5150 60  0000 C CNN
	1    8250 5150
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 55D08B29
P 7600 4800
F 0 "R2" H 7600 4900 40  0000 C CNN
F 1 "1K" H 7600 4700 40  0000 C CNN
F 2 "~" H 7600 4450 60  0000 C CNN
F 3 "~" H 7600 4450 60  0000 C CNN
	1    7600 4800
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 55D08B38
P 7200 5100
F 0 "R1" H 7200 5200 40  0000 C CNN
F 1 "50K" H 7200 5000 40  0000 C CNN
F 2 "~" H 7200 4750 60  0000 C CNN
F 3 "~" H 7200 4750 60  0000 C CNN
	1    7200 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7200 4800 7200 4900
Connection ~ 7200 4800
Wire Wire Line
	7200 5300 7200 5450
$Comp
L +12V #PWR03
U 1 1 55D08C19
P 7300 4000
F 0 "#PWR03" H 7300 3950 20  0001 C CNN
F 1 "+12V" H 7300 4100 30  0000 C CNN
F 2 "" H 7300 4000 60  0000 C CNN
F 3 "" H 7300 4000 60  0000 C CNN
	1    7300 4000
	1    0    0    -1  
$EndComp
$Comp
L ZENER Z1
U 1 1 55D08C79
P 8200 4350
F 0 "Z1" H 8200 4450 50  0000 C CNN
F 1 "1N5365B" H 8200 4250 40  0000 C CNN
F 2 "~" H 8200 4350 60  0000 C CNN
F 3 "~" H 8200 4350 60  0000 C CNN
	1    8200 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 4950 8500 5450
Connection ~ 8500 5150
Wire Wire Line
	8400 4350 9000 4350
Wire Wire Line
	8500 4550 8500 4350
Connection ~ 8500 4350
Wire Wire Line
	8000 4350 7900 4350
Wire Wire Line
	7300 4350 7500 4350
Wire Wire Line
	7300 4000 7300 4350
$Comp
L CONN_2 P1
U 1 1 55D0955B
P 9350 4250
F 0 "P1" V 9300 4250 40  0000 C CNN
F 1 "INJECTOR" V 9400 4250 40  0000 C CNN
F 2 "" H 9350 4250 60  0000 C CNN
F 3 "" H 9350 4250 60  0000 C CNN
	1    9350 4250
	1    0    0    -1  
$EndComp
$Comp
L IRL540N Q2
U 1 1 55D308A1
P 9600 2950
F 0 "Q2" H 9600 2802 40  0000 R CNN
F 1 "IRL540N" H 9600 3099 40  0000 R CNN
F 2 "TO220" H 9421 3051 29  0000 C CNN
F 3 "" H 9600 2950 60  0000 C CNN
	1    9600 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3000 8600 3000
$Comp
L GND #PWR04
U 1 1 55D308AA
P 8400 3650
F 0 "#PWR04" H 8400 3650 30  0001 C CNN
F 1 "GND" H 8400 3580 30  0001 C CNN
F 2 "" H 8400 3650 60  0000 C CNN
F 3 "" H 8400 3650 60  0000 C CNN
	1    8400 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 55D308B0
P 9700 3650
F 0 "#PWR05" H 9700 3650 30  0001 C CNN
F 1 "GND" H 9700 3580 30  0001 C CNN
F 2 "" H 9700 3650 60  0000 C CNN
F 3 "" H 9700 3650 60  0000 C CNN
	1    9700 3650
	1    0    0    -1  
$EndComp
$Comp
L TVS T2
U 1 1 55D308B6
P 9450 3350
F 0 "T2" H 9450 3450 50  0000 C CNN
F 1 "TVS(6KE6CA8)" H 9450 3250 40  0000 C CNN
F 2 "~" H 9450 3350 60  0000 C CNN
F 3 "~" H 9450 3350 60  0000 C CNN
	1    9450 3350
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 55D308BC
P 8800 3000
F 0 "R4" H 8800 3100 40  0000 C CNN
F 1 "1K" H 8800 2900 40  0000 C CNN
F 2 "~" H 8800 2650 60  0000 C CNN
F 3 "~" H 8800 2650 60  0000 C CNN
	1    8800 3000
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 55D308C2
P 8400 3300
F 0 "R3" H 8400 3400 40  0000 C CNN
F 1 "50K" H 8400 3200 40  0000 C CNN
F 2 "~" H 8400 2950 60  0000 C CNN
F 3 "~" H 8400 2950 60  0000 C CNN
	1    8400 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8400 3000 8400 3100
Connection ~ 8400 3000
Wire Wire Line
	8400 3500 8400 3650
$Comp
L +12V #PWR06
U 1 1 55D308CB
P 9000 2200
F 0 "#PWR06" H 9000 2150 20  0001 C CNN
F 1 "+12V" H 9000 2300 30  0000 C CNN
F 2 "" H 9000 2200 60  0000 C CNN
F 3 "" H 9000 2200 60  0000 C CNN
	1    9000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 3150 9700 3650
Connection ~ 9700 3350
Wire Wire Line
	9000 3000 9400 3000
Wire Wire Line
	9600 2550 10150 2550
Wire Wire Line
	9700 2750 9700 2550
Connection ~ 9700 2550
$Comp
L CONN_2 P2
U 1 1 55D308E8
P 10500 2450
F 0 "P2" V 10450 2450 40  0000 C CNN
F 1 "FUEL_PUMP" V 10550 2450 31  0000 C CNN
F 2 "" H 10500 2450 60  0000 C CNN
F 3 "" H 10500 2450 60  0000 C CNN
	1    10500 2450
	1    0    0    -1  
$EndComp
$Comp
L IRL540N Q3
U 1 1 55D308F6
P 8400 1600
F 0 "Q3" H 8400 1452 40  0000 R CNN
F 1 "IRL540N" H 8400 1749 40  0000 R CNN
F 2 "TO220" H 8221 1701 29  0000 C CNN
F 3 "" H 8400 1600 60  0000 C CNN
	1    8400 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 1650 7400 1650
$Comp
L GND #PWR07
U 1 1 55D308FF
P 7200 2300
F 0 "#PWR07" H 7200 2300 30  0001 C CNN
F 1 "GND" H 7200 2230 30  0001 C CNN
F 2 "" H 7200 2300 60  0000 C CNN
F 3 "" H 7200 2300 60  0000 C CNN
	1    7200 2300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 55D30905
P 8500 2300
F 0 "#PWR08" H 8500 2300 30  0001 C CNN
F 1 "GND" H 8500 2230 30  0001 C CNN
F 2 "" H 8500 2300 60  0000 C CNN
F 3 "" H 8500 2300 60  0000 C CNN
	1    8500 2300
	1    0    0    -1  
$EndComp
$Comp
L TVS T3
U 1 1 55D3090B
P 8250 2000
F 0 "T3" H 8250 2100 50  0000 C CNN
F 1 "TVS(6KE6CA8)" H 8250 1900 40  0000 C CNN
F 2 "~" H 8250 2000 60  0000 C CNN
F 3 "~" H 8250 2000 60  0000 C CNN
	1    8250 2000
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 55D30911
P 7600 1650
F 0 "R6" H 7600 1750 40  0000 C CNN
F 1 "1K" H 7600 1550 40  0000 C CNN
F 2 "~" H 7600 1300 60  0000 C CNN
F 3 "~" H 7600 1300 60  0000 C CNN
	1    7600 1650
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 55D30917
P 7200 1950
F 0 "R5" H 7200 2050 40  0000 C CNN
F 1 "50K" H 7200 1850 40  0000 C CNN
F 2 "~" H 7200 1600 60  0000 C CNN
F 3 "~" H 7200 1600 60  0000 C CNN
	1    7200 1950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7200 1650 7200 1750
Connection ~ 7200 1650
Wire Wire Line
	7200 2150 7200 2300
$Comp
L +12V #PWR09
U 1 1 55D30920
P 7800 850
F 0 "#PWR09" H 7800 800 20  0001 C CNN
F 1 "+12V" H 7800 950 30  0000 C CNN
F 2 "" H 7800 850 60  0000 C CNN
F 3 "" H 7800 850 60  0000 C CNN
	1    7800 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 1800 8500 2300
Connection ~ 8500 2000
Wire Wire Line
	8400 1200 8950 1200
Wire Wire Line
	8500 1400 8500 1200
Connection ~ 8500 1200
$Comp
L CONN_2 P3
U 1 1 55D3093D
P 9300 1100
F 0 "P3" V 9250 1100 40  0000 C CNN
F 1 "FAN" V 9350 1100 40  0000 C CNN
F 2 "" H 9300 1100 60  0000 C CNN
F 3 "" H 9300 1100 60  0000 C CNN
	1    9300 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 2200 9000 2550
Wire Wire Line
	9000 2550 9200 2550
Wire Wire Line
	6750 3500 6750 4800
Wire Wire Line
	7800 1200 8000 1200
Wire Wire Line
	7800 850  7800 1200
Wire Wire Line
	6150 3300 6750 3300
Wire Wire Line
	6150 3500 6750 3500
Wire Wire Line
	6150 3600 6650 3600
Wire Wire Line
	6650 3600 6650 6000
Wire Wire Line
	6150 3900 6550 3900
Wire Wire Line
	6550 3900 6550 6350
Wire Wire Line
	6150 4150 6450 4150
Wire Wire Line
	6450 4150 6450 6700
$Comp
L R R7
U 1 1 55D3221D
P 9750 4750
F 0 "R7" H 9750 4850 40  0000 C CNN
F 1 "500" H 9750 4650 40  0000 C CNN
F 2 "~" H 9750 4400 60  0000 C CNN
F 3 "~" H 9750 4400 60  0000 C CNN
	1    9750 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 4950 9850 4950
Wire Wire Line
	9450 5150 9450 5250
$Comp
L GND #PWR010
U 1 1 55D323EC
P 9450 5250
F 0 "#PWR010" H 9450 5250 30  0001 C CNN
F 1 "GND" H 9450 5180 30  0001 C CNN
F 2 "~" H 9450 5250 60  0000 C CNN
F 3 "~" H 9450 5250 60  0000 C CNN
	1    9450 5250
	1    0    0    -1  
$EndComp
Connection ~ 9450 4750
$Comp
L CONN_3 P4
U 1 1 55D32EEE
P 10400 4850
F 0 "P4" V 10350 4850 50  0000 C CNN
F 1 "IGNITION" V 10450 4850 40  0000 C CNN
F 2 "" H 10400 4850 60  0000 C CNN
F 3 "" H 10400 4850 60  0000 C CNN
	1    10400 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 5150 9850 5150
Wire Wire Line
	9850 5150 9850 4950
Wire Wire Line
	6150 3400 7300 3400
Wire Wire Line
	7300 3400 7300 3000
Wire Wire Line
	6150 4450 6250 4450
Wire Wire Line
	6250 4450 6250 6050
Wire Wire Line
	6250 6050 4100 6050
Wire Wire Line
	6150 4350 6350 4350
Wire Wire Line
	4100 7000 6350 7000
Wire Wire Line
	6350 7000 6350 4350
Wire Wire Line
	7800 1650 8200 1650
Wire Wire Line
	8000 2000 8000 1650
Connection ~ 8000 1650
Wire Wire Line
	7800 4800 8200 4800
Wire Wire Line
	8000 5150 8000 4800
Connection ~ 8000 4800
$Comp
L GND #PWR011
U 1 1 55D33FD5
P 5450 5550
F 0 "#PWR011" H 5450 5550 30  0001 C CNN
F 1 "GND" H 5450 5480 30  0001 C CNN
F 2 "~" H 5450 5550 60  0000 C CNN
F 3 "~" H 5450 5550 60  0000 C CNN
	1    5450 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 5400 5450 5550
$Comp
L +5V #PWR012
U 1 1 55D3403F
P 4900 2550
F 0 "#PWR012" H 4900 2640 20  0001 C CNN
F 1 "+5V" H 4900 2640 30  0000 C CNN
F 2 "" H 4900 2550 60  0000 C CNN
F 3 "" H 4900 2550 60  0000 C CNN
	1    4900 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 2550 4900 2700
$Comp
L +12V #PWR013
U 1 1 55D340AA
P 4750 2400
F 0 "#PWR013" H 4750 2350 20  0001 C CNN
F 1 "+12V" H 4750 2500 30  0000 C CNN
F 2 "" H 4750 2400 60  0000 C CNN
F 3 "" H 4750 2400 60  0000 C CNN
	1    4750 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2400 4750 2700
$Comp
L CONN_2 P7
U 1 1 55D34122
P 4950 1400
F 0 "P7" V 4900 1400 40  0000 C CNN
F 1 "POWER" V 5000 1400 40  0000 C CNN
F 2 "" H 4950 1400 60  0000 C CNN
F 3 "" H 4950 1400 60  0000 C CNN
	1    4950 1400
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 55D34131
P 5900 1900
F 0 "#PWR014" H 5900 1900 30  0001 C CNN
F 1 "GND" H 5900 1830 30  0001 C CNN
F 2 "" H 5900 1900 60  0000 C CNN
F 3 "" H 5900 1900 60  0000 C CNN
	1    5900 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1500 5900 1500
Wire Wire Line
	5900 1500 5900 1900
$Comp
L +12V #PWR015
U 1 1 55D341AA
P 5950 1050
F 0 "#PWR015" H 5950 1000 20  0001 C CNN
F 1 "+12V" H 5950 1150 30  0000 C CNN
F 2 "" H 5950 1050 60  0000 C CNN
F 3 "" H 5950 1050 60  0000 C CNN
	1    5950 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1050 5950 1300
$Comp
L R R12
U 1 1 55D343FA
P 3900 6050
F 0 "R12" H 3900 6150 40  0000 C CNN
F 1 "10k" H 3900 5950 40  0000 C CNN
F 2 "~" H 3900 5700 60  0000 C CNN
F 3 "~" H 3900 5700 60  0000 C CNN
	1    3900 6050
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 55D344FE
P 3200 6250
F 0 "C2" H 3200 6350 40  0000 L CNN
F 1 "4.7nF" H 3206 6165 40  0000 L CNN
F 2 "~" H 3238 6100 30  0000 C CNN
F 3 "~" H 3200 6250 60  0000 C CNN
	1    3200 6250
	1    0    0    -1  
$EndComp
$Comp
L ZENER Z2
U 1 1 55D47F24
P 9450 4950
F 0 "Z2" H 9450 5050 50  0000 C CNN
F 1 "1N4733" H 9450 4850 40  0000 C CNN
F 2 "~" H 9450 4950 60  0000 C CNN
F 3 "~" H 9450 4950 60  0000 C CNN
	1    9450 4950
	0    -1   -1   0   
$EndComp
$Comp
L ZENER Z4
U 1 1 55D48057
P 3500 6250
F 0 "Z4" H 3500 6350 50  0000 C CNN
F 1 "1N4733" H 3500 6150 40  0000 C CNN
F 2 "~" H 3500 6250 60  0000 C CNN
F 3 "~" H 3500 6250 60  0000 C CNN
	1    3500 6250
	0    -1   -1   0   
$EndComp
$Comp
L CONN_3 P9
U 1 1 55D480F6
P 1250 6150
F 0 "P9" V 1200 6150 50  0000 C CNN
F 1 "HALL" V 1300 6150 40  0000 C CNN
F 2 "" H 1250 6150 60  0000 C CNN
F 3 "" H 1250 6150 60  0000 C CNN
	1    1250 6150
	-1   0    0    -1  
$EndComp
$Comp
L +5V #PWR016
U 1 1 55D481F0
P 1800 5450
F 0 "#PWR016" H 1800 5540 20  0001 C CNN
F 1 "+5V" H 1800 5540 30  0000 C CNN
F 2 "" H 1800 5450 60  0000 C CNN
F 3 "" H 1800 5450 60  0000 C CNN
	1    1800 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 5450 1800 6150
Wire Wire Line
	1800 6150 1600 6150
Wire Wire Line
	1600 6050 2100 6050
Wire Wire Line
	3150 6050 3700 6050
Wire Wire Line
	1600 6250 2000 6250
Wire Wire Line
	2000 6250 2000 6450
Wire Wire Line
	2000 6450 3500 6450
Connection ~ 3200 6450
$Comp
L R R13
U 1 1 55D48649
P 2000 5850
F 0 "R13" H 2000 5950 40  0000 C CNN
F 1 "10K" H 2000 5750 40  0000 C CNN
F 2 "~" H 2000 5500 60  0000 C CNN
F 3 "~" H 2000 5500 60  0000 C CNN
	1    2000 5850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2000 5650 1800 5650
Connection ~ 1800 5650
Connection ~ 2000 6050
Connection ~ 3200 6050
Connection ~ 3500 6050
$Comp
L R R23
U 1 1 55D4921F
P 3900 7000
F 0 "R23" H 3900 7100 40  0000 C CNN
F 1 "10k" H 3900 6900 40  0000 C CNN
F 2 "~" H 3900 6650 60  0000 C CNN
F 3 "~" H 3900 6650 60  0000 C CNN
	1    3900 7000
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 55D49225
P 3200 7200
F 0 "C1" H 3200 7300 40  0000 L CNN
F 1 "1nF" H 3206 7115 40  0000 L CNN
F 2 "~" H 3238 7050 30  0000 C CNN
F 3 "~" H 3200 7200 60  0000 C CNN
	1    3200 7200
	1    0    0    -1  
$EndComp
$Comp
L ZENER Z3
U 1 1 55D4922B
P 3500 7200
F 0 "Z3" H 3500 7300 50  0000 C CNN
F 1 "1N4733" H 3500 7100 40  0000 C CNN
F 2 "~" H 3500 7200 60  0000 C CNN
F 3 "~" H 3500 7200 60  0000 C CNN
	1    3500 7200
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR017
U 1 1 55D49231
P 3500 7550
F 0 "#PWR017" H 3500 7550 30  0001 C CNN
F 1 "GND" H 3500 7480 30  0001 C CNN
F 2 "~" H 3500 7550 60  0000 C CNN
F 3 "~" H 3500 7550 60  0000 C CNN
	1    3500 7550
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 P8
U 1 1 55D49237
P 1250 7100
F 0 "P8" V 1200 7100 50  0000 C CNN
F 1 "SERVO" V 1300 7100 40  0000 C CNN
F 2 "" H 1250 7100 60  0000 C CNN
F 3 "" H 1250 7100 60  0000 C CNN
	1    1250 7100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1600 7000 2000 7000
Wire Wire Line
	3100 7000 3700 7000
Wire Wire Line
	1600 7200 2000 7200
Wire Wire Line
	2000 7400 3500 7400
Connection ~ 3200 7400
Connection ~ 3200 7000
Connection ~ 3500 7000
$Comp
L GND #PWR018
U 1 1 55D4926F
P 3500 6600
F 0 "#PWR018" H 3500 6600 30  0001 C CNN
F 1 "GND" H 3500 6530 30  0001 C CNN
F 2 "~" H 3500 6600 60  0000 C CNN
F 3 "~" H 3500 6600 60  0000 C CNN
	1    3500 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 6450 3500 6600
Wire Wire Line
	2000 7200 2000 7400
Wire Wire Line
	3500 7400 3500 7550
$Comp
L CONN_3 P5
U 1 1 55D49603
P 10400 5650
F 0 "P5" V 10350 5650 50  0000 C CNN
F 1 "THROTTLE" V 10450 5650 40  0000 C CNN
F 2 "" H 10400 5650 60  0000 C CNN
F 3 "" H 10400 5650 60  0000 C CNN
	1    10400 5650
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 P6
U 1 1 55D49612
P 10500 6450
F 0 "P6" V 10450 6450 50  0000 C CNN
F 1 "LOAD" V 10550 6450 40  0000 C CNN
F 2 "" H 10500 6450 60  0000 C CNN
F 3 "" H 10500 6450 60  0000 C CNN
	1    10500 6450
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 55D71BAE
P 2300 6050
F 0 "R11" H 2300 6150 40  0000 C CNN
F 1 "300" H 2300 5950 40  0000 C CNN
F 2 "~" H 2300 5700 60  0000 C CNN
F 3 "~" H 2300 5700 60  0000 C CNN
	1    2300 6050
	1    0    0    -1  
$EndComp
$Comp
L R R24
U 1 1 55D71BBD
P 2200 7000
F 0 "R24" H 2200 7100 40  0000 C CNN
F 1 "150" H 2200 6900 40  0000 C CNN
F 2 "~" H 2200 6650 60  0000 C CNN
F 3 "~" H 2200 6650 60  0000 C CNN
	1    2200 7000
	1    0    0    -1  
$EndComp
$Comp
L R R20
U 1 1 55D72485
P 3150 1500
F 0 "R20" H 3150 1600 40  0000 C CNN
F 1 "10k" H 3150 1400 40  0000 C CNN
F 2 "~" H 3150 1150 60  0000 C CNN
F 3 "~" H 3150 1150 60  0000 C CNN
	1    3150 1500
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 55D7248B
P 2450 1700
F 0 "C6" H 2450 1800 40  0000 L CNN
F 1 "0.1uF" H 2456 1615 40  0000 L CNN
F 2 "~" H 2488 1550 30  0000 C CNN
F 3 "~" H 2450 1700 60  0000 C CNN
	1    2450 1700
	1    0    0    -1  
$EndComp
$Comp
L ZENER Z7
U 1 1 55D72491
P 2750 1700
F 0 "Z7" H 2750 1800 50  0000 C CNN
F 1 "1N4733" H 2750 1600 40  0000 C CNN
F 2 "~" H 2750 1700 60  0000 C CNN
F 3 "~" H 2750 1700 60  0000 C CNN
	1    2750 1700
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR019
U 1 1 55D7249D
P 1650 900
F 0 "#PWR019" H 1650 990 20  0001 C CNN
F 1 "+5V" H 1650 990 30  0000 C CNN
F 2 "" H 1650 900 60  0000 C CNN
F 3 "" H 1650 900 60  0000 C CNN
	1    1650 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 900  1650 1100
Wire Wire Line
	1650 1700 1250 1700
Wire Wire Line
	1250 1500 1950 1500
Wire Wire Line
	2350 1500 2950 1500
Wire Wire Line
	1650 1700 1650 1900
Wire Wire Line
	1650 1900 2750 1900
Connection ~ 2450 1900
$Comp
L R R22
U 1 1 55D724B0
P 1650 1300
F 0 "R22" H 1650 1400 40  0000 C CNN
F 1 "20K" H 1650 1200 40  0000 C CNN
F 2 "~" H 1650 950 60  0000 C CNN
F 3 "~" H 1650 950 60  0000 C CNN
	1    1650 1300
	0    -1   -1   0   
$EndComp
Connection ~ 1650 1500
Connection ~ 2450 1500
Connection ~ 2750 1500
$Comp
L GND #PWR020
U 1 1 55D724BB
P 2750 2050
F 0 "#PWR020" H 2750 2050 30  0001 C CNN
F 1 "GND" H 2750 1980 30  0001 C CNN
F 2 "~" H 2750 2050 60  0000 C CNN
F 3 "~" H 2750 2050 60  0000 C CNN
	1    2750 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 1900 2750 2050
Wire Wire Line
	3350 1500 4100 1500
$Comp
L R R21
U 1 1 55D724C3
P 2150 1500
F 0 "R21" H 2150 1600 40  0000 C CNN
F 1 "300" H 2150 1400 40  0000 C CNN
F 2 "~" H 2150 1150 60  0000 C CNN
F 3 "~" H 2150 1150 60  0000 C CNN
	1    2150 1500
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P13
U 1 1 55D724CB
P 900 1600
F 0 "P13" V 850 1600 40  0000 C CNN
F 1 "ENGINE_TEMP" V 950 1600 30  0000 C CNN
F 2 "" H 900 1600 60  0000 C CNN
F 3 "" H 900 1600 60  0000 C CNN
	1    900  1600
	-1   0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 55D72F53
P 3150 2850
F 0 "R17" H 3150 2950 40  0000 C CNN
F 1 "10k" H 3150 2750 40  0000 C CNN
F 2 "~" H 3150 2500 60  0000 C CNN
F 3 "~" H 3150 2500 60  0000 C CNN
	1    3150 2850
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 55D72F59
P 2450 3050
F 0 "C5" H 2450 3150 40  0000 L CNN
F 1 "0.1uF" H 2456 2965 40  0000 L CNN
F 2 "~" H 2488 2900 30  0000 C CNN
F 3 "~" H 2450 3050 60  0000 C CNN
	1    2450 3050
	1    0    0    -1  
$EndComp
$Comp
L ZENER Z6
U 1 1 55D72F5F
P 2750 3050
F 0 "Z6" H 2750 3150 50  0000 C CNN
F 1 "1N4733" H 2750 2950 40  0000 C CNN
F 2 "~" H 2750 3050 60  0000 C CNN
F 3 "~" H 2750 3050 60  0000 C CNN
	1    2750 3050
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR021
U 1 1 55D72F65
P 1650 2250
F 0 "#PWR021" H 1650 2340 20  0001 C CNN
F 1 "+5V" H 1650 2340 30  0000 C CNN
F 2 "" H 1650 2250 60  0000 C CNN
F 3 "" H 1650 2250 60  0000 C CNN
	1    1650 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2250 1650 2450
Wire Wire Line
	1650 3050 1250 3050
Wire Wire Line
	1250 2850 1950 2850
Wire Wire Line
	2350 2850 2950 2850
Wire Wire Line
	1650 3050 1650 3250
Wire Wire Line
	1650 3250 2750 3250
Connection ~ 2450 3250
$Comp
L R R19
U 1 1 55D72F76
P 1650 2650
F 0 "R19" H 1650 2750 40  0000 C CNN
F 1 "20K" H 1650 2550 40  0000 C CNN
F 2 "~" H 1650 2300 60  0000 C CNN
F 3 "~" H 1650 2300 60  0000 C CNN
	1    1650 2650
	0    -1   -1   0   
$EndComp
Connection ~ 1650 2850
Connection ~ 2450 2850
Connection ~ 2750 2850
$Comp
L GND #PWR022
U 1 1 55D72F7F
P 2750 3400
F 0 "#PWR022" H 2750 3400 30  0001 C CNN
F 1 "GND" H 2750 3330 30  0001 C CNN
F 2 "~" H 2750 3400 60  0000 C CNN
F 3 "~" H 2750 3400 60  0000 C CNN
	1    2750 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3250 2750 3400
Wire Wire Line
	3350 2850 3950 2850
$Comp
L R R18
U 1 1 55D72F87
P 2150 2850
F 0 "R18" H 2150 2950 40  0000 C CNN
F 1 "300" H 2150 2750 40  0000 C CNN
F 2 "~" H 2150 2500 60  0000 C CNN
F 3 "~" H 2150 2500 60  0000 C CNN
	1    2150 2850
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P12
U 1 1 55D72F8D
P 900 2950
F 0 "P12" V 850 2950 40  0000 C CNN
F 1 "AIR_TEMP" V 950 2950 40  0000 C CNN
F 2 "" H 900 2950 60  0000 C CNN
F 3 "" H 900 2950 60  0000 C CNN
	1    900  2950
	-1   0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 55D72F93
P 3150 3700
F 0 "R16" H 3150 3800 40  0000 C CNN
F 1 "10k" H 3150 3600 40  0000 C CNN
F 2 "~" H 3150 3350 60  0000 C CNN
F 3 "~" H 3150 3350 60  0000 C CNN
	1    3150 3700
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 55D72F99
P 2450 3900
F 0 "C4" H 2450 4000 40  0000 L CNN
F 1 "470pF" H 2456 3815 40  0000 L CNN
F 2 "~" H 2488 3750 30  0000 C CNN
F 3 "~" H 2450 3900 60  0000 C CNN
	1    2450 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3700 2950 3700
Wire Wire Line
	1900 4100 2750 4100
Connection ~ 2450 4100
Connection ~ 2450 3700
$Comp
L GND #PWR023
U 1 1 55D72FBF
P 2750 4250
F 0 "#PWR023" H 2750 4250 30  0001 C CNN
F 1 "GND" H 2750 4180 30  0001 C CNN
F 2 "~" H 2750 4250 60  0000 C CNN
F 3 "~" H 2750 4250 60  0000 C CNN
	1    2750 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 4100 2750 4250
Wire Wire Line
	3350 3700 3850 3700
$Comp
L R R14
U 1 1 55D730B3
P 3150 4550
F 0 "R14" H 3150 4650 40  0000 C CNN
F 1 "10k" H 3150 4450 40  0000 C CNN
F 2 "~" H 3150 4200 60  0000 C CNN
F 3 "~" H 3150 4200 60  0000 C CNN
	1    3150 4550
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 55D730B9
P 2450 4750
F 0 "C3" H 2450 4850 40  0000 L CNN
F 1 "47nf" H 2456 4665 40  0000 L CNN
F 2 "~" H 2488 4600 30  0000 C CNN
F 3 "~" H 2450 4750 60  0000 C CNN
	1    2450 4750
	1    0    0    -1  
$EndComp
$Comp
L ZENER Z5
U 1 1 55D730BF
P 2750 4750
F 0 "Z5" H 2750 4850 50  0000 C CNN
F 1 "1N4733" H 2750 4650 40  0000 C CNN
F 2 "~" H 2750 4750 60  0000 C CNN
F 3 "~" H 2750 4750 60  0000 C CNN
	1    2750 4750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1650 4750 1250 4750
Wire Wire Line
	1250 4550 1950 4550
Wire Wire Line
	2350 4550 2950 4550
Wire Wire Line
	1650 4750 1650 4950
Wire Wire Line
	1650 4950 2750 4950
Connection ~ 2450 4950
Connection ~ 1650 4550
Connection ~ 2450 4550
Connection ~ 2750 4550
$Comp
L GND #PWR024
U 1 1 55D730D2
P 2750 5100
F 0 "#PWR024" H 2750 5100 30  0001 C CNN
F 1 "GND" H 2750 5030 30  0001 C CNN
F 2 "~" H 2750 5100 60  0000 C CNN
F 3 "~" H 2750 5100 60  0000 C CNN
	1    2750 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 4950 2750 5100
Wire Wire Line
	3350 4550 3950 4550
$Comp
L R R15
U 1 1 55D730DA
P 2150 4550
F 0 "R15" H 2150 4650 40  0000 C CNN
F 1 "300" H 2150 4450 40  0000 C CNN
F 2 "~" H 2150 4200 60  0000 C CNN
F 3 "~" H 2150 4200 60  0000 C CNN
	1    2150 4550
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P10
U 1 1 55D730E0
P 900 4650
F 0 "P10" V 850 4650 40  0000 C CNN
F 1 "FUEL_P" V 950 4650 40  0000 C CNN
F 2 "" H 900 4650 60  0000 C CNN
F 3 "" H 900 4650 60  0000 C CNN
	1    900  4650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4250 3650 4100 3650
Wire Wire Line
	4100 3650 4100 1500
Wire Wire Line
	3950 2850 3950 3750
Wire Wire Line
	3950 3750 4250 3750
Wire Wire Line
	4250 3850 3850 3850
Wire Wire Line
	3850 3850 3850 3700
Wire Wire Line
	4250 3950 3950 3950
Wire Wire Line
	3950 3950 3950 4550
$Comp
L MPX4250AP U2
U 1 1 55D73772
P 1250 4300
F 0 "U2" H 950 5000 60  0000 C CNN
F 1 "MPX4250/4115AP" V 800 4650 47  0000 C CNN
F 2 "~" H 1200 4650 60  0000 C CNN
F 3 "~" H 1200 4650 60  0000 C CNN
	1    1250 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3800 1900 3800
Wire Wire Line
	1900 3800 1900 4100
$Comp
L +5V #PWR025
U 1 1 55D73ACC
P 1700 3500
F 0 "#PWR025" H 1700 3590 20  0001 C CNN
F 1 "+5V" H 1700 3590 30  0000 C CNN
F 2 "" H 1700 3500 60  0000 C CNN
F 3 "" H 1700 3500 60  0000 C CNN
	1    1700 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 3500 1700 3900
Wire Wire Line
	1700 3900 1550 3900
$Comp
L DIODESCH D4
U 1 1 55D85022
P 9150 4950
F 0 "D4" H 9150 5050 40  0000 C CNN
F 1 "BAT48" H 9150 4850 40  0000 C CNN
F 2 "~" H 9150 4950 60  0000 C CNN
F 3 "~" H 9150 4950 60  0000 C CNN
	1    9150 4950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8750 4750 9550 4750
Connection ~ 9150 4750
Connection ~ 9450 5150
$Comp
L DIODE D1
U 1 1 55D85451
P 7700 4350
F 0 "D1" H 7700 4450 40  0000 C CNN
F 1 "FR304" H 7700 4250 40  0000 C CNN
F 2 "~" H 7700 4350 60  0000 C CNN
F 3 "~" H 7700 4350 60  0000 C CNN
	1    7700 4350
	-1   0    0    1   
$EndComp
$Comp
L DIODE D2
U 1 1 55D8546B
P 9400 2550
F 0 "D2" H 9400 2650 40  0000 C CNN
F 1 "FR304" H 9400 2450 40  0000 C CNN
F 2 "~" H 9400 2550 60  0000 C CNN
F 3 "~" H 9400 2550 60  0000 C CNN
	1    9400 2550
	-1   0    0    1   
$EndComp
$Comp
L DIODE D3
U 1 1 55D8547A
P 8200 1200
F 0 "D3" H 8200 1300 40  0000 C CNN
F 1 "FR304" H 8200 1100 40  0000 C CNN
F 2 "~" H 8200 1200 60  0000 C CNN
F 3 "~" H 8200 1200 60  0000 C CNN
	1    8200 1200
	-1   0    0    1   
$EndComp
Wire Wire Line
	9950 4750 10050 4750
$Comp
L INDUCTOR L1
U 1 1 55D95555
P 2850 6050
F 0 "L1" V 2800 6050 40  0000 C CNN
F 1 "100uH" V 2950 6050 40  0000 C CNN
F 2 "~" H 2850 6050 60  0000 C CNN
F 3 "~" H 2850 6050 60  0000 C CNN
	1    2850 6050
	0    1    1    0   
$EndComp
Wire Wire Line
	2500 6050 2550 6050
$Comp
L C C7
U 1 1 55D96043
P 6150 1550
F 0 "C7" H 6150 1650 40  0000 L CNN
F 1 "10uF" H 6156 1465 40  0000 L CNN
F 2 "~" H 6188 1400 30  0000 C CNN
F 3 "~" H 6150 1550 60  0000 C CNN
	1    6150 1550
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 55D9636D
P 9850 6350
F 0 "R9" H 9850 6450 40  0000 C CNN
F 1 "500" H 9850 6250 40  0000 C CNN
F 2 "~" H 9850 6000 60  0000 C CNN
F 3 "~" H 9850 6000 60  0000 C CNN
	1    9850 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 6550 9950 6550
Wire Wire Line
	9550 6750 9550 6850
$Comp
L GND #PWR026
U 1 1 55D96375
P 9550 6850
F 0 "#PWR026" H 9550 6850 30  0001 C CNN
F 1 "GND" H 9550 6780 30  0001 C CNN
F 2 "~" H 9550 6850 60  0000 C CNN
F 3 "~" H 9550 6850 60  0000 C CNN
	1    9550 6850
	1    0    0    -1  
$EndComp
Connection ~ 9550 6350
Wire Wire Line
	9250 6750 9950 6750
Wire Wire Line
	9950 6750 9950 6550
$Comp
L ZENER Z9
U 1 1 55D9637F
P 9550 6550
F 0 "Z9" H 9550 6650 50  0000 C CNN
F 1 "1N4733" H 9550 6450 40  0000 C CNN
F 2 "~" H 9550 6550 60  0000 C CNN
F 3 "~" H 9550 6550 60  0000 C CNN
	1    9550 6550
	0    -1   -1   0   
$EndComp
$Comp
L DIODESCH D6
U 1 1 55D96385
P 9250 6550
F 0 "D6" H 9250 6650 40  0000 C CNN
F 1 "BAT48" H 9250 6450 40  0000 C CNN
F 2 "~" H 9250 6550 60  0000 C CNN
F 3 "~" H 9250 6550 60  0000 C CNN
	1    9250 6550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9050 6350 9650 6350
Connection ~ 9250 6350
Connection ~ 9550 6750
Wire Wire Line
	10050 6350 10150 6350
$Comp
L R R8
U 1 1 55D96391
P 9750 5550
F 0 "R8" H 9750 5650 40  0000 C CNN
F 1 "500" H 9750 5450 40  0000 C CNN
F 2 "~" H 9750 5200 60  0000 C CNN
F 3 "~" H 9750 5200 60  0000 C CNN
	1    9750 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 5750 9850 5750
Wire Wire Line
	9450 5950 9450 6050
$Comp
L GND #PWR027
U 1 1 55D96399
P 9450 6050
F 0 "#PWR027" H 9450 6050 30  0001 C CNN
F 1 "GND" H 9450 5980 30  0001 C CNN
F 2 "~" H 9450 6050 60  0000 C CNN
F 3 "~" H 9450 6050 60  0000 C CNN
	1    9450 6050
	1    0    0    -1  
$EndComp
Connection ~ 9450 5550
Wire Wire Line
	9150 5950 9850 5950
Wire Wire Line
	9850 5950 9850 5750
$Comp
L ZENER Z8
U 1 1 55D963A3
P 9450 5750
F 0 "Z8" H 9450 5850 50  0000 C CNN
F 1 "1N4733" H 9450 5650 40  0000 C CNN
F 2 "~" H 9450 5750 60  0000 C CNN
F 3 "~" H 9450 5750 60  0000 C CNN
	1    9450 5750
	0    -1   -1   0   
$EndComp
$Comp
L DIODESCH D5
U 1 1 55D963A9
P 9150 5750
F 0 "D5" H 9150 5850 40  0000 C CNN
F 1 "BAT48" H 9150 5650 40  0000 C CNN
F 2 "~" H 9150 5750 60  0000 C CNN
F 3 "~" H 9150 5750 60  0000 C CNN
	1    9150 5750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8950 5550 9550 5550
Connection ~ 9150 5550
Connection ~ 9450 5950
Wire Wire Line
	9950 5550 10050 5550
Wire Wire Line
	8750 6000 8750 4750
Wire Wire Line
	8950 6350 8950 5550
Wire Wire Line
	9050 6700 9050 6350
$Comp
L PWR_FLAG #FLG028
U 1 1 55D9AE62
P 5800 1300
F 0 "#FLG028" H 5800 1395 30  0001 C CNN
F 1 "PWR_FLAG" H 5800 1480 30  0000 C CNN
F 2 "" H 5800 1300 60  0000 C CNN
F 3 "" H 5800 1300 60  0000 C CNN
	1    5800 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 1000 7800 1000
Connection ~ 7800 1000
$Comp
L PWR_FLAG #FLG029
U 1 1 55D9B427
P 5300 1500
F 0 "#FLG029" H 5300 1595 30  0001 C CNN
F 1 "PWR_FLAG" H 5300 1680 30  0000 C CNN
F 2 "" H 5300 1500 60  0000 C CNN
F 3 "" H 5300 1500 60  0000 C CNN
	1    5300 1500
	-1   0    0    1   
$EndComp
Wire Wire Line
	10150 2350 9000 2350
Connection ~ 9000 2350
Wire Wire Line
	9000 4150 7300 4150
Connection ~ 7300 4150
Wire Wire Line
	9200 3350 9200 3000
Connection ~ 9200 3000
$Comp
L TVS T4
U 1 1 55D9B7ED
P 6450 1550
F 0 "T4" H 6450 1650 50  0000 C CNN
F 1 "TVS(1.5KE16CA)" H 6450 1450 40  0000 C CNN
F 2 "~" H 6450 1550 60  0000 C CNN
F 3 "~" H 6450 1550 60  0000 C CNN
	1    6450 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5800 1300 6450 1300
$Comp
L DIODESCH D7
U 1 1 55DC6B3F
P 5600 1300
F 0 "D7" H 5600 1400 40  0000 C CNN
F 1 "1N5822" H 5600 1200 40  0000 C CNN
F 2 "~" H 5600 1300 60  0000 C CNN
F 3 "~" H 5600 1300 60  0000 C CNN
	1    5600 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1300 5400 1300
Connection ~ 5950 1300
Wire Wire Line
	6150 1300 6150 1350
Connection ~ 6150 1300
Wire Wire Line
	5900 1800 6450 1800
Wire Wire Line
	6150 1800 6150 1750
Connection ~ 6150 1800
Connection ~ 5900 1800
$Comp
L R R27
U 1 1 55DC8388
P 7450 6000
F 0 "R27" H 7450 6100 40  0000 C CNN
F 1 "150" H 7450 5900 40  0000 C CNN
F 2 "~" H 7450 5650 60  0000 C CNN
F 3 "~" H 7450 5650 60  0000 C CNN
	1    7450 6000
	1    0    0    -1  
$EndComp
$Comp
L R R26
U 1 1 55DC8397
P 7650 6350
F 0 "R26" H 7650 6450 40  0000 C CNN
F 1 "150" H 7650 6250 40  0000 C CNN
F 2 "~" H 7650 6000 60  0000 C CNN
F 3 "~" H 7650 6000 60  0000 C CNN
	1    7650 6350
	1    0    0    -1  
$EndComp
$Comp
L R R25
U 1 1 55DC83A6
P 7850 6700
F 0 "R25" H 7850 6800 40  0000 C CNN
F 1 "150" H 7850 6600 40  0000 C CNN
F 2 "~" H 7850 6350 60  0000 C CNN
F 3 "~" H 7850 6350 60  0000 C CNN
	1    7850 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 6000 7250 6000
Wire Wire Line
	7650 6000 8750 6000
Wire Wire Line
	6550 6350 7450 6350
Wire Wire Line
	8950 6350 7850 6350
Wire Wire Line
	6450 6700 7650 6700
Wire Wire Line
	8050 6700 9050 6700
Text Notes 4200 7350 0    60   ~ 0
This work is licensed under a Creative Commons\nAttribution-ShareAlike 4.0 International License.\n\nhttp://creativecommons.org/licenses/by-sa/4.0/\n
$Comp
L INDUCTOR L2
U 1 1 55E04FB3
P 2800 7000
F 0 "L2" V 2750 7000 40  0000 C CNN
F 1 "10uH" V 2900 7000 40  0000 C CNN
F 2 "~" H 2800 7000 60  0000 C CNN
F 3 "~" H 2800 7000 60  0000 C CNN
	1    2800 7000
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 7000 2500 7000
$EndSCHEMATC
