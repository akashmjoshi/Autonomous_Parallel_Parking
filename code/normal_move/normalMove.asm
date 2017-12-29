
_main:

;normalMove.c,9 :: 		void main()
;normalMove.c,11 :: 		ADCON1=0x0A;  //AN3 will be used as reference and A4-A0 will be set as analog i/p i.e A0-A5 !A4
	MOVLW       10
	MOVWF       ADCON1+0 
;normalMove.c,13 :: 		TRISA=0x6F;
	MOVLW       111
	MOVWF       TRISA+0 
;normalMove.c,14 :: 		PWM1_Init(10000);
	BSF         T2CON+0, 0, 0
	BCF         T2CON+0, 1, 0
	MOVLW       124
	MOVWF       PR2+0, 0
	CALL        _PWM1_Init+0, 0
;normalMove.c,24 :: 		TRISB=0;
	CLRF        TRISB+0 
;normalMove.c,25 :: 		TRISC=0;              //RC1-CCP2(PWM2)  RC2-CCP1(PWM1)      RC4/5- F/W   RC6/7- L/R
	CLRF        TRISC+0 
;normalMove.c,26 :: 		move_car_f(127);
	MOVLW       127
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
;normalMove.c,27 :: 		while(1)
L_main0:
;normalMove.c,29 :: 		float dist_tra=0.0;
;normalMove.c,30 :: 		dist1=ADC_Read(0);
	CLRF        FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist1+0 
	MOVF        R1, 0 
	MOVWF       _dist1+1 
;normalMove.c,31 :: 		dist2=ADC_Read(1);
	MOVLW       1
	MOVWF       FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist2+0 
	MOVF        R1, 0 
	MOVWF       _dist2+1 
;normalMove.c,32 :: 		dist3=ADC_Read(2);
	MOVLW       2
	MOVWF       FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist3+0 
	MOVF        R1, 0 
	MOVWF       _dist3+1 
;normalMove.c,33 :: 		dist4=ADC_Read(3);
	MOVLW       3
	MOVWF       FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist4+0 
	MOVF        R1, 0 
	MOVWF       _dist4+1 
;normalMove.c,35 :: 		LATB=0x0A;             //  To send 1010 at PortB
	MOVLW       10
	MOVWF       LATB+0 
;normalMove.c,36 :: 		Delay_ms(5);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_main2:
	DECFSZ      R13, 1, 1
	BRA         L_main2
	DECFSZ      R12, 1, 1
	BRA         L_main2
	NOP
;normalMove.c,38 :: 		LATB=0x09;             //  To send 1001 at PortB
	MOVLW       9
	MOVWF       LATB+0 
;normalMove.c,39 :: 		Delay_ms(5);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_main3:
	DECFSZ      R13, 1, 1
	BRA         L_main3
	DECFSZ      R12, 1, 1
	BRA         L_main3
	NOP
;normalMove.c,41 :: 		LATB=0x05;             //  To send 0101 at PortB
	MOVLW       5
	MOVWF       LATB+0 
;normalMove.c,42 :: 		Delay_ms(5);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_main4:
	DECFSZ      R13, 1, 1
	BRA         L_main4
	DECFSZ      R12, 1, 1
	BRA         L_main4
	NOP
;normalMove.c,44 :: 		LATB=0x06;             //  To send 0110 at PortB
	MOVLW       6
	MOVWF       LATB+0 
;normalMove.c,45 :: 		Delay_ms(5);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_main5:
	DECFSZ      R13, 1, 1
	BRA         L_main5
	DECFSZ      R12, 1, 1
	BRA         L_main5
	NOP
;normalMove.c,46 :: 		if(dist1>355)
	MOVLW       128
	XORLW       1
	MOVWF       R0 
	MOVLW       128
	XORWF       _dist1+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main14
	MOVF        _dist1+0, 0 
	SUBLW       99
L__main14:
	BTFSC       STATUS+0, 0 
	GOTO        L_main6
;normalMove.c,50 :: 		move_car_b(200);
	MOVLW       200
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;normalMove.c,51 :: 		}
L_main6:
;normalMove.c,53 :: 		if(dist4>355)
	MOVLW       128
	XORLW       1
	MOVWF       R0 
	MOVLW       128
	XORWF       _dist4+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main15
	MOVF        _dist4+0, 0 
	SUBLW       99
L__main15:
	BTFSC       STATUS+0, 0 
	GOTO        L_main7
;normalMove.c,57 :: 		move_car_f(200);
	MOVLW       200
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
;normalMove.c,58 :: 		}
L_main7:
;normalMove.c,60 :: 		if(dist2>355)
	MOVLW       128
	XORLW       1
	MOVWF       R0 
	MOVLW       128
	XORWF       _dist2+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main16
	MOVF        _dist2+0, 0 
	SUBLW       99
