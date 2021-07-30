#include <stdio.h>
#include <cs50.h>


int main(void)
{
    long n, i, j, k;
    int a, b, c, d, t1, t2;
    string s;

    n = get_long(" Credit Card number: ");


    b = 0;
    d = 0;
    s = "INVALID";

    for (i = 100; i < n * 10; i = i * 100)
    {
        a = 2 * ((n % i) - (n % (i / 10))) / (i / 10);
        if (a >= 10)
        {
            a = (a - (a % 10)) / 10 + (a % 10);
        }
        b = b + a;
    }

    for (i = 10; i < n * 10; i = i * 100)
    {
        c = ((n % i) - (n % (i / 10))) / (i / 10);
        if (c >= 10)
        {
            c = (c - (c % 10)) / 10 + (c % 10);
        }
        d = d + c;
    }



    if ((b + d) % 10 == 0)
    {
        for (k = 10; k <= n; k = k * 10)
        {
            j = k * 10;
        }

        t1 = ((n % j) - (n % (j / 10))) / (j / 10);
        t2 = ((n % j) - (n % (j / 100))) / (j / 100);

        if ((t1 == 4) && ((n >= 1000000000000000 && n < 10000000000000000) || (n >= 1000000000000 && n < 10000000000000)))
        {
            s = "VISA";
        }


        else if ((t2 == 34 || t2 == 37) && (n >= 100000000000000 && n < 1000000000000000))
        {
            s = "AMEX";
        }


        else if ((t2 >= 51 && t2 <= 55) && (n >= 1000000000000000 && n < 10000000000000000))
        {
            s = "MASTERCARD";
        }

        printf("%s\n", s);
    }

    else
    {
        printf("%s\n", s);
    }
}
