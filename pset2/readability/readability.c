#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>


int count_letters(string t);
int count_words(string t);
int count_sentences(string t);

int main(void)
{
    string text = get_string("Insert a text: ");

    int letters1 = count_letters(text);
    int words1 = count_words(text);
    int sentences1 = count_sentences(text);

    float index = (0.0588 * (letters1 / (words1 * 0.01))) - (0.296 * (sentences1 / (words1 * 0.01))) - 15.8;

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.f\n", index);
    }
}


int count_letters(string t)
{
    int letters = 0;

    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if ((t[i] >= 'A' && t[i] <= 'Z') || (t[i] >= 'a' && t[i] <= 'z'))
        {
            letters++;
        }
    }
    return letters;
}


int count_words(string t)
{
    int words = 0;

    for (int i = 0, n = strlen(t); i <= n; i++)
    {
        if (t[i] == 32 || t[i] == '\0')
        {
            words++;
        }
    }
    return words;
}


int count_sentences(string t)
{
    int sentences = 0;

    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (t[i] == '.' || t[i] == '!' || t[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}