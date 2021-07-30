#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool is_cycle(int w, int l);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];
        // array with candidate_count number of values

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    for (int i = 0; i < candidate_count; i++)
    {
        printf("[");
        for (int j = 0; j < candidate_count; j++)
        {
            printf(" %i ", preferences[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

    add_pairs();
    sort_pairs();

    for (int i = 0; i < pair_count; i++)
    {
        printf("Winner: %i\n", pairs[i].winner);
        printf("Loser: %i\n", pairs[i].loser);
        printf("\n");
    }

    lock_pairs();

    for (int i = 0; i < candidate_count; i++)
    {
        printf("[");
        for (int j = 0; j < candidate_count; j++)
        {
            printf(" %i ", locked[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int z, r;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (ranks[j] == i)
            {
                z = j;
            }
        }
        if (z + 1 < candidate_count)
        {
            for (int k = z + 1; k < candidate_count; k++)
            {
                r = ranks[k];
                preferences[i][r]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int z, w = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j != i)
            {
                z = preferences[i][j] - preferences[i][i];
                w = preferences[j][i] - preferences[j][j];
                if (z > w)
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
            }
        }
    }
    // update pair_count
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int pair_s[pair_count];
    int t = -1;
    int swap = 0;

    for (int i = 0; i < pair_count; i++)
    {
        pair_s[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }

    do
    {
        swap = 0;
        for (int j = 0; j < pair_count - 1; j++)
        {
            if (pair_s[j] < pair_s[j + 1])
            {
                pair_s[t] = pair_s[j];
                pair_s[j] = pair_s[j + 1];
                pair_s[j + 1] = pair_s[t];
                pairs[t].winner = pairs[j].winner;
                pairs[t].loser = pairs[j].loser;
                pairs[j].winner = pairs[j + 1].winner;
                pairs[j].loser = pairs[j + 1].loser;
                pairs[j + 1].winner = pairs[t].winner;
                pairs[j + 1].loser = pairs[t].loser;
                swap++;
            }
        }
    }
    while (swap > 0);

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // I did some research in the cs50 stackexchange for this part

    for (int i = 0; i < pair_count; i++)
    {
        if (is_cycle(pairs[i].winner, pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    int w;
    string winners[candidate_count];
    int winners_count = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        w = 0;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                w++;
                if (w == candidate_count)
                {
                    winners[winners_count] = candidates[i];
                    winners_count++;
                }
            }
        }
    }

    for (int i = 0; i < winners_count; i++)
    {
        printf("%s\n", winners[i]);
    }

    return;
}

bool is_cycle(int w, int l)
{
    if (w == l)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[l][i] == true)
        {
            if (i == w)
            {
                return true;
            }
            else if (is_cycle(w, i) == true)
            {
                return true;
            }
        }
    }
    return false;
}