/*
void *memset(void *str, int c, int len)
{
    //Creates an unsigned char pointer that points to the address stored in str. 
    //This pointer is used to access individual bytes in memory.
    unsigned char* p = str;

    //For loop that loops until the len variable is equal to 0.
    while(len--)
    {
        //Sets the byte pointed to by p to the value of c. P will increment each time the loop goes.
        *p++ = (unsigned char)c;
    }
    //Returns the original pointer str.
    return str;
}*/