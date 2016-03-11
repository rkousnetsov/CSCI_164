# CSCI_164

This repository will contain files associated with the 4-way intersection project for CSCI 164 (Computer Simulation).
All code will be written in the C++ language (C++11 features will be used).

Intersection text map:

		SrcL0	SrcL0	SnkL0	SnkL0				SrcL2	SrcL2	SnkL2	SnkL2
		1 	0						9	8
SnkL1						7	IntL1						15	SrcL3

SnkL1						6	IntL1						14	SrcL3

SrcL1	2						IntL2	10						SnkL3

SrcL1	3						IntL2 	11						SnkL3
				4	5						12	13
		IntL4	IntL4	IntL0	IntL0				IntL6	IntL6	IntL3	IntL3
		17 	16						25	24
SnkL5						23 	IntL5	 					31	SrcL7

SnkL5						22 	IntL5	 					30	SrcL7

SrcL4	18						IntL7	26						SnkL6
						
SrcL4	19						IntL7	27						SnkL6
				20	21						28	29
		SnkL4	SnkL4	SrcL5	SrcL5				SnkL7	SnkL7	SrcL6	SrcL6

Abbrevations:
SrcL# = Source lane number
SnkL# = Sink lane number
IntL# = Intermediate lane number
Numbers without letters: Stoplight number

Input connections (stoplight <- lane)
0, 1 <- SrcL0
2, 3 <- SrcL1
4, 5 <- IntL0
6, 7 <- IntL1
8, 9 <- SrcL2
10, 11 <- IntL2
12, 13 <- IntL3
14, 15 <- SrcL3
16, 17 <- IntL4
18, 19 <- SrcL4
20, 21 <- SrcL5
22, 23 <- IntL5
24, 25 <- IntL6
26, 27 <- IntL7
28, 29 <- SrcL6
30, 31 <- SrcL7

Output connections (stoplight -> lane)
0 -> SnkL0, IntL1		
1 -> IntL4, SnkL1		
2 -> SnkL1, SnkL0		
3 -> IntL2, IntL4		
4 -> IntL4, SnkL1		
5 -> SnkL0, IntL2		
6 -> IntL2, SnkL4		
7 -> SnkL1, SnkL2	
				
8 -> SnkL2, SnkL3		
9 -> IntL6, IntL1		
10 -> IntL1, SnkL2		
11 -> SnkL3, SnkL6		
12 -> IntL6, IntL1		
13 -> SnkL2, SnkL3		
14 -> SnkL3, IntL6		
15 -> IntL1, SnkL2		

16 -> IntL0, IntL7
17 -> SnkL4, SnkL5
18 -> SnkL5, IntL0
19 -> IntL7, SnkL4
20 -> SnkL4, SnkL5
21 -> IntL0, IntL7
22 -> IntL7, SnkL4
23 -> SnkL5, IntL0

24 -> IntL3, SnkL6
25 -> SnkL7, IntL5
26 -> IntL5, IntL3
27 -> SnkL6, SnkL7
28 -> SnkL7, IntL5
29 -> IntL3, SnkL6
30 -> SnkL6, SnkL7
31 -> IntL5, IntL3
