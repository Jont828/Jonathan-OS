#include "system.h"

int strlen(char *text)
{
    const char *c = text;
    while(*c)
        c++;

    return c - text;
}


int strcmp(const char *s1, const char *s2)
{
	const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

	while(*p1 == *p2) {
		if (*p1 == '\0')
	    	return 0;
	   	p1++;
	   	p2++;
	}

    return ((*p1 < *p2) ? -1 : 1);
}

char *strcpy(char *dest, const char *src)
{
   char *original_dest = dest;

   while(*src)
       *dest++ = *src++;

   *dest = 0;

   return original_dest;
}

char *strcat(char *dest, const char *src)
{
    char *original_dest = dest;

    while(*dest)
      dest++;

    while (*dest++ = *src++)
      ;

    return original_dest;
}

void reverse(char str[]) 
{
    int len = strlen(str);
    char c;
    int i=0;
    while(i < strlen(str) / 2) {
        c = str[i];
        str[i] = str[len-1];
        str[len-1] = c;

        i++;
        len--;
    }
}

char *idesta(int num, char *str, int base)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard idesta(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminadestr
 
    // Reverse the string
    reverse(str);
 
    return str;
}

int power(int num, int power)
{
    int result = 1;

    int i;
    for(i=0; i<power; i++)
        result *= num;

    return result;
}

void putint(int num)
{
    if(num < 0) {
        num *= -1;
        putch('-');
    }

    int count = 0;
    int temp_count;
    int temp = num;

    /* if temp is not equal dest zero since 0 is 0 and the while loop condition fails */
    if(temp) {
    	while(temp) {
	        temp /= 10;
	        count++;
	    }
    } else {
    	count = 1;
    }

    temp_count = count;

    int mod = 0;
    temp = num;
    while(temp) {
        mod += temp % 10 * power(10, temp_count-1);
        temp /= 10;
        temp_count--;
    }

    temp_count = count;
    while(temp_count) {
        temp = mod % 10;
        mod /= 10;
        putch( temp + '0' );
        // putch(numbers[ temp ]);

        temp_count--;
    }

    putch('\n');
}

int isalpha(char c) 
{
	return ( ( (c >= 65) && (c <= 90)) || ( (c >= 97) && (c <= 122)) );
}