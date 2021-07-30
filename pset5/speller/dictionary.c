// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

int count_words = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 2000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char *w = malloc(strlen(word) + 1);
    if (w == NULL)
    {
        free(w);
        return 0;
    }

    strcpy(w, word);

    for(int i = 0; i < strlen(word); i++)
    {
        w[i] = tolower(word[i]);
    }

    int index = hash(w);

    for (node *tmp = table[index]; tmp != NULL; tmp = tmp -> next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            free(w);
            return true;
        }
    }

    free(w);

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
// source: https://stackoverflow.com/questions/7666509/hash-function-for-string - Comment by Gabriel Staples.
//unsigned hash(char *s)
//{
    unsigned hashval;

    for (hashval = 0; *word != '\0'; word++)
    {
        hashval = *word + 31 * hashval;
    }
    hashval %= N;
    return hashval;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    char word_d[LENGTH + 1];
    node *w = NULL;
    int index;

    while (fscanf(f, "%s", word_d) != EOF)
    {
        w = malloc(sizeof(node));
        if (w == NULL)
        {
            free(w);
            return NULL;
        }

        index = hash(word_d);

        if (table[index] == NULL)
        {
            strcpy(w -> word, word_d);
            w -> next = NULL;
            table[index] = w;
            count_words++;
        }

        else
        {
            strcpy(w -> word, word_d);
            w -> next = table[index];
            table[index] = w;
            count_words++;
        }
    }

    fclose(f);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
//  source: notes from lecture 5
    node *tmp = NULL;

    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
        // does nothing if pointer == null. ok!
        free(tmp);
    }

    return true;
}
