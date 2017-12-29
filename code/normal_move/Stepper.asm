
_main:

;Stepper.c,1 :: 		void main()
;Stepper.c,3 :: 		unsigned int i=0;
;Stepper.c,4 :: 		TRISB=0;                       // Set PortB as output port
	CLRF        TRISB+0 
;Stepper.c,5 :: 		while(1)
L_main0:
;Stepper.c,7 :: 		LATB=0x0c;             //  To send 1110 at PortB
	MOVLW       12
	MOVWF       LATB+0 
;Stepper.c,8 :: 		Delay_ms(20);
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_main2:
	DECFSZ      R13, 1, 1
	BRA         L_main2
	DECFSZ      R12, 1, 1
	BRA         L_main2
	NOP
	NOP
;Stepper.c,10 :: 		LATB=0x06;             //  To send 1101 at PortB
	MOVLW       6
	MOVWF       LATB+0 
;Stepper.c,11 :: 		Delay_ms(20);
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_main3:
	DECFSZ      R13, 1, 1
	BRA         L_main3
	DECFSZ      R12, 1, 1
	BRA         L_main3
	NOP
	NOP
;Stepper.c,13 :: 		LATB=0x03;             //  To send 1011 at PortB
	MOVLW       3
	MOVWF       LATB+0 
;Stepper.c,14 :: 		Delay_ms(20);
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_main4:
	DECFSZ      R13, 1, 1
	BRA         L_main4
	DECFSZ      R12, 1, 1
	BRA         L_main4
	NOP
	NOP
;Stepper.c,16 :: 		LATB=0x09;             //  To send 0111 at PortB
	MOVLW       9
	MOVWF       LATB+0 
;Stepper.c,17 :: 		Delay_ms(20);
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_main5:
	DECFSZ      R13, 1, 1
	BRA         L_main5
	DECFSZ      R12, 1, 1
	BRA         L_main5
	NOP
	NOP
;Stepper.c,18 :: 		}
	GOTO        L_main0
;Stepper.c,19 :: 		}
	GOTO        $+0
; end of _main
