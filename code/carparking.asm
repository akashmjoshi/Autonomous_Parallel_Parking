
_main:

;carparking.c,14 :: 		void main()
;carparking.c,16 :: 		ADCON1=0x1A;  //AN3 will be used as reference and A4-A0 will be set as analog i/p i.e A0-A5 !A4
	MOVLW       26
	MOVWF       ADCON1+0 
;carparking.c,18 :: 		TRISA=0x6F;
	MOVLW       111
	MOVWF       TRISA+0 
;carparking.c,19 :: 		PWM1_Init(5000);
	BSF         T2CON+0, 0, 0
	BCF         T2CON+0, 1, 0
	MOVLW       249
	MOVWF       PR2+0, 0
	CALL        _PWM1_Init+0, 0
;carparking.c,25 :: 		INTCON.T0IE=1;          //enable interrupt for timer1
	BSF         INTCON+0, 5 
;carparking.c,32 :: 		TRISC=0;              //RC1-CCP2(PWM2)  RC2-CCP1(PWM1)      RC4/5- F/W   RC6/7- L/R
	CLRF        TRISC+0 
;carparking.c,34 :: 		while(1)
L_main0:
;carparking.c,36 :: 		float dist_tra=0.0;
	CLRF        main_dist_tra_L1+0 
	CLRF        main_dist_tra_L1+1 
	CLRF        main_dist_tra_L1+2 
	CLRF        main_dist_tra_L1+3 
;carparking.c,37 :: 		dist1=ADC_Read(0);
	CLRF        FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist1+0 
	MOVF        R1, 0 
	MOVWF       _dist1+1 
;carparking.c,38 :: 		dist2=ADC_Read(1);
	MOVLW       1
	MOVWF       FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist2+0 
	MOVF        R1, 0 
	MOVWF       _dist2+1 
;carparking.c,39 :: 		dist3=ADC_Read(2);
	MOVLW       2
	MOVWF       FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist3+0 
	MOVF        R1, 0 
	MOVWF       _dist3+1 
;carparking.c,40 :: 		dist4=ADC_Read(4);
	MOVLW       4
	MOVWF       FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _dist4+0 
	MOVF        R1, 0 
	MOVWF       _dist4+1 
;carparking.c,42 :: 		if(INTCON.TMR0IF)        //timer for distance
	BTFSS       INTCON+0, 2 
	GOTO        L_main2
;carparking.c,44 :: 		counter++;
	INFSNZ      _counter+0, 1 
	INCF        _counter+1, 1 
;carparking.c,45 :: 		INTCON.TMR0IF=0;
	BCF         INTCON+0, 2 
;carparking.c,46 :: 		}
L_main2:
;carparking.c,48 :: 		if(dist1<15)
	MOVLW       128
	XORWF       _dist1+1, 0 
	MOVWF       R0 
	MOVLW       128
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main23
	MOVLW       15
	SUBWF       _dist1+0, 0 
L__main23:
	BTFSC       STATUS+0, 0 
	GOTO        L_main3
;carparking.c,50 :: 		stop_car_f();
	CALL        _stop_car_f+0, 0
;carparking.c,51 :: 		move_car_b(127);
	MOVLW       127
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;carparking.c,52 :: 		}
L_main3:
;carparking.c,54 :: 		if(dist4<15)
	MOVLW       128
	XORWF       _dist4+1, 0 
	MOVWF       R0 
	MOVLW       128
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main24
	MOVLW       15
	SUBWF       _dist4+0, 0 
L__main24:
	BTFSC       STATUS+0, 0 
	GOTO        L_main4
;carparking.c,56 :: 		stop_car_b();
	CALL        _stop_car_b+0, 0
;carparking.c,57 :: 		move_car_f(127);
	MOVLW       127
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
;carparking.c,58 :: 		}
L_main4:
;carparking.c,60 :: 		if(dist2>35)
	MOVLW       128
	MOVWF       R0 
	MOVLW       128
	XORWF       _dist2+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main25
	MOVF        _dist2+0, 0 
	SUBLW       35
L__main25:
	BTFSC       STATUS+0, 0 
	GOTO        L_main5
;carparking.c,62 :: 		T0CON=0x82;  // T0CONbits.T08BIT = 0;  T0CONbits.T0CS = 0;  T0CONbits.PSA = 0;  T0CONbits.T0PS2-0 = 010;  T0CONbits.TMR0ON = 1;
	MOVLW       130
	MOVWF       T0CON+0 
