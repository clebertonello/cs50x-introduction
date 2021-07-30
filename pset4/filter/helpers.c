#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int w;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            w = ((image[i][j].rgbtGreen + image[i][j].rgbtBlue + image[i][j].rgbtRed) / 3.0) + 0.5;
            image[i][j].rgbtGreen = w;
            image[i][j].rgbtRed = w;
            image[i][j].rgbtBlue = w;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][width - j - 1] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    RGBTRIPLE colors[9];
    int cblue = 0;
    int cgreen = 0;
    int cred = 0;
    int z = 0;

    for (int i = 0; i < 9; i++)
    {
        colors[i].rgbtBlue = 0;
        colors[i].rgbtGreen = 0;
        colors[i].rgbtRed = 0;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            z = 0;
            cblue = 0;
            cgreen = 0;
            cred = 0;

            for (int k = -1; k <= 1; k++)
            {
                if ((i + k >= 0) && (i + k < height))
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if ((j + l >= 0) && (j + l < width))
                        {
                            colors[z] = image[i + k][j + l];
                            z++;
                        }
                    }
                }
            }

            for (int m = 0; m < z; m++)
            {
                cblue += colors[m].rgbtBlue;
                cgreen += colors[m].rgbtGreen;
                cred += colors[m].rgbtRed;
            }

            cblue = (cblue / (float) z) + 0.5;
            cgreen = (cgreen / (float) z) + 0.5;
            cred = (cred / (float) z) + 0.5;

            tmp[i][j].rgbtBlue = cblue;
            tmp[i][j].rgbtGreen = cgreen;
            tmp[i][j].rgbtRed = cred;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    typedef struct
    {
        int x;
        int y;
    }
    color;

    RGBTRIPLE tmp[height][width];
    color blue;
    color green;
    color red;
    int cblue = 0;
    int cgreen = 0;
    int cred = 0;
    const int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            blue.x = 0;
            blue.y = 0;
            green.x = 0;
            green.y = 0;
            red.x = 0;
            red.y = 0;

            for (int k = -1; k <= 1; k++)
            {
                if ((i + k >= 0) && (i + k < height))
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if ((j + l >= 0) && (j + l < width))
                        {
                            blue.x += image[i + k][j + l].rgbtBlue * GX[k + 1][l + 1];
                            green.x += image[i + k][j + l].rgbtGreen * GX[k + 1][l + 1];
                            red.x += image[i + k][j + l].rgbtRed * GX[k + 1][l + 1];

                            blue.y += image[i + k][j + l].rgbtBlue * GY[k + 1][l + 1];
                            green.y += image[i + k][j + l].rgbtGreen * GY[k + 1][l + 1];
                            red.y += image[i + k][j + l].rgbtRed * GY[k + 1][l + 1];
                        }
                    }
                }
            }

            cblue = sqrt(blue.x * blue.x + blue.y * blue.y) + 0.5;
            if (cblue > 255)
            {
                cblue = 255;
            }
            cgreen = sqrt(green.x * green.x + green.y * green.y) + 0.5;
            if (cgreen > 255)
            {
                cgreen = 255;
            }
            cred = sqrt(red.x * red.x + red.y * red.y) + 0.5;
            if (cred > 255)
            {
                cred = 255;
            }

            tmp[i][j].rgbtBlue = cblue;
            tmp[i][j].rgbtGreen = cgreen;
            tmp[i][j].rgbtRed = cred;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}
