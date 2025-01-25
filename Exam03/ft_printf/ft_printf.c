#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

void ft_put_char(char c, int * count)
{
    write(1, &c , 1);
    (*count) ++;
}
int ft_strlen (char*s)
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    return(i);
}

void print_s(char* s, int *count)
{
    int i;

    i = 0;
    if (s == NULL)
    {
        print_s("(null)", count);
        return;
    }
    while (s[i] != '\0')
    {
        ft_put_char(s[i], count);
        i++;
    }
}

void put_nbr_base(long n, char* base, int* count)
{

    int factor = ft_strlen(base);
    if (n >= factor)
    {
        put_nbr_base(n / factor, base, count);
    }
        ft_put_char(base[n%factor], count);
}
void print_d(int n, int *count)
{
    char *base = "0123456789";
    if (n == INT_MIN)
    {
        print_s("-2147483648", count);
        return;
    }
    if (n < 0)
    {
        n = -n;
        ft_put_char('-', count);
    }
    put_nbr_base(n, base, count);
}

void print_x(unsigned int x, int *count)
{
    char * base = "0123456789abcdef";
    // if (x < 0)
    //     x = -x;
    put_nbr_base(x, base, count);
}


int ft_printf(char* s, ...)
{
    va_list params;
    int count;
    int i;
    int result;

    i = 0;
    count = 0;
    va_start(params, s);
    while(s[i] != '\0')
    {
        if (s[i] == '%' && s[i+1] != '\0')
        {
            if (s[i+1] == 's')
                print_s(va_arg(params, char*), &count);
            if (s[i+1] == 'x')
                print_x(va_arg(params, unsigned int), &count);
            if(s[i+1] == 'd')
                print_d(va_arg(params, int), &count);
            i = i+2;
        }
        else
        {
            write (1, &s[i], 1);
            count ++;
            i++;
        }
    }
    va_end(params);
    result = count;
    return(result);
}

// int main (void)
// {
//     int n = 2147483648;
//     int bytes;
//     int ft_bytes;
//     printf("---x----\n");
//     ft_bytes = ft_printf("FT: %x\n", n);
//     bytes = printf("PF: %x\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     printf("\n");

//     printf("---NULL----\n");
//     char * s = NULL;
//     ft_bytes = ft_printf("FT: %s\n", s);
//     bytes = printf("PF: %s\n", s);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     printf("\n");
// }

// int main(void)
// {
//     int bytes;
//     int ft_bytes;
//     printf("---S----\n");
//     ft_bytes = ft_printf("FT: Hola\n");
//     bytes = printf("PF: Hola\n");
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     printf("\n");
//     ft_bytes = ft_printf("FT:Hola %s\n", "mundo");
//     bytes = printf("PF:Hola %s\n", "mundo");
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     printf("---D----\n");
//     int n = 123;
//     ft_bytes = ft_printf("FT: %d\n", n);
//     bytes = printf("PF: %d\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = 0;
//     ft_bytes = ft_printf("FT: %d\n", n);
//     bytes = printf("PF: %d\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = -123;
//     ft_bytes = ft_printf("FT: %d\n", n);
//     bytes = printf("PF: %d\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = INT_MIN;
//     ft_bytes = ft_printf("FT: %d\n", n);
//     bytes = printf("PF: %d\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = INT_MAX;
//     ft_bytes = ft_printf("FT: %d\n", n);
//     bytes = printf("PF: %d\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     printf("\n");

//     printf("---X----\n");
//     n = 123;
//     ft_bytes = ft_printf("FT: %x\n", n);
//     bytes = printf("PF: %x\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = 0;
//     ft_bytes = ft_printf("FT: %x\n", n);
//     bytes = printf("PF: %x\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = -123;
//     ft_bytes = ft_printf("FT: %x\n", n);
//     bytes = printf("PF: %x\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = INT_MIN;
//     ft_bytes = ft_printf("FT: %x\n", n);
//     bytes = printf("PF: %x\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     n = INT_MAX;
//     ft_bytes = ft_printf("FT: %x\n", n);
//     bytes = printf("PF: %x\n", n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
//     printf("\n");

//     printf("----PRUEBA FINAL----\n");
//     char *s = "STRING";
//     n = 1234567;
//     ft_bytes = ft_printf("FT: la string es %s, el numero es %d y el hexa es %x\n", s, n, n);
//     bytes = printf("PF: la string es %s, el numero es %d y el hexa es %x\n", s, n, n);
//     printf("Resultado: bytes: %i, ft_bytes: %i\n", bytes, ft_bytes);
// } 