;carparking.c,63 :: 		TMR0H=0xB;
	MOVLW       11
	MOVWF       TMR0H+0 
;carparking.c,64 :: 		TMR0L=0xDC;  // TMR0= 3036 FOR 100ms
	MOVLW       220
	MOVWF       TMR0L+0 
;carparking.c,65 :: 		flag=1;
	MOVLW       1
	MOVWF       _flag+0 
	MOVLW       0
	MOVWF       _flag+1 
;carparking.c,66 :: 		}
L_main5:
;carparking.c,68 :: 		if(dist2<=35 && flag==1)
	MOVLW       128
	MOVWF       R0 
	MOVLW       128
	XORWF       _dist2+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main26
	MOVF        _dist2+0, 0 
	SUBLW       35
L__main26:
	BTFSS       STATUS+0, 0 
	GOTO        L_main8
	MOVLW       0
	XORWF       _flag+1, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main27
	MOVLW       1
	XORWF       _flag+0, 0 
L__main27:
	BTFSS       STATUS+0, 2 
	GOTO        L_main8
L__main22:
;carparking.c,70 :: 		T0CON.TMR0ON=0;
	BCF         T0CON+0, 7 
;carparking.c,71 :: 		dist_tra=speed*counter/10;        //speed needs to be calculated in terms of cm/s
	MOVF        _counter+0, 0 
	MOVWF       R0 
	MOVF        _counter+1, 0 
	MOVWF       R1 
	CALL        _Int2Double+0, 0
	MOVF        _speed+0, 0 
	MOVWF       R4 
	MOVF        _speed+1, 0 
	MOVWF       R5 
	MOVF        _speed+2, 0 
	MOVWF       R6 
	MOVF        _speed+3, 0 
	MOVWF       R7 
	CALL        _Mul_32x32_FP+0, 0
	MOVLW       0
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVLW       32
	MOVWF       R6 
	MOVLW       130
	MOVWF       R7 
	CALL        _Div_32x32_FP+0, 0
	MOVF        R0, 0 
	MOVWF       main_dist_tra_L1+0 
	MOVF        R1, 0 
	MOVWF       main_dist_tra_L1+1 
	MOVF        R2, 0 
	MOVWF       main_dist_tra_L1+2 
	MOVF        R3, 0 
	MOVWF       main_dist_tra_L1+3 
;carparking.c,72 :: 		if(dist_tra>=75)         //Giving a safe distance of 65
	MOVLW       0
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	MOVLW       22
	MOVWF       R6 
	MOVLW       133
	MOVWF       R7 
	CALL        _Compare_Double+0, 0
	MOVLW       0
	BTFSC       STATUS+0, 0 
	MOVLW       1
	MOVWF       R0 
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main9
;carparking.c,74 :: 		stop_car_f();
	CALL        _stop_car_f+0, 0
;carparking.c,75 :: 		parking_mode(dist_tra);
	MOVF        main_dist_tra_L1+0, 0 
	MOVWF       FARG_parking_mode_p_space+0 
	MOVF        main_dist_tra_L1+1, 0 
	MOVWF       FARG_parking_mode_p_space+1 
	MOVF        main_dist_tra_L1+2, 0 
	MOVWF       FARG_parking_mode_p_space+2 
	MOVF        main_dist_tra_L1+3, 0 
	MOVWF       FARG_parking_mode_p_space+3 
	CALL        _parking_mode+0, 0
;carparking.c,76 :: 		}
	GOTO        L_main10
L_main9:
;carparking.c,78 :: 		move_car_f(127);
	MOVLW       127
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
L_main10:
;carparking.c,79 :: 		flag=0;
	CLRF        _flag+0 
	CLRF        _flag+1 
;carparking.c,80 :: 		}
L_main8:
;carparking.c,83 :: 		}
	GOTO        L_main0
;carparking.c,84 :: 		}
	GOTO        $+0
; end of _main

_move_car_f:

;carparking.c,86 :: 		void move_car_f(int req_speed)
;carparking.c,88 :: 		PWM1_Start();
	CALL        _PWM1_Start+0, 0
;carparking.c,89 :: 		PWM1_Set_Duty(req_speed);
	MOVF        FARG_move_car_f_req_speed+0, 0 
	MOVWF       FARG_PWM1_Set_Duty_new_duty+0 
	CALL        _PWM1_Set_Duty+0, 0
