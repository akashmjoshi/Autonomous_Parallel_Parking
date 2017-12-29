#line 1 "C:/Users/User 2/Desktop/B.E Project/NormalMove/normalMove.c"
int dist1,dist2,dist3,dist4;


void move_car_f(int req_speed);
void move_car_b(int req_speed);
void stop_car_f();
void stop_car_b();

void main()
{
 ADCON1=0x0A;

 TRISA=0x6F;
 PWM1_Init(10000);
#line 24 "C:/Users/User 2/Desktop/B.E Project/NormalMove/normalMove.c"
 TRISB=0;
 TRISC=0;
 move_car_f(127);
 while(1)
 {
 float dist_tra=0.0;
 dist1=ADC_Read(0);
 dist2=ADC_Read(1);
 dist3=ADC_Read(2);
 dist4=ADC_Read(3);

 LATB=0x0A;
 Delay_ms(5);

 LATB=0x09;
 Delay_ms(5);

 LATB=0x05;
 Delay_ms(5);

 LATB=0x06;
 Delay_ms(5);
 if(dist1>355)
 {


 move_car_b(200);
 }

 if(dist4>355)
 {


 move_car_f(200);
 }

 if(dist2>355)
 {


 move_car_b(255);
 }

 if(dist3>355)
 {
 stop_car_b();

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
