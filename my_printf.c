#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

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

    for (size_t i = 1; i < strlen(format); ++i)
    {
        if (format[i - 1] == '%')
        {
            if ( (format[i] != 'd') &&
                 (format[i] != 'o') &&
                 (format[i] != 'u') &&
                 (format[i] != 'x') &&
                 (format[i] != 's') &&
                 (format[i] != 'c') &&
                 (format[i] != 'p') )
            {
                return 0;
            }
        }
    }

    va_list ap;
    int d, count = 0;
    char c, *s;
    unsigned u;
    void *p;

    va_start(ap, format);

    for (size_t i = 0; i < strlen(format); ++i)
    {
        if (i == 0 && format[i] != '%')
        {
            ++count;
            write(STDOUT_FILENO,&format[i],1); 
            continue;
        }

        if (format[i-1] == '%' && format[i] == 'd')
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
                ++count;   
                write(STDOUT_FILENO,&hyphen,1); 
            }

            int print_digit;
            while (len_d > 0)
            {
                if (len_d == 1)
                {
                    print_digit = d % 10 + 48;
                    ++count;
                    write(STDOUT_FILENO,&print_digit,1); 
                    break;
                }

                int power_10 = 1;
                for (int j = 1; j < len_d; ++j)
                {
                    power_10 *= 10;
                }

                print_digit = ( d / power_10 ) % 10 + 48;
                ++count;
                write(STDOUT_FILENO,&print_digit,1); 
                --len_d;
            }
        }
        else if (format[i-1]=='%' && format[i] == 'u')
        {
            u = va_arg(ap, unsigned);

            int len_u = 1;
            int hyphen = 45;
            unsigned copy_u = u;
            copy_u /= 10;
            while (copy_u > 0)
            {
                ++len_u;
                copy_u /= 10;
            }

            int print_digit;
            while (len_u > 0)
            {
                if (len_u == 1)
                {
                    print_digit = u % 10 + 48;
                    ++count;
                    write(STDOUT_FILENO,&print_digit,1); 
                    break;
                }

                unsigned power_10 = 1;
                for (int j = 1; j < len_u; ++j)
                {
                    power_10 *= 10;
                }

                print_digit = (int)(u / power_10 % 10) + 48;
                ++count;
                write(STDOUT_FILENO,&print_digit,1); 

                --len_u;
            }
        }
        else if (format[i-1]=='%' && format[i] == 'o')
        {
            u = va_arg(ap, unsigned);

            unsigned count_octal = 1;
            unsigned sum = 0;
            unsigned octal_convert = u;
            while (octal_convert > 0)
            {
                unsigned rem = octal_convert % 8;
                unsigned power_10 = 1;
                for (int j = 1; j < count_octal; ++j)
                {
                    power_10 *= 10;
                }
                sum += rem * power_10;
                octal_convert /= 8;
                ++count_octal;
            }

            u = sum;

            int len_u = 1;
            int hyphen = 45;
            unsigned copy_u = u;
            copy_u /= 10;
            while (copy_u > 0)
            {
                ++len_u;
                copy_u /= 10;
            }

            int print_digit;
            while (len_u > 0)
            {
                if (len_u == 1)
                {
                    print_digit = u % 10 + 48;
                    ++count;
                    write(STDOUT_FILENO,&print_digit,1); 
                    break;
                }

                unsigned power_10 = 1;
                for (int j = 1; j < len_u; ++j)
                {
                    power_10 *= 10;
                }

                print_digit = (int)(u / power_10 % 10) + 48;
                ++count;
                write(STDOUT_FILENO,&print_digit,1); 

                --len_u;
            }
        }
        else if (format[i-1]=='%' && format[i] == 'x')
        {
            u = va_arg(ap, unsigned);

            char * hex = (char*)malloc(999);

            unsigned ind = 0U;
            unsigned hex_convert = u;
            while (hex_convert > 0)
            {
                unsigned rem = hex_convert % 16;

                if (rem >= 0 && rem <=9)
                {
                    hex[ind++] = rem + 48;
                }
                else if (rem == 10)
                {
                    hex[ind++] = 'a';
                }
                else if (rem == 11)
                {
                    hex[ind++] = 'b';
                }
                else if (rem == 12)
                {
                    hex[ind++] = 'c';
                }
                else if (rem == 13)
                {
                    hex[ind++] = 'd';
                }
                else if (rem == 14)
                {
                    hex[ind++] = 'e';
                }
                else if (rem == 15)
                {
                    hex[ind++] = 'f';
                }
                
                hex_convert /= 16;
            }

            hex[ind] = '\0';

            for (int i = strlen(hex) - 1; i >= 0; --i)
            {
                ++count;
                write(STDOUT_FILENO,&hex[i],1);
            }

            free(hex);
        }
        else if (format[i-1]=='%' && format[i] == 'p')
        {
            p = va_arg(ap, void *);

            if (p == NULL)
            {
                char * nil = "(nil)";
                for (int i = 0; i < strlen(nil); ++i)
                {
                    ++count;
                    write(STDOUT_FILENO,&nil[i],1);
                }
            }
            else 
            {
                long unsigned p_lu = (long unsigned)p;
                char * hex = (char*)malloc(999);

                int ind = 0;
                long unsigned hex_convert = p_lu;
                while (hex_convert > 0)
                {
                    long unsigned rem = hex_convert % (long unsigned)16;

                    if (rem >= 0 && rem <=9)
                    {
                        hex[ind++] = (int)rem + 48;
                    }
                    else if (rem == 10)
                    {
                        hex[ind++] = 'a';
                    }
                    else if (rem == 11)
                    {
                        hex[ind++] = 'b';
                    }
                    else if (rem == 12)
                    {
                        hex[ind++] = 'c';
                    }
                    else if (rem == 13)
                    {
                        hex[ind++] = 'd';
                    }
                    else if (rem == 14)
                    {
                        hex[ind++] = 'e';
                    }
                    else if (rem == 15)
                    {
                        hex[ind++] = 'f';
                    }
                    
                    hex_convert /= 16;
                }

                hex[ind] = '\0';

                char * ex = "0x";
                for (int i = 0; i < strlen(ex); ++i)
                {
                    ++count;
                    write(STDOUT_FILENO,&ex[i],1);
                }

                for (int i = strlen(hex) - 1; i >= 0; --i)
                {
                    ++count;
                    write(STDOUT_FILENO,&hex[i],1);
                }

                free(hex);
            }
        }
        else if (format[i-1]=='%' && format[i] == 's')
        {
            s = va_arg(ap, char *);
            while (*s != '\0')
            {
                ++count;
                write(STDOUT_FILENO,&(*s),1);
                ++s;
            }            
        }
        else if (format[i-1]=='%' && format[i] == 'c')
        {
            c = (char) va_arg(ap, int);
            ++count;
            write(STDOUT_FILENO,&c,1);
        }
        else if (format[i] != '%')
        {
            ++count;
            write(STDOUT_FILENO,&format[i],1); 
        }
    }    
    va_end(ap);

    return count;
}


int main( int argc, const char* argv[] )
{
    char * a = (char*)malloc(1980);

	int apple = printf( "Hello %d %u what: %s %c %o %x %p", -123456 , 2U, "Apppleee-bee", 't', 23, 54, a);
    printf("\n%d\n", apple);

    apple = my_printf( "Hello %d %u what: %s %c %o %x %p", -123456 , 2U, "Apppleee-bee", 't', 23, 54, a);
    printf("\n%d\n", apple);

    free(a);
}