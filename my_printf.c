#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

int global_count = 0;

void my_write(int input)
{
    ++global_count;
    write(STDOUT_FILENO,&input,1);
}

void my_str_write(char * input)
{
    if (input == NULL)
    {
        my_str_write("(null)");
    }
    else 
    {
        for (int i = 0; i < strlen(input); ++i)
        {
            my_write(input[i]);
        }    
    }
}

void my_str_write_reverse(char * input)
{
    if (input == NULL)
    {
        my_str_write("(null)");
    }
    else 
    {
        for (int i = strlen(input) - 1; i > -1; --i)
        {
            my_write(input[i]);
        }    
    }
}

void my_print_number_base(bool is_negative, unsigned number)
{
    if (is_negative == true)
    {
        my_write(45);
    }

    int len = 1;
    unsigned copy_number = number;
    copy_number /= 10U;
    while (copy_number > 0)
    {
        ++len;
        copy_number /= 10U;
    }

    int print_digit;
    while (len > 0)
    {
        unsigned power_10 = 1;
        for (int j = 1; j < len; ++j)
        {
            power_10 *= 10;
        }

        print_digit = ( number / power_10 ) % 10 + 48;
        my_write(print_digit);
        --len;
    }
}

char * my_hex(long unsigned number)
{
    char * hex = (char*)malloc(999);

    int ind = 0;
    while (number > 0)
    {
        long unsigned rem = number % (long unsigned)16;

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
        
        number /= (long unsigned)16;
    }

    hex[ind] = '\0';

    return hex;
}

int my_printf(char * restrict format, ...)
{
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
        if (i == 0)
        {
            if (format[i] != '%')
            {
                my_write(format[i]);
            }

            continue;
        }

        if (format[i-1] == '%' && format[i] == 'd')
        {
            d = va_arg(ap, int);
            const bool is_negative = d < 0;
            d = is_negative ? -d : d;
            my_print_number_base(is_negative, (unsigned)d);
        }
        else if (format[i-1]=='%' && format[i] == 'u')
        {
            u = va_arg(ap, unsigned);
            my_print_number_base(false, u);
        }
        else if (format[i-1]=='%' && format[i] == 'o')
        {
            u = va_arg(ap, unsigned);

            unsigned count_octal = 1;
            unsigned sum = 0;
            unsigned octal_convert = u;
            while (octal_convert > 0)
            {
                unsigned rem = octal_convert % 8U;
                unsigned power_10 = 1U;
                for (unsigned j = 1U; j < count_octal; ++j)
                {
                    power_10 *= 10U;
                }
                sum += rem * power_10;
                octal_convert /= 8U;
                ++count_octal;
            }
            my_print_number_base(false, sum);
        }
        else if (format[i-1]=='%' && format[i] == 'x')
        {
            u = va_arg(ap, unsigned);
            char * hex = my_hex( (long unsigned) u );
            my_str_write_reverse(hex);
            free(hex);
        }
        else if (format[i-1]=='%' && format[i] == 'p')
        {
            p = va_arg(ap, void *);
            if (p == NULL)
            {
                my_str_write("(nil)");
            }
            else 
            {
                char * hex = my_hex( (long unsigned) p );                
                my_str_write("0x");
                my_str_write_reverse(hex);
                free(hex);
            }
        }
        else if (format[i-1]=='%' && format[i] == 's')
        {
            s = va_arg(ap, char *);
            my_str_write(s);          
        }
        else if (format[i-1]=='%' && format[i] == 'c')
        {
            c = (char) va_arg(ap, int);
            my_write(c);
        }
        else if (format[i] != '%')
        {
            my_write(format[i]); 
        }
    }    
    va_end(ap);

    return global_count;
}

// int main( int argc, const char* argv[] )
// {
//     int ac = 43;

// 	int apple = printf("%p!\n", &ac);
//     printf("\n%d\n", apple);

//     apple = my_printf("%p!\n", &ac);
//     printf("\n%d\n", apple);

// }