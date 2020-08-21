char *itoa(int val, char *buf, unsigned radix)
{
    char *p;             
    char *firstdig;      
    char temp;           
    unsigned digval;     
    p = buf;
    if(val <0)
    {
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }
    firstdig = p; 
    do{
        digval = (unsigned)(val % radix);
        val /= radix;
       
        if  (digval > 9)
            *p++ = (char)(digval - 10 + 'a'); 
        else
            *p++ = (char)(digval + '0');      
    }while(val > 0);
   
    *p-- = '\0';         
    do{
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;
        --p;
        ++firstdig;        
    }while(firstdig < p);  
    return buf;
}

void Playnote(unsigned char flag, int i, int noteVisible){
	char str[2] = "7";
	OPT_CHECK = 0xFF;
	if (noteVisible){
		if (i%7!=0) itoa(i%7,str,10);
		Disp(3,4,1,str);
	}
	while (OPT_CHECK&flag){
		bee_Speak = ~bee_Speak;
		Delay_xMs(NOTE[i-1]);
		OPT_CHECK = 0xFF;
	}
}