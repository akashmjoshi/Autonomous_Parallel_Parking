#line 1 "C:/Users/User 2/Desktop/B.E Project/carparking.c"
int counter=0, dist1,dist2,dist3,dist4,flag=0;
float speed;


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
 ADCON1=0x1A;

 TRISA=0x6F;
 PWM1_Init(5000);
#line 25 "C:/Users/User 2/Desktop/B.E Project/carparking.c"
 INTCON.T0IE=1;
#line 32 "C:/Users/User 2/Desktop/B.E Project/carparking.c"
 TRISC=0;

 while(1)
 {
 float dist_tra=0.0;
 dist1=ADC_Read(0);
 dist2=ADC_Read(1);
 dist3=ADC_Read(2);
 dist4=ADC_Read(4);

 if(INTCON.TMR0IF)
 {
 counter++;
 INTCON.TMR0IF=0;
 }

 if(dist1<15)
 {
 stop_car_f();
 move_car_b(127);
 }

 if(dist4<15)
 {
 stop_car_b();
 move_car_f(127);
 }

 if(dist2>35)
 {
 T0CON=0x82;
 TMR0H=0xB;
 TMR0L=0xDC;
 flag=1;
 }

 if(dist2<=35 && flag==1)
 {
 T0CON.TMR0ON=0;
 dist_tra=speed*counter/10;
 if(dist_tra>=75)
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
