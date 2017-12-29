#line 1 "C:/Users/User 2/Desktop/B.E Project/NormalMove/Stepper.c"
void main()
{
 unsigned int i=0;
 TRISB=0;
 while(1)
 {
 LATB=0x0c;
 Delay_ms(20);

 LATB=0x06;
 Delay_ms(20);

 LATB=0x03;
 Delay_ms(20);

 LATB=0x09;
 Delay_ms(20);
 }
}
