#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// global variables

uint8_t BYTE;
uint8_t data[512];

// prototypes
void read(FILE *file, int z);

// main function
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    int z = 0;

    while (fread(&data, sizeof(BYTE), 512, file))
    {
        if (data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff && ((data[3] & 0xf0) == 0xe0))
        {
            fseek(file, -512, SEEK_CUR);
            read(file, z);
            z++;

        }
    }
}

void read(FILE *file, int z)
{
    int y = 0;

    char *filename = malloc(10000);
    if (filename == NULL)
    {
        return;
    }
    sprintf(filename, "%03i.jpg", z);

    FILE *output = fopen(filename, "w");

    while (fread(&data, sizeof(BYTE), 512, file))
    {
        if (data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff && ((data[3] & 0xf0) == 0xe0))
        {
            y++;
        }

        if (y < 2)
        {
            fwrite(&data, sizeof(BYTE), 512, output);
        }
        else
        {
            fclose(output);
            free(filename);
            fseek(file, -512, SEEK_CUR);
            z++;
            read(file, z);
            return;
        }
    }
    return;
}