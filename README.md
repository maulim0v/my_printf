# my_printf

## Description
The **my_printf()** function is very simple implementation of <stdio.h> library printf() function with argument options described below. The **my_printf()** function writes output to stdout (standard output).

The format for the **my_printf()** function is assumed to be ordinary characters and '%' character followed by one of the flags specified below. After the '%' character, one of the following should appear:

* d Signed decimal
* o Unsigned octal
* u Unsigned decimal
* x Unsigned hexadecimal
* c Unsigned char
* s String (pointer to array of character type), terminated by NULL terminator character '\0'
* p The void * pointer, printed in hexadecimal with "0x" in front.

## Used standard functions to achieve <stdio.h> library printf functionality:
```
write
malloc
free
va_start
va_arg
va_copy
va_end
```

## Return
The **my_printf()** returns number of charaters written to stdout (standard output stream).

## Run
```
gcc my_printf.c
./a.out
```
In order to test the **my_printf()** properly, do not forget to modify the main().
