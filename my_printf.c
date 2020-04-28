#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int my_printf(char * restrict format, ...)
{
    int count_args = 0;
    for (size_t i = 0; i < strlen(format); ++i)
    {
        if (format[i] == '%')
        {
            ++count_args;
        }
    }

    va_list ap;
    int d;
    char c, *s;

    printf("%s\n", format);

    va_start(ap, format);
    for (size_t i = 0; i < strlen(format); ++i)
    {
        if (format[i] == 'd')
        {
            d = va_arg(ap, int);

            bool neg = false;
            if (d < 0)
            {
                neg = true;
            }

            if (neg == true)
            {
                d = -d;
            }

            int len_d = 1;
            int hyphen = 45;
            int copy_d = d;
            copy_d /= 10;
            while (copy_d > 0)
            {
                ++len_d;
                copy_d /= 10;
            }
            
            if (neg == true)
            {                
                write(STDOUT_FILENO,&hyphen,1); 
            }

            int print_digit;
            while (len_d > 0)
            {
                if (len_d == 1)
                {
                    print_digit = d % 10 + 48;
                    write(STDOUT_FILENO,&print_digit,1); 
                    break;
                }

                int power_10 = 1;
                for (int j = 1; j < len_d; ++j)
                {
                    power_10 *= 10;
                }

                print_digit = d / power_10 + 48;
                write(STDOUT_FILENO,&print_digit,1); 
                --len_d;
            }
        }
        else if (format[i] == 's')
        {
            s = va_arg(ap, char *);
            printf("%s", s);
        }
        else if (format[i] == 'c')
        {
            c = (char) va_arg(ap, int);
            printf("%c", c);
        }
    }    
    va_end(ap);

    //printf("%d", count_args);
    return 0;
}


int main( int argc, const char* argv[] )
{
	printf("Hello%6.8f", 4.2);

    printf( "\n" );

    my_printf( "Hello %d", 34);
}