;carparking.c,90 :: 		RC4_bit=1;
	BSF         RC4_bit+0, 4 
;carparking.c,91 :: 		RC5_bit=0;
	BCF         RC5_bit+0, 5 
;carparking.c,92 :: 		}
	RETURN      0
; end of _move_car_f

_move_car_b:

;carparking.c,93 :: 		void move_car_b(int req_speed)
;carparking.c,95 :: 		PWM1_Start();
	CALL        _PWM1_Start+0, 0
;carparking.c,96 :: 		PWM1_Set_Duty(req_speed);
	MOVF        FARG_move_car_b_req_speed+0, 0 
	MOVWF       FARG_PWM1_Set_Duty_new_duty+0 
	CALL        _PWM1_Set_Duty+0, 0
;carparking.c,97 :: 		RC4_bit=0;
	BCF         RC4_bit+0, 4 
;carparking.c,98 :: 		RC5_bit=1;
	BSF         RC5_bit+0, 5 
;carparking.c,99 :: 		}
	RETURN      0
; end of _move_car_b

_stop_car_f:

;carparking.c,100 :: 		void stop_car_f()
;carparking.c,102 :: 		move_car_b(127);
	MOVLW       127
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;carparking.c,103 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_stop_car_f11:
	DECFSZ      R13, 1, 1
	BRA         L_stop_car_f11
	DECFSZ      R12, 1, 1
	BRA         L_stop_car_f11
	DECFSZ      R11, 1, 1
	BRA         L_stop_car_f11
	NOP
;carparking.c,104 :: 		PWM1_Stop();
	CALL        _PWM1_Stop+0, 0
;carparking.c,105 :: 		}
	RETURN      0
; end of _stop_car_f

_stop_car_b:

;carparking.c,106 :: 		void stop_car_b()
;carparking.c,108 :: 		move_car_f(127);
	MOVLW       127
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
;carparking.c,109 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_stop_car_b12:
	DECFSZ      R13, 1, 1
	BRA         L_stop_car_b12
	DECFSZ      R12, 1, 1
	BRA         L_stop_car_b12
	DECFSZ      R11, 1, 1
	BRA         L_stop_car_b12
	NOP
;carparking.c,110 :: 		PWM1_Stop();
	CALL        _PWM1_Stop+0, 0
;carparking.c,111 :: 		}
	RETURN      0
; end of _stop_car_b

_wheel_l:

;carparking.c,112 :: 		void wheel_l()
;carparking.c,116 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_wheel_l13:
	DECFSZ      R13, 1, 1
	BRA         L_wheel_l13
	DECFSZ      R12, 1, 1
	BRA         L_wheel_l13
	DECFSZ      R11, 1, 1
	BRA         L_wheel_l13
	NOP
;carparking.c,117 :: 		}
	RETURN      0
; end of _wheel_l

_wheel_r:

;carparking.c,118 :: 		void wheel_r()
;carparking.c,122 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_wheel_r14:
	DECFSZ      R13, 1, 1
	BRA         L_wheel_r14
	DECFSZ      R12, 1, 1
	BRA         L_wheel_r14
	DECFSZ      R11, 1, 1
	BRA         L_wheel_r14
	NOP
;carparking.c,123 :: 		}
	RETURN      0
; end of _wheel_r

_wheel_c:

;carparking.c,124 :: 		void wheel_c()
;carparking.c,128 :: 		delay_ms(500);
	MOVLW       13
	MOVWF       R11, 0
	MOVLW       175
	MOVWF       R12, 0
	MOVLW       182
	MOVWF       R13, 0
L_wheel_c15:
	DECFSZ      R13, 1, 1
	BRA         L_wheel_c15
	DECFSZ      R12, 1, 1
	BRA         L_wheel_c15
	DECFSZ      R11, 1, 1
	BRA         L_wheel_c15
	NOP
;carparking.c,129 :: 		}
	RETURN      0
; end of _wheel_c

_parking_mode:

;carparking.c,130 :: 		void parking_mode(float p_space)
;carparking.c,132 :: 		move_car_f(127);
	MOVLW       127
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
;carparking.c,133 :: 		delay_ms(3000);
	MOVLW       77
	MOVWF       R11, 0
	MOVLW       25
	MOVWF       R12, 0
	MOVLW       79
	MOVWF       R13, 0
L_parking_mode16:
	DECFSZ      R13, 1, 1
	BRA         L_parking_mode16
	DECFSZ      R12, 1, 1
	BRA         L_parking_mode16
	DECFSZ      R11, 1, 1
	BRA         L_parking_mode16
	NOP
	NOP
