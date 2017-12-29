void main()
{
	unsigned int i=0;
	TRISB=0;                       // Set PortB as output port
	while(1)
	{
		LATB=0x0c;             //  To send 1110 at PortB
        	Delay_ms(20);

        	LATB=0x06;             //  To send 1101 at PortB
        	Delay_ms(20);

        	LATB=0x03;             //  To send 1011 at PortB
        	Delay_ms(20);

        	LATB=0x09;             //  To send 0111 at PortB
       	 	Delay_ms(20);
        }
}