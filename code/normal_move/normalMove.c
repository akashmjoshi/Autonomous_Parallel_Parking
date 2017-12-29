int dist1,dist2,dist3,dist4;


void move_car_f(int req_speed);
void move_car_b(int req_speed);
void stop_car_f();
void stop_car_b();

void main()
{
    ADCON1=0x0A;  //AN3 will be used as reference and A4-A0 will be set as analog i/p i.e A0-A5 !A4
                  //A6 is osc2
    TRISA=0x6F;
    PWM1_Init(10000);

    /*PWM1_Start();
    PWM1_Set_Duty(req_speed);*/



    /*T0CON=0x82;
    TMR0H=0xB;   //3036 FOR 100ms
    TMR0L=0xDC;*/
    TRISB=0;
    TRISC=0;              //RC1-CCP2(PWM2)  RC2-CCP1(PWM1)      RC4/5- F/W   RC6/7- L/R
    move_car_f(127);
    while(1)
    {
            float dist_tra=0.0;
            dist1=ADC_Read(0);
            dist2=ADC_Read(1);
            dist3=ADC_Read(2);
            dist4=ADC_Read(3);

            LATB=0x0A;             //  To send 1010 at PortB
                Delay_ms(5);

                LATB=0x09;             //  To send 1001 at PortB
                Delay_ms(5);

                LATB=0x05;             //  To send 0101 at PortB
                Delay_ms(5);

                LATB=0x06;             //  To send 0110 at PortB
                        Delay_ms(5);
            if(dist1>355)
            {                                  //back n forth
            //  stop_car_f();
            //  delay_ms(500);
              move_car_b(200);
            }

            if(dist4>355)
            {                                  //back n forth
            //  stop_car_b();
            //  delay_ms(500);
              move_car_f(200);
            }

            if(dist2>355)
            {
             // stop_car_f();
             // delay_ms(500);
              move_car_b(255);
            }

            if(dist3>355)
            {
              stop_car_b();
             // move_car_f(127);
            }


    }
}


void move_car_f(int req_speed)
{
     PWM1_Start();
     PWM1_Set_Duty(req_speed);
     RC4_bit=1;
     RC5_bit=0;
}
void move_car_b(int req_speed)
{
     PWM1_Start();
     PWM1_Set_Duty(req_speed);
     RC4_bit=0;
     RC5_bit=1;
}
void stop_car_f()
{
     move_car_b(127);
     delay_ms(500);
     PWM1_Stop();
   //  RC4_bit=0;
   //  RC5_bit=0;
}
void stop_car_b()
{
    // move_car_f(127);
    // delay_ms(500);
     PWM1_Stop();
    // RC4_bit=0;
    // RC5_bit=0;

}
void wheel_l()
{


 delay_ms(500);
}
void wheel_r()
{


 delay_ms(500);
}
void wheel_c()
{


 delay_ms(500);
}