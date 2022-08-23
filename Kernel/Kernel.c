void _start(){
    char *str = "Hello form kernel!", *ch;
	unsigned short *VideoMemory = (unsigned short*) 0xb8000;
	unsigned i;
	
	for (ch = str, i = 0; *ch; ch++, i++)
		VideoMemory[i] = (unsigned char) *ch | 0x0700;

    while(1);
}