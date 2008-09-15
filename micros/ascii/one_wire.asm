
;CodeVisionAVR C Compiler V1.25.5 Professional
;(C) Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega16
;Program type           : Application
;Clock frequency        : 4.000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External SRAM size     : 0
;Data Stack size        : 256 byte(s)
;Heap size              : 0 byte(s)
;Promote char to int    : No
;char is unsigned       : Yes
;8 bit enums            : Yes
;Word align FLASH struct: No
;Enhanced core instructions    : On
;Smart register allocation : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega16
	#pragma AVRPART MEMORY PROG_FLASH 16384
	#pragma AVRPART MEMORY EEPROM 512
	#pragma AVRPART MEMORY INT_SRAM SIZE 1024
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM
	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM
	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM
	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM
	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM
	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM
	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ANDI R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ORI  R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __CLRD1S
	LDI  R30,0
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+@1)
	LDI  R31,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	LDI  R22,BYTE3(2*@0+@1)
	LDI  R23,BYTE4(2*@0+@1)
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+@2)
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+@3)
	LDI  R@1,HIGH(@2+@3)
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+@3)
	LDI  R@1,HIGH(@2*2+@3)
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+@1
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+@1
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	LDS  R22,@0+@1+2
	LDS  R23,@0+@1+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+@2
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+@3
	LDS  R@1,@2+@3+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+@1
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	LDS  R24,@0+@1+2
	LDS  R25,@0+@1+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+@1,R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	STS  @0+@1+2,R22
	STS  @0+@1+3,R23
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+@1,R0
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+@1,R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+@1,R@2
	STS  @0+@1+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __CLRD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R@1
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

	.CSEG
	.ORG 0

	.INCLUDE "one_wire.vec"
	.INCLUDE "one_wire.inc"

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,13
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(0x400)
	LDI  R25,HIGH(0x400)
	LDI  R26,0x60
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;STACK POINTER INITIALIZATION
	LDI  R30,LOW(0x45F)
	OUT  SPL,R30
	LDI  R30,HIGH(0x45F)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(0x160)
	LDI  R29,HIGH(0x160)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x160
;       1 /*****************************************************
;       2 This program was produced by the
;       3 CodeWizardAVR V1.25.5 Professional
;       4 Automatic Program Generator
;       5 © Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
;       6 http://www.hpinfotech.com
;       7 
;       8 Project :
;       9 Version :
;      10 Date    : 15/07/2008
;      11 Author  : Pato
;      12 Company : Pato
;      13 Comments:
;      14 
;      15 
;      16 Chip type           : ATmega16
;      17 Program type        : Application
;      18 Clock frequency     : 4.000000 MHz
;      19 Memory model        : Small
;      20 External SRAM size  : 0
;      21 Data Stack size     : 256
;      22 *****************************************************/
;      23 
;      24 #include <mega16.h>
;      25 	#ifndef __SLEEP_DEFINED__
	#ifndef __SLEEP_DEFINED__
;      26 	#define __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
;      27 	.EQU __se_bit=0x40
	.EQU __se_bit=0x40
;      28 	.EQU __sm_mask=0xB0
	.EQU __sm_mask=0xB0
;      29 	.EQU __sm_powerdown=0x20
	.EQU __sm_powerdown=0x20
;      30 	.EQU __sm_powersave=0x30
	.EQU __sm_powersave=0x30
;      31 	.EQU __sm_standby=0xA0
	.EQU __sm_standby=0xA0
;      32 	.EQU __sm_ext_standby=0xB0
	.EQU __sm_ext_standby=0xB0
;      33 	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_adc_noise_red=0x10
;      34 	.SET power_ctrl_reg=mcucr
	.SET power_ctrl_reg=mcucr
;      35 	#endif
	#endif
;      36 #include <string.h>
;      37 
;      38 // 1 Wire Bus functions
;      39 #asm
;      40    .equ __w1_port=0x1B ;PORTA
   .equ __w1_port=0x1B ;PORTA
;      41    .equ __w1_bit=0
   .equ __w1_bit=0
;      42 #endasm
;      43 #include <1wire.h>
;      44 
;      45 // DS1820 Temperature Sensor functions
;      46 #include <ds1820.h>
;      47 
;      48 #define RXB8 1
;      49 #define TXB8 0
;      50 #define UPE 2
;      51 #define OVR 3
;      52 #define FE 4
;      53 #define UDRE 5
;      54 #define RXC 7
;      55 
;      56 #define FRAMING_ERROR (1<<FE)
;      57 #define PARITY_ERROR (1<<UPE)
;      58 #define DATA_OVERRUN (1<<OVR)
;      59 #define DATA_REGISTER_EMPTY (1<<UDRE)
;      60 #define RX_COMPLETE (1<<RXC)
;      61 
;      62 // USART Receiver buffer
;      63 #define MAX_DEVICES 30
;      64 #define RX_BUFFER_SIZE 8
;      65 char rx_buffer[RX_BUFFER_SIZE];
_rx_buffer:
	.BYTE 0x8
