int counter=0, dist1,dist2,dist3,dist4,flag=0;
float speed;
//int req_speed=127;          //50% DC

void move_car_f(int req_speed);
void move_car_b(int req_speed);
void stop_car_f();
void stop_car_b();
void wheel_l();
void wheel_r();
void wheel_c();
void parking_mode(float p_space);

void main() 
{
    ADCON1=0x1A;  //AN3 will be used as reference and A4-A0 will be set as analog i/p i.e A0-A5 !A4
                  //A6 is osc2
    TRISA=0x6F;
    PWM1_Init(5000);
    
    /*PWM1_Start();
    PWM1_Set_Duty(req_speed);*/
    
    
    INTCON.T0IE=1;          //enable interrupt for timer1
    
    
    /*T0CON=0x82;
    TMR0H=0xB;   //3036 FOR 100ms
    TMR0L=0xDC;*/
    
    TRISC=0;              //RC1-CCP2(PWM2)  RC2-CCP1(PWM1)      RC4/5- F/W   RC6/7- L/R
   
    while(1)
    {
            float dist_tra=0.0;
            dist1=ADC_Read(0);
            dist2=ADC_Read(1);
            dist3=ADC_Read(2);
            dist4=ADC_Read(4);

            if(INTCON.TMR0IF)        //timer for distance
            {
             counter++;
             INTCON.TMR0IF=0;
            }

            if(dist1<15)
            {                                  //back n forth
              stop_car_f();
              move_car_b(127);
            }
            
            if(dist4<15)
            {                                  //back n forth
              stop_car_b();
              move_car_f(127);
            }
            
            if(dist2>35)
            {
              T0CON=0x82;  // T0CONbits.T08BIT = 0;  T0CONbits.T0CS = 0;  T0CONbits.PSA = 0;  T0CONbits.T0PS2-0 = 010;  T0CONbits.TMR0ON = 1;
              TMR0H=0xB;
              TMR0L=0xDC;  // TMR0= 3036 FOR 100ms
              flag=1;
            }
            
            if(dist2<=35 && flag==1)
            {
             T0CON.TMR0ON=0;
             dist_tra=speed*counter/10;        //speed needs to be calculated in terms of cm/s
                      if(dist_tra>=75)         //Giving a safe distance of 65
                      {
                          stop_car_f();
                          parking_mode(dist_tra);
                      }
                      else
                          move_car_f(127);
             flag=0;
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
}
void stop_car_b()
{
     move_car_f(127);
     delay_ms(500);
     PWM1_Stop();
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
void parking_mode(float p_space)
{
     move_car_f(127);
     delay_ms(3000);
     stop_car_f();
     
     wheel_l();
     move_car_b(127);
     delay_ms(4000);
     stop_car_b();
     
     wheel_c();
     move_car_f(127);
     delay_ms(2000);
     stop_car_f();
     
     wheel_l();
     move_car_b(127);
     delay_ms(4000);
     stop_car_b();
     
     wheel_r();
     move_car_b(127);
     delay_ms(3000);
     stop_car_b();
     
     wheel_l();
     move_car_f(127);
     delay_ms(4000);
     stop_car_f();
}