;carparking.c,134 :: 		stop_car_f();
	CALL        _stop_car_f+0, 0
;carparking.c,136 :: 		wheel_l();
	CALL        _wheel_l+0, 0
;carparking.c,137 :: 		move_car_b(127);
	MOVLW       127
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;carparking.c,138 :: 		delay_ms(4000);
	MOVLW       102
	MOVWF       R11, 0
	MOVLW       118
	MOVWF       R12, 0
	MOVLW       193
	MOVWF       R13, 0
L_parking_mode17:
	DECFSZ      R13, 1, 1
	BRA         L_parking_mode17
	DECFSZ      R12, 1, 1
	BRA         L_parking_mode17
	DECFSZ      R11, 1, 1
	BRA         L_parking_mode17
;carparking.c,139 :: 		stop_car_b();
	CALL        _stop_car_b+0, 0
;carparking.c,141 :: 		wheel_c();
	CALL        _wheel_c+0, 0
;carparking.c,142 :: 		move_car_f(127);
	MOVLW       127
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
;carparking.c,143 :: 		delay_ms(2000);
	MOVLW       51
	MOVWF       R11, 0
	MOVLW       187
	MOVWF       R12, 0
	MOVLW       223
	MOVWF       R13, 0
L_parking_mode18:
	DECFSZ      R13, 1, 1
	BRA         L_parking_mode18
	DECFSZ      R12, 1, 1
	BRA         L_parking_mode18
	DECFSZ      R11, 1, 1
	BRA         L_parking_mode18
	NOP
	NOP
;carparking.c,144 :: 		stop_car_f();
	CALL        _stop_car_f+0, 0
;carparking.c,146 :: 		wheel_l();
	CALL        _wheel_l+0, 0
;carparking.c,147 :: 		move_car_b(127);
	MOVLW       127
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;carparking.c,148 :: 		delay_ms(4000);
	MOVLW       102
	MOVWF       R11, 0
	MOVLW       118
	MOVWF       R12, 0
	MOVLW       193
	MOVWF       R13, 0
L_parking_mode19:
	DECFSZ      R13, 1, 1
	BRA         L_parking_mode19
	DECFSZ      R12, 1, 1
	BRA         L_parking_mode19
	DECFSZ      R11, 1, 1
	BRA         L_parking_mode19
;carparking.c,149 :: 		stop_car_b();
	CALL        _stop_car_b+0, 0
;carparking.c,151 :: 		wheel_r();
	CALL        _wheel_r+0, 0
;carparking.c,152 :: 		move_car_b(127);
	MOVLW       127
	MOVWF       FARG_move_car_b_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_b_req_speed+1 
	CALL        _move_car_b+0, 0
;carparking.c,153 :: 		delay_ms(3000);
	MOVLW       77
	MOVWF       R11, 0
	MOVLW       25
	MOVWF       R12, 0
	MOVLW       79
	MOVWF       R13, 0
L_parking_mode20:
	DECFSZ      R13, 1, 1
	BRA         L_parking_mode20
	DECFSZ      R12, 1, 1
	BRA         L_parking_mode20
	DECFSZ      R11, 1, 1
	BRA         L_parking_mode20
	NOP
	NOP
;carparking.c,154 :: 		stop_car_b();
	CALL        _stop_car_b+0, 0
;carparking.c,156 :: 		wheel_l();
	CALL        _wheel_l+0, 0
;carparking.c,157 :: 		move_car_f(127);
	MOVLW       127
	MOVWF       FARG_move_car_f_req_speed+0 
	MOVLW       0
	MOVWF       FARG_move_car_f_req_speed+1 
	CALL        _move_car_f+0, 0
;carparking.c,158 :: 		delay_ms(4000);
	MOVLW       102
	MOVWF       R11, 0
	MOVLW       118
	MOVWF       R12, 0
	MOVLW       193
	MOVWF       R13, 0
L_parking_mode21:
	DECFSZ      R13, 1, 1
	BRA         L_parking_mode21
	DECFSZ      R12, 1, 1
	BRA         L_parking_mode21
	DECFSZ      R11, 1, 1
	BRA         L_parking_mode21
;carparking.c,159 :: 		stop_car_f();
	CALL        _stop_car_f+0, 0
;carparking.c,160 :: 		}
	RETURN      0
; end of _parking_mode