;      66 
;      67 #if RX_BUFFER_SIZE<256
;      68 unsigned char rx_wr_index,rx_rd_index,rx_counter;
;      69 #else
;      70 unsigned int rx_wr_index,rx_rd_index,rx_counter;
;      71 #endif
;      72 
;      73 // This flag is set on USART Receiver buffer overflow
;      74 bit rx_buffer_overflow;
;      75 
;      76 // USART Receiver interrupt service routine
;      77 interrupt [USART_RXC] void usart_rx_isr(void)
;      78 {

	.CSEG
_usart_rx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;      79 char status,data;
;      80 status=UCSRA;
	ST   -Y,R17
	ST   -Y,R16
;	status -> R17
;	data -> R16
	IN   R17,11
;      81 data=UDR;
	IN   R16,12
;      82 if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	MOV  R30,R17
	ANDI R30,LOW(0x1C)
	BRNE _0x3
;      83    {
;      84    rx_buffer[rx_wr_index]=data;
	MOV  R30,R5
	LDI  R31,0
	SUBI R30,LOW(-_rx_buffer)
	SBCI R31,HIGH(-_rx_buffer)
	ST   Z,R16
;      85    if (++rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
	INC  R5
	LDI  R30,LOW(8)
	CP   R30,R5
	BRNE _0x4
	CLR  R5
;      86    if (++rx_counter == RX_BUFFER_SIZE)
_0x4:
	INC  R7
	LDI  R30,LOW(8)
	CP   R30,R7
	BRNE _0x5
;      87       {
;      88       rx_counter=0;
	CLR  R7
;      89       rx_buffer_overflow=1;
	SET
	BLD  R2,0
;      90       };
_0x5:
;      91    };
_0x3:
;      92 }
	LD   R16,Y+
	LD   R17,Y+
	RJMP _0xE2
;      93 
;      94 #ifndef _DEBUG_TERMINAL_IO_
;      95 // Get a character from the USART Receiver buffer
;      96 #define _ALTERNATE_GETCHAR_
;      97 #pragma used+
;      98 char getchar(void)
;      99 {
;     100 char data;
;     101 while (rx_counter==0);
;	data -> R17
;     102 data=rx_buffer[rx_rd_index];
;     103 if (++rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
;     104 #asm("cli")
;     105 --rx_counter;
;     106 #asm("sei")
;     107 return data;
;     108 }
;     109 #pragma used-
;     110 #endif
;     111 
;     112 // USART Transmitter buffer
;     113 #define TX_BUFFER_SIZE 8
;     114 char tx_buffer[TX_BUFFER_SIZE];

	.DSEG
_tx_buffer:
	.BYTE 0x8
;     115 
;     116 #if TX_BUFFER_SIZE<256
;     117 unsigned char tx_wr_index,tx_rd_index,tx_counter;
;     118 #else
;     119 unsigned int tx_wr_index,tx_rd_index,tx_counter;
;     120 #endif
;     121 
;     122 // USART Transmitter interrupt service routine
;     123 interrupt [USART_TXC] void usart_tx_isr(void)
;     124 {

	.CSEG
_usart_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     125 if (tx_counter)
	TST  R8
	BREQ _0xA
;     126    {
;     127    --tx_counter;
	DEC  R8
;     128    UDR=tx_buffer[tx_rd_index];
	MOV  R30,R9
	LDI  R31,0
	SUBI R30,LOW(-_tx_buffer)
	SBCI R31,HIGH(-_tx_buffer)
	LD   R30,Z
	OUT  0xC,R30
;     129    if (++tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
	INC  R9
	LDI  R30,LOW(8)
	CP   R30,R9
	BRNE _0xB
	CLR  R9
;     130    };
_0xB:
_0xA:
;     131 }
_0xE2:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     132 
;     133 #ifndef _DEBUG_TERMINAL_IO_
;     134 // Write a character to the USART Transmitter buffer
;     135 #define _ALTERNATE_PUTCHAR_
;     136 #pragma used+
;     137 void putchar(char c)
;     138 {
_putchar:
;     139 while (tx_counter == TX_BUFFER_SIZE);
;	c -> Y+0
_0xC:
	LDI  R30,LOW(8)
	CP   R30,R8
	BREQ _0xC
;     140 #asm("cli")
	cli
;     141 if (tx_counter || ((UCSRA & DATA_REGISTER_EMPTY)==0))
	TST  R8
	BRNE _0x10
	SBIC 0xB,5
	RJMP _0xF
_0x10:
;     142    {
;     143    tx_buffer[tx_wr_index]=c;
	MOV  R30,R6
	LDI  R31,0
	SUBI R30,LOW(-_tx_buffer)
	SBCI R31,HIGH(-_tx_buffer)
	LD   R26,Y
	STD  Z+0,R26
;     144    if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
	INC  R6
	LDI  R30,LOW(8)
	CP   R30,R6
	BRNE _0x12
	CLR  R6
;     145    ++tx_counter;
_0x12:
	INC  R8
;     146    }
;     147 else
	RJMP _0x13
_0xF:
;     148    UDR=c;
	LD   R30,Y
	OUT  0xC,R30
;     149 #asm("sei")
_0x13:
	sei
;     150 }
	ADIW R28,1
	RET
;     151 #pragma used-
;     152 #endif
;     153 
;     154 // Standard Input/Output functions
;     155 #include <stdio.h>
;     156 
;     157 // Declare your global variables here
;     158 unsigned char rom_codes[MAX_DEVICES,9];

	.DSEG
_rom_codes:
	.BYTE 0x10E
;     159 void main(void)
;     160 {

	.CSEG
_main:
;     161 // Declare your local variables here
;     162 
;     163 unsigned char i,devices;
;     164 char temp_buf[100];
;     165 
;     166 int temp, j;
;     167 // Input/Output Ports initialization
;     168 // Port A initialization
;     169 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=Out
;     170 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=0
;     171 PORTA=0x00;
	SBIW R28,63
	SBIW R28,37
;	i -> R17
;	devices -> R16
;	temp_buf -> Y+0
;	temp -> R18,R19
;	j -> R20,R21
	LDI  R30,LOW(0)
	OUT  0x1B,R30
;     172 DDRA=0x01;
	LDI  R30,LOW(1)
	OUT  0x1A,R30
;     173 
;     174 // Port B initialization
;     175 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;     176 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;     177 PORTB=0x00;
	LDI  R30,LOW(0)
	OUT  0x18,R30
;     178 DDRB=0x00;
	OUT  0x17,R30
;     179 
;     180 // Port C initialization
;     181 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;     182 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;     183 PORTC=0x00;
	OUT  0x15,R30
;     184 DDRC=0x00;
	OUT  0x14,R30
;     185 
;     186 // Port D initialization
;     187 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;     188 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;     189 PORTD=0x00;
	OUT  0x12,R30
;     190 DDRD=0x00;
	OUT  0x11,R30
;     191 
;     192 // Timer/Counter 0 initialization
;     193 // Clock source: System Clock
;     194 // Clock value: Timer 0 Stopped
;     195 // Mode: Normal top=FFh
;     196 // OC0 output: Disconnected
;     197 TCCR0=0x00;
	OUT  0x33,R30
;     198 TCNT0=0x00;
	OUT  0x32,R30
;     199 OCR0=0x00;
	OUT  0x3C,R30
;     200 
;     201 // Timer/Counter 1 initialization
;     202 // Clock source: System Clock
;     203 // Clock value: Timer 1 Stopped
;     204 // Mode: Normal top=FFFFh
;     205 // OC1A output: Discon.
;     206 // OC1B output: Discon.
;     207 // Noise Canceler: Off
;     208 // Input Capture on Falling Edge
;     209 // Timer 1 Overflow Interrupt: Off
;     210 // Input Capture Interrupt: Off
;     211 // Compare A Match Interrupt: Off
;     212 // Compare B Match Interrupt: Off
;     213 TCCR1A=0x00;
	OUT  0x2F,R30
;     214 TCCR1B=0x00;
	OUT  0x2E,R30
;     215 TCNT1H=0x00;
	OUT  0x2D,R30
;     216 TCNT1L=0x00;
	OUT  0x2C,R30
;     217 ICR1H=0x00;
	OUT  0x27,R30
;     218 ICR1L=0x00;
	OUT  0x26,R30
;     219 OCR1AH=0x00;
	OUT  0x2B,R30
;     220 OCR1AL=0x00;
	OUT  0x2A,R30
;     221 OCR1BH=0x00;
	OUT  0x29,R30
;     222 OCR1BL=0x00;
	OUT  0x28,R30
;     223 
;     224 // Timer/Counter 2 initialization
;     225 // Clock source: System Clock
;     226 // Clock value: Timer 2 Stopped
;     227 // Mode: Normal top=FFh
;     228 // OC2 output: Disconnected
;     229 ASSR=0x00;
	OUT  0x22,R30
;     230 TCCR2=0x00;
	OUT  0x25,R30
;     231 TCNT2=0x00;
	OUT  0x24,R30
;     232 OCR2=0x00;
	OUT  0x23,R30
;     233 
;     234 // External Interrupt(s) initialization
;     235 // INT0: Off
;     236 // INT1: Off
;     237 // INT2: Off
;     238 MCUCR=0x00;
	OUT  0x35,R30
;     239 MCUCSR=0x00;
	OUT  0x34,R30
;     240 
;     241 // Timer(s)/Counter(s) Interrupt(s) initialization
;     242 TIMSK=0x00;
	OUT  0x39,R30
;     243 
;     244 // USART initialization
;     245 // Communication Parameters: 8 Data, 1 Stop, No Parity
;     246 // USART Receiver: On
;     247 // USART Transmitter: On
;     248 // USART Mode: Asynchronous
;     249 // USART Baud Rate: 9600
;     250 
;     251 UCSRA=0x00;
	OUT  0xB,R30
;     252 UCSRB=0xD8;
	LDI  R30,LOW(216)
	OUT  0xA,R30
;     253 UCSRC=0x86;
	LDI  R30,LOW(134)
	OUT  0x20,R30
;     254 UBRRH=0x00;
	LDI  R30,LOW(0)
	OUT  0x20,R30
;     255 UBRRL=0x19;
	LDI  R30,LOW(25)
	OUT  0x9,R30
;     256 
;     257 // Analog Comparator initialization
;     258 // Analog Comparator: Off
;     259 // Analog Comparator Input Capture by Timer/Counter 1: Off
;     260 ACSR=0x80;
	LDI  R30,LOW(128)
	OUT  0x8,R30
;     261 SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x30,R30
;     262 
;     263 // 1 Wire Bus initialization
;     264 w1_init();
	CALL _w1_init
;     265 
;     266 // Global enable interrupts
;     267 #asm("sei")
	sei
;     268 devices=w1_search(0xf0,rom_codes);
	LDI  R30,LOW(240)
	ST   -Y,R30
	LDI  R30,LOW(_rom_codes)
	LDI  R31,HIGH(_rom_codes)
	ST   -Y,R31
	ST   -Y,R30
	CALL _w1_search
	MOV  R16,R30
;     269 while (1)
_0x14:
;     270       {
;     271       // Place your code here
;     272         for (i=0;i<devices;)
	LDI  R17,LOW(0)
_0x18:
	CP   R17,R16
	BRSH _0x19
;     273 
;     274           {
;     275 
;     276                temp=ds1820_temperature_10(&rom_codes[i,0]);
	LDI  R26,LOW(9)
	MUL  R17,R26
	MOVW R30,R0
	SUBI R30,LOW(-_rom_codes)
	SBCI R31,HIGH(-_rom_codes)
	ST   -Y,R31
	ST   -Y,R30
	RCALL _ds1820_temperature_10
	MOVW R18,R30
;     277                sprintf(temp_buf, "%d\n", temp);
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,0
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R18
	CALL __CWD1
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
;     278                for (j=0; j < strlen(temp_buf); j++)
	__GETWRN 20,21,0
_0x1B:
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	CP   R20,R30
	CPC  R21,R31
	BRSH _0x1C
;     279                {
;     280                         putchar(temp_buf[j]);
	MOVW R26,R28
	ADD  R26,R20
	ADC  R27,R21
	LD   R30,X
	ST   -Y,R30
	CALL _putchar
;     281                         // putchar('A');
;     282                };
	__ADDWRN 20,21,1
	RJMP _0x1B
_0x1C:
;     283           };
	RJMP _0x18
_0x19:
;     284       };
	RJMP _0x14
;     285 }
_0x1D:
	RJMP _0x1D


	.DSEG
___ds1820_scratch_pad:
	.BYTE 0x9

	.CSEG
_ds1820_select:
	ST   -Y,R17
	CALL _w1_init
	CPI  R30,0
	BRNE _0x1E
	LDI  R30,LOW(0)
	LDD  R17,Y+0
	RJMP _0xDE
_0x1E:
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	SBIW R30,0
	BREQ _0x1F
	LDI  R30,LOW(85)
	ST   -Y,R30
	CALL _w1_write
	LDI  R17,LOW(0)
_0x21:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	ST   -Y,R30
	CALL _w1_write
	SUBI R17,-LOW(1)
	CPI  R17,8
	BRLO _0x21
	RJMP _0x23
_0x1F:
	LDI  R30,LOW(204)
	ST   -Y,R30
	CALL _w1_write
_0x23:
	LDI  R30,LOW(1)
	LDD  R17,Y+0
_0xDE:
	ADIW R28,3
	RET
_ds1820_read_spd:
	CALL __SAVELOCR4
	LDD  R30,Y+4
	LDD  R31,Y+4+1
	ST   -Y,R31
	ST   -Y,R30
	CALL _ds1820_select
	CPI  R30,0
	BRNE _0x24
	LDI  R30,LOW(0)
	RJMP _0xDD
_0x24:
	LDI  R30,LOW(190)
	ST   -Y,R30
	CALL _w1_write
	LDI  R17,LOW(0)
	__POINTWRM 18,19,___ds1820_scratch_pad
_0x26:
	PUSH R19
	PUSH R18
	__ADDWRN 18,19,1
	CALL _w1_read
	POP  R26
	POP  R27
	ST   X,R30
	SUBI R17,-LOW(1)
	CPI  R17,9
	BRLO _0x26
	LDI  R30,LOW(___ds1820_scratch_pad)
	LDI  R31,HIGH(___ds1820_scratch_pad)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(9)
	ST   -Y,R30
	CALL _w1_dow_crc8
	CALL __LNEGB1
_0xDD:
	CALL __LOADLOCR4
	ADIW R28,6
	RET
_ds1820_temperature_10:
	LD   R30,Y
	LDD  R31,Y+1
	ST   -Y,R31
	ST   -Y,R30
	CALL _ds1820_select
	CPI  R30,0
	BRNE _0x28
	LDI  R30,LOW(55537)
	LDI  R31,HIGH(55537)
	RJMP _0xDC
_0x28:
	LDI  R30,LOW(68)
	ST   -Y,R30
	CALL _w1_write
	LDI  R30,LOW(550)
	LDI  R31,HIGH(550)
	ST   -Y,R31
	ST   -Y,R30
	CALL _delay_ms
	LD   R30,Y
	LDD  R31,Y+1
	ST   -Y,R31
	ST   -Y,R30
	CALL _ds1820_read_spd
	CPI  R30,0
	BRNE _0x29
	LDI  R30,LOW(55537)
	LDI  R31,HIGH(55537)
	RJMP _0xDC
_0x29:
	CALL _w1_init
	__GETB1HMN ___ds1820_scratch_pad,1
	LDI  R30,LOW(0)
	MOVW R26,R30
	LDS  R30,___ds1820_scratch_pad
	LDI  R31,0
	OR   R30,R26
	OR   R31,R27
	LDI  R26,LOW(5)
	LDI  R27,HIGH(5)
	CALL __MULW12
_0xDC:
	ADIW R28,2
	RET
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
__put_G4:
	ST   -Y,R17
	ST   -Y,R16
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x30
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x32
	__CPWRN 16,17,2
	BRLO _0x33
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	ST   X+,R30
	ST   X,R31
_0x32:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	LDD  R26,Y+6
	STD  Z+0,R26
_0x33:
	RJMP _0x34
_0x30:
	LDD  R30,Y+6
	ST   -Y,R30
	CALL _putchar
_0x34:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,7
	RET
__print_G4:
	SBIW R28,6
	CALL __SAVELOCR6
	LDI  R17,0
_0x35:
	LDD  R30,Y+18
	LDD  R31,Y+18+1
	ADIW R30,1
	STD  Y+18,R30
	STD  Y+18+1,R31
	SBIW R30,1
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x37
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x3B
	CPI  R18,37
	BRNE _0x3C
	LDI  R17,LOW(1)
	RJMP _0x3D
_0x3C:
	RCALL SUBOPT_0x0
_0x3D:
	RJMP _0x3A
_0x3B:
	CPI  R30,LOW(0x1)
	BRNE _0x3E
	CPI  R18,37
	BRNE _0x3F
	RCALL SUBOPT_0x0
	RJMP _0xDF
_0x3F:
	LDI  R17,LOW(2)
	LDI  R20,LOW(0)
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x40
	LDI  R16,LOW(1)
	RJMP _0x3A
_0x40:
	CPI  R18,43
	BRNE _0x41
	LDI  R20,LOW(43)
	RJMP _0x3A
_0x41:
	CPI  R18,32
	BRNE _0x42
	LDI  R20,LOW(32)
	RJMP _0x3A
_0x42:
	RJMP _0x43
_0x3E:
	CPI  R30,LOW(0x2)
	BRNE _0x44
_0x43:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x45
	ORI  R16,LOW(128)
	RJMP _0x3A
_0x45:
	RJMP _0x46
_0x44:
	CPI  R30,LOW(0x3)
	BREQ PC+3
	JMP _0x3A
_0x46:
	CPI  R18,48
	BRLO _0x49
	CPI  R18,58
	BRLO _0x4A
_0x49:
	RJMP _0x48
_0x4A:
	MOV  R26,R21
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R21,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x3A
_0x48:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x4E
	RCALL SUBOPT_0x1
	LD   R30,X
	RCALL SUBOPT_0x2
	RJMP _0x4F
_0x4E:
	CPI  R30,LOW(0x73)
	BRNE _0x51
	RCALL SUBOPT_0x1
	RCALL SUBOPT_0x3
	CALL _strlen
	MOV  R17,R30
	RJMP _0x52
_0x51:
	CPI  R30,LOW(0x70)
	BRNE _0x54
	RCALL SUBOPT_0x1
	RCALL SUBOPT_0x3
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x52:
	ORI  R16,LOW(2)
	ANDI R16,LOW(127)
	LDI  R19,LOW(0)
	RJMP _0x55
_0x54:
	CPI  R30,LOW(0x64)
	BREQ _0x58
	CPI  R30,LOW(0x69)
	BRNE _0x59
_0x58:
	ORI  R16,LOW(4)
	RJMP _0x5A
_0x59:
	CPI  R30,LOW(0x75)
	BRNE _0x5B
_0x5A:
	LDI  R30,LOW(_tbl10_G4*2)
	LDI  R31,HIGH(_tbl10_G4*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(5)
	RJMP _0x5C
_0x5B:
	CPI  R30,LOW(0x58)
	BRNE _0x5E
	ORI  R16,LOW(8)
	RJMP _0x5F
_0x5E:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0x90
_0x5F:
	LDI  R30,LOW(_tbl16_G4*2)
	LDI  R31,HIGH(_tbl16_G4*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(4)
_0x5C:
	SBRS R16,2
	RJMP _0x61
	RCALL SUBOPT_0x1
	CALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	SBIW R26,0
	BRGE _0x62
	CALL __ANEGW1
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDI  R20,LOW(45)
_0x62:
	CPI  R20,0
	BREQ _0x63
	SUBI R17,-LOW(1)
	RJMP _0x64
_0x63:
	ANDI R16,LOW(251)
_0x64:
	RJMP _0x65
_0x61:
	RCALL SUBOPT_0x1
	CALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
_0x65:
_0x55:
	SBRC R16,0
	RJMP _0x66
_0x67:
	CP   R17,R21
	BRSH _0x69
	SBRS R16,7
	RJMP _0x6A
	SBRS R16,2
	RJMP _0x6B
	ANDI R16,LOW(251)
	MOV  R18,R20
	SUBI R17,LOW(1)
	RJMP _0x6C
_0x6B:
	LDI  R18,LOW(48)
_0x6C:
	RJMP _0x6D
_0x6A:
	LDI  R18,LOW(32)
_0x6D:
	RCALL SUBOPT_0x0
	SUBI R21,LOW(1)
	RJMP _0x67
_0x69:
_0x66:
	MOV  R19,R17
	SBRS R16,1
	RJMP _0x6E
_0x6F:
	CPI  R19,0
	BREQ _0x71
	SBRS R16,3
	RJMP _0x72
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ADIW R30,1
	STD  Y+6,R30
	STD  Y+6+1,R31
	SBIW R30,1
	LPM  R30,Z
	RJMP _0xE0
_0x72:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R30,X+
	STD  Y+6,R26
	STD  Y+6+1,R27
_0xE0:
	ST   -Y,R30
	RCALL SUBOPT_0x4
	CPI  R21,0
	BREQ _0x74
	SUBI R21,LOW(1)
_0x74:
	SUBI R19,LOW(1)
	RJMP _0x6F
_0x71:
	RJMP _0x75
_0x6E:
_0x77:
	LDI  R18,LOW(48)
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ADIW R30,2
	STD  Y+6,R30
	STD  Y+6+1,R31
	SBIW R30,2
	CALL __GETW1PF
	STD  Y+8,R30
	STD  Y+8+1,R31
_0x79:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	CP   R26,R30
	CPC  R27,R31
	BRLO _0x7B
	SUBI R18,-LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	SUB  R30,R26
	SBC  R31,R27
	STD  Y+10,R30
	STD  Y+10+1,R31
	RJMP _0x79
_0x7B:
	CPI  R18,58
	BRLO _0x7C
	SBRS R16,3
	RJMP _0x7D
	SUBI R18,-LOW(7)
	RJMP _0x7E
_0x7D:
	SUBI R18,-LOW(39)
_0x7E:
_0x7C:
	SBRC R16,4
	RJMP _0x80
	CPI  R18,49
	BRSH _0x82
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,1
	BRNE _0x81
_0x82:
	RJMP _0xE1
_0x81:
	CP   R21,R19
	BRLO _0x86
	SBRS R16,0
	RJMP _0x87
_0x86:
	RJMP _0x85
_0x87:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x88
	LDI  R18,LOW(48)
_0xE1:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x89
	ANDI R16,LOW(251)
	ST   -Y,R20
	RCALL SUBOPT_0x4
	CPI  R21,0
	BREQ _0x8A
	SUBI R21,LOW(1)
_0x8A:
_0x89:
_0x88:
_0x80:
	RCALL SUBOPT_0x0
	CPI  R21,0
	BREQ _0x8B
	SUBI R21,LOW(1)
_0x8B:
_0x85:
	SUBI R19,LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,2
	BRLO _0x78
	RJMP _0x77
_0x78:
_0x75:
	SBRS R16,0
	RJMP _0x8C
_0x8D:
	CPI  R21,0
	BREQ _0x8F
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	RCALL SUBOPT_0x2
	RJMP _0x8D
_0x8F:
_0x8C:
_0x90:
_0x4F:
_0xDF:
	LDI  R17,LOW(0)
_0x3A:
	RJMP _0x35
_0x37:
	CALL __LOADLOCR6
	ADIW R28,20
	RET
_sprintf:
	PUSH R15
	MOV  R15,R24
	SBIW R28,2
	ST   -Y,R17
	ST   -Y,R16
	MOVW R26,R28
	CALL __ADDW2R15
	MOVW R16,R26
	MOVW R26,R28
	ADIW R26,6
	CALL __ADDW2R15
	CALL __GETW1P
	STD  Y+2,R30
	STD  Y+2+1,R31
	MOVW R26,R28
	ADIW R26,4
	CALL __ADDW2R15
	CALL __GETW1P
	ST   -Y,R31
	ST   -Y,R30
	ST   -Y,R17
	ST   -Y,R16
	MOVW R30,R28
	ADIW R30,6
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   -Y,R31
	ST   -Y,R30
	RCALL __print_G4
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	LDI  R30,LOW(0)
	ST   X,R30
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,4
	POP  R15
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x0:
	ST   -Y,R18
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RJMP __put_G4

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x1:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,4
	STD  Y+16,R26
	STD  Y+16+1,R27
	ADIW R26,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x2:
	ST   -Y,R30
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RJMP __put_G4

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3:
	CALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x4:
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RJMP __put_G4

_isdigit:
	ldi  r30,1
	ld   r31,y+
	cpi  r31,'0'
	brlo __isdigit0
	cpi  r31,'9'+1
	brlo __isdigit1
__isdigit0:
	clr  r30
__isdigit1:
	ret

_isspace:
	ldi  r30,1
	ld   r31,y+
	cpi  r31,' '
	breq __isspace1
	cpi  r31,9
	brlo __isspace0
	cpi  r31,14
	brlo __isspace1
__isspace0:
	clr  r30
__isspace1:
	ret

_isxdigit:
	ldi  r30,1
	ld   r31,y+
	subi r31,0x30
	brcs __isxdigit0
	cpi  r31,10
	brcs __isxdigit1
	andi r31,0x5f
	subi r31,7
	cpi  r31,10
	brcs __isxdigit0
	cpi  r31,16
	brcs __isxdigit1
__isxdigit0:
	clr  r30
__isxdigit1:
	ret

_strlen:
	ld   r26,y+
	ld   r27,y+
	clr  r30
	clr  r31
__strlen0:
	ld   r22,x+
	tst  r22
	breq __strlen1
	adiw r30,1
	rjmp __strlen0
__strlen1:
	ret

_strlenf:
	clr  r26
	clr  r27
	ld   r30,y+
	ld   r31,y+
__strlenf0:
	lpm  r0,z+
	tst  r0
	breq __strlenf1
	adiw r26,1
	rjmp __strlenf0
__strlenf1:
	movw r30,r26
	ret

_delay_ms:
	ld   r30,y+
	ld   r31,y+
	adiw r30,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0x3E8
	wdr
	sbiw r30,1
	brne __delay_ms0
__delay_ms1:
	ret

_w1_init:
	clr  r30
	cbi  __w1_port,__w1_bit
	sbi  __w1_port-1,__w1_bit
	__DELAY_USW 0x1E0
	cbi  __w1_port-1,__w1_bit
	__DELAY_USB 0x13
	sbis __w1_port-2,__w1_bit
	ret
	__DELAY_USB 0x65
	sbis __w1_port-2,__w1_bit
	inc  r30
	__DELAY_USW 0x186
	ret

__w1_read_bit:
	sbi  __w1_port-1,__w1_bit
	__DELAY_USB 0x3
	cbi  __w1_port-1,__w1_bit
	__DELAY_USB 0xF
	clc
	sbic __w1_port-2,__w1_bit
	sec
	ror  r30
	__DELAY_USB 0x6B
	ret

__w1_write_bit:
	clt
	sbi  __w1_port-1,__w1_bit
	__DELAY_USB 0x3
	sbrc r23,0
	cbi  __w1_port-1,__w1_bit
	__DELAY_USB 0x11
	sbic __w1_port-2,__w1_bit
	rjmp __w1_write_bit0
	sbrs r23,0
	rjmp __w1_write_bit1
	ret
__w1_write_bit0:
	sbrs r23,0
	ret
__w1_write_bit1:
	__DELAY_USB 0x64
	cbi  __w1_port-1,__w1_bit
	__DELAY_USB 0x7
	set
	ret

_w1_read:
	ldi  r22,8
	__w1_read0:
	rcall __w1_read_bit
	dec  r22
	brne __w1_read0
	ret

_w1_write:
	ldi  r22,8
	ld   r23,y+
	clr  r30
__w1_write0:
	rcall __w1_write_bit
	brtc __w1_write1
	ror  r23
	dec  r22
	brne __w1_write0
	inc  r30
__w1_write1:
	ret

_w1_search:
	push r20
	push r21
	clr  r1
	clr  r20
	ld   r26,y
	ldd  r27,y+1
__w1_search0:
	mov  r0,r1
	clr  r1
	rcall _w1_init
	tst  r30
	breq __w1_search7
	ldd  r30,y+2
	st   -y,r30
	rcall _w1_write
	ldi  r21,1
__w1_search1:
	cp   r21,r0
	brsh __w1_search6
	rcall __w1_read_bit
	sbrc r30,7
	rjmp __w1_search2
	rcall __w1_read_bit
	sbrc r30,7
	rjmp __w1_search3
	rcall __sel_bit
	and  r24,r25
	brne __w1_search3
	mov  r1,r21
	rjmp __w1_search3
__w1_search2:
	rcall __w1_read_bit
__w1_search3:
	rcall __sel_bit
	and  r24,r25
	ldi  r23,0
	breq __w1_search5
__w1_search4:
	ldi  r23,1
__w1_search5:
	rcall __w1_write_bit
	rjmp __w1_search13
__w1_search6:
	rcall __w1_read_bit
	sbrs r30,7
	rjmp __w1_search9
	rcall __w1_read_bit
	sbrs r30,7
	rjmp __w1_search8
__w1_search7:
	mov  r30,r20
	pop  r21
	pop  r20
	adiw r28,3
	ret
__w1_search8:
	set
	rcall __set_bit
	rjmp __w1_search4
__w1_search9:
	rcall __w1_read_bit
	sbrs r30,7
	rjmp __w1_search10
	rjmp __w1_search11
__w1_search10:
	cp   r21,r0
	breq __w1_search12
	mov  r1,r21
__w1_search11:
	clt
	rcall __set_bit
	clr  r23
	rcall __w1_write_bit
	rjmp __w1_search13
__w1_search12:
	set
	rcall __set_bit
	ldi  r23,1
	rcall __w1_write_bit
__w1_search13:
	inc  r21
	cpi  r21,65
	brlt __w1_search1
	rcall __w1_read_bit
	rol  r30
	rol  r30
	andi r30,1
	adiw r26,8
	st   x,r30
	sbiw r26,8
	inc  r20
	tst  r1
	breq __w1_search7
	ldi  r21,9
__w1_search14:
	ld   r30,x
	adiw r26,9
	st   x,r30
	sbiw r26,8
	dec  r21
	brne __w1_search14
	rjmp __w1_search0

__sel_bit:
	mov  r30,r21
	dec  r30
	mov  r22,r30
	lsr  r30
	lsr  r30
	lsr  r30
	clr  r31
	add  r30,r26
	adc  r31,r27
	ld   r24,z
	ldi  r25,1
	andi r22,7
__sel_bit0:
	breq __sel_bit1
	lsl  r25
	dec  r22
	rjmp __sel_bit0
__sel_bit1:
	ret

__set_bit:
	rcall __sel_bit
	brts __set_bit2
	com  r25
	and  r24,r25
	rjmp __set_bit3
__set_bit2:
	or   r24,r25
__set_bit3:
	st   z,r24
	ret

_w1_dow_crc8:
	clr  r30
	ld   r24,y
	tst  r24
	breq __w1_dow_crc83
	ldi  r22,0x18
	ldd  r26,y+1
	ldd  r27,y+2
__w1_dow_crc80:
	ldi  r25,8
	ld   r31,x+
__w1_dow_crc81:
	mov  r23,r31
	eor  r23,r30
	ror  r23
	brcc __w1_dow_crc82
	eor  r30,r22
__w1_dow_crc82:
	ror  r30
	lsr  r31
	dec  r25
	brne __w1_dow_crc81
	dec  r24
	brne __w1_dow_crc80
__w1_dow_crc83:
	adiw r28,3
	ret

__ADDW2R15:
	CLR  R0
	ADD  R26,R15
	ADC  R27,R0
	RET

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__LNEGB1:
	TST  R30
	LDI  R30,1
	BREQ __LNEGB1F
	CLR  R30
__LNEGB1F:
	RET

__MULW12U:
	MUL  R31,R26
	MOV  R31,R0
	MUL  R30,R27
	ADD  R31,R0
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	RET

__MULW12:
	RCALL __CHKSIGNW
	RCALL __MULW12U
	BRTC __MULW121
	RCALL __ANEGW1
__MULW121:
	RET

__CHKSIGNW:
	CLT
	SBRS R31,7
	RJMP __CHKSW1
	RCALL __ANEGW1
	SET
__CHKSW1:
	SBRS R27,7
	RJMP __CHKSW2
	COM  R26
	COM  R27
	ADIW R26,1
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSW2:
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETW1PF:
	LPM  R0,Z+
	LPM  R31,Z
	MOV  R30,R0
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
