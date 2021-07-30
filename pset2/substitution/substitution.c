#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>


string mixing(string text, string arg);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            if (!(isalpha(argv[1][i])))
            {
                printf("Key must contain 26 alhpabetic characters\n");
                return 1;
            }
            for (int j = 0; j < i; j++)
            {
                if (argv[1][j] == argv[1][i])
                {
                    printf("Key characters must be unique\n");
                    return 1;
                }
            }
            for (int j = 0; i < j && j < 26; j++)
            {
                if (argv[1][j] == argv[1][i])
                {
                    printf("Key characters must be unique\n");
                    return 1;
                }
            }
        }
    }

    string text1 = get_string("Text: ");
    string text2 = mixing(text1, argv[1]);

    printf("ciphertext: %s\n", text2);

    return 0;
}


string mixing(string text, string arg)
{
    // TODO: Compute and return score for string
    string LETTERS1 = "abcdefghijklmnopqrstuvwxyz";
    string LETTERS2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (islower(text[i]))
            {
                if (text[i] == LETTERS1[j])
                {
                    text[i] = tolower(arg[j]);
                    break;
                }
            }
            else
            {
                if (text[i] == LETTERS2[j])
                {
                    text[i] = toupper(arg[j]);
                    break;
                }
            }
        }
    }
    return text;
}