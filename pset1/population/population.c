#include <cs50.h>
#include <stdio.h>


int main(void)
{
    // TODO: Prompt for start size
    int s;
    do
    {
        s = get_int("Population start size: ");
    }
    while (s < 9);


    // TODO: Prompt for end size
    int e;
    do
    {
        e = get_int("Population end size: ");
    }
    while (e < s);


    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    if (s != e)
    {
        do
        {
            s = s + (s / 3) - (s / 4);
            year++;
        }
        while (s < e);
    }


    // TODO: Print number of years

    printf("Years: %i\n", year);
}