L__main16:
	BTFSC       STATUS+0, 0 
	GOTO        L_main8
;normalMove.c,64 :: 		move_car_b(255);
	MOVLW       255
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;normalMove.c,65 :: 		}
L_main8:
;normalMove.c,67 :: 		if(dist3>355)
	MOVLW       128
	XORLW       1
	MOVWF       R0 
	MOVLW       128
	XORWF       _dist3+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main17
	MOVF        _dist3+0, 0 
	SUBLW       99
L__main17:
	BTFSC       STATUS+0, 0 
	GOTO        L_main9
;normalMove.c,69 :: 		stop_car_b();
	CALL        _stop_car_b+0, 0
;normalMove.c,71 :: 		}
L_main9:
;normalMove.c,74 :: 		}
	GOTO        L_main0
;normalMove.c,75 :: 		}
	GOTO        $+0
; end of _main

_move_car_f:

;normalMove.c,78 :: 		void move_car_f(int req_speed)
;normalMove.c,80 :: 		PWM1_Start();
	CALL        _PWM1_Start+0, 0
;normalMove.c,81 :: 		PWM1_Set_Duty(req_speed);
	MOVF        FARG_move_car_f_req_speed+0, 0 
	MOVWF       FARG_PWM1_Set_Duty_new_duty+0 
	CALL        _PWM1_Set_Duty+0, 0
;normalMove.c,82 :: 		RC4_bit=1;
	BSF         RC4_bit+0, 4 
;normalMove.c,83 :: 		RC5_bit=0;
	BCF         RC5_bit+0, 5 
;normalMove.c,84 :: 		}
	RETURN      0
; end of _move_car_f

_move_car_b:

;normalMove.c,85 :: 		void move_car_b(int req_speed)
;normalMove.c,87 :: 		PWM1_Start();
	CALL        _PWM1_Start+0, 0
;normalMove.c,88 :: 		PWM1_Set_Duty(req_speed);
	MOVF        FARG_move_car_b_req_speed+0, 0 
	MOVWF       FARG_PWM1_Set_Duty_new_duty+0 
	CALL        _PWM1_Set_Duty+0, 0
;normalMove.c,89 :: 		RC4_bit=0;
	BCF         RC4_bit+0, 4 
;normalMove.c,90 :: 		RC5_bit=1;
	BSF         RC5_bit+0, 5 
;normalMove.c,91 :: 		}
	RETURN      0
; end of _move_car_b

_stop_car_f:

;normalMove.c,92 :: 		void stop_car_f()
;normalMove.c,94 :: 		move_car_b(127);
	MOVLW       127
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;normalMove.c,95 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_stop_car_f10:
	DECFSZ      R13, 1, 1
	BRA         L_stop_car_f10
	DECFSZ      R12, 1, 1
	BRA         L_stop_car_f10
	DECFSZ      R11, 1, 1
	BRA         L_stop_car_f10
	NOP
;normalMove.c,96 :: 		PWM1_Stop();
	CALL        _PWM1_Stop+0, 0
;normalMove.c,99 :: 		}
	RETURN      0
; end of _stop_car_f

_stop_car_b:

;normalMove.c,100 :: 		void stop_car_b()
;normalMove.c,104 :: 		PWM1_Stop();
	CALL        _PWM1_Stop+0, 0
;normalMove.c,108 :: 		}
	RETURN      0
; end of _stop_car_b

_wheel_l:

;normalMove.c,109 :: 		void wheel_l()
;normalMove.c,113 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_wheel_l11:
	DECFSZ      R13, 1, 1
	BRA         L_wheel_l11
	DECFSZ      R12, 1, 1
	BRA         L_wheel_l11
	DECFSZ      R11, 1, 1
	BRA         L_wheel_l11
	NOP
;normalMove.c,114 :: 		}
	RETURN      0
; end of _wheel_l

_wheel_r:

;normalMove.c,115 :: 		void wheel_r()
;normalMove.c,119 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_wheel_r12:
	DECFSZ      R13, 1, 1
	BRA         L_wheel_r12
	DECFSZ      R12, 1, 1
	BRA         L_wheel_r12
	DECFSZ      R11, 1, 1
	BRA         L_wheel_r12
	NOP
;normalMove.c,120 :: 		}
	RETURN      0
; end of _wheel_r

_wheel_c:

;normalMove.c,121 :: 		void wheel_c()
;normalMove.c,125 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_wheel_c13:
	DECFSZ      R13, 1, 1
	BRA         L_wheel_c13
	DECFSZ      R12, 1, 1
	BRA         L_wheel_c13
	DECFSZ      R11, 1, 1
	BRA         L_wheel_c13
	NOP
;normalMove.c,126 :: 		}
	RETURN      0
; end of _wheel_c
