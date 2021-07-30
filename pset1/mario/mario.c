#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h, i, e, c;
    do
    {
        h = get_int("Height: ");
    }
    while (h <= 0 || h > 8);


    for (i = 1; i <= h; i++)
    {
        for (e = h - i - 1; e >= 0; e--)
        {
            printf(" ");
        }

        for (c = 1; c <= i; c++)
        {
            printf("#");
        }
        printf("  ");
        for (c = 1; c <= i; c++)
        {
            printf("#");
        }

        printf("\n");
    }

}