// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 26 + 26 + 26 + 1;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Define hash table pointer
    node *ptr = table[hash(word)];

    // While end of linked list is not found do
    while (ptr != NULL)
    {
        // If word is not found do
        if (strcasecmp(word, ptr->word) != 0)
        {
            // Move pointer to the next word
            ptr = ptr->next;
        }
        // If word is found return true
        else
        {
            return true;
        }
    }
    // End of while loop reached - no word found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Count letters in word
    int count = 0;
    while (word[count] != '\0')
    {
        count++;
    }

    // If word contains no letters return hash 28 * N
    if (count == 0)
    {
        return 28 * 26;
    }

    // If one letter from a to z return hash from 26 * N to 27 * N - 1
    else if (count == 1)
    {
        return 26 * 26 + toupper(word[0]) - 'A';
    }

    // If apostrophe after the first letter return hash from 27 * N to 28 * N - 1
    else if (word[1] == '\'')
    {
        return 27 * 26 + toupper(word[0]) - 'A';
    }

    // If two letters from aa to zz return hash from 0 until N * N - 1
    else
    {
        return (toupper(word[0]) - 'A') * 26 + toupper(word[1]) - 'A';
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary text file, do not forget to close
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Variables
    int index = 0;
    int hashtag = 0;
    int hashtag_previous = 0;
    node *list = NULL;
    node *n_new = NULL;
    char c = '\0';

    while (fread(&c, sizeof(char), 1, file))
    {
        // If no paragraph there must be a letter
        if (c != '\n')
        {
            if (index == 0)
            {
                // CREATE NODE
                n_new = malloc(sizeof(node));
                if (n_new == NULL)
                {
                    fclose(file);
                    return 1;
                }
                n_new->next = NULL;
            }

            // Fill up the word array within the node
            n_new->word[index] = c;

            // Update counter
            index++;
        }

        // If paragraph is found
        else if (n_new != NULL)
        {
            // End word with '\0' to declare string
            n_new->word[index] = '\0';

            // Figure out hashtag
            hashtag = hash(n_new->word);

            // If hashtag is different do
            if (hashtag != hashtag_previous)
            {
                // Assign hash table pointer
                table[hashtag_previous] = list;
            }

            // If hashtag is equal insert new node as the following
            else
            {
                // New node points to old list
                n_new->next = list;
            }

            // List points to n_new
            list = n_new;

            // Reset n_new
            n_new = NULL; // Stop ja doch müsste eig ok sein oder? list zeigt auf n_new (ein Schritt vorher)

            // Hash word for next loop
            hashtag_previous = hashtag;

            // Update counter
            index = 0;
        }
    }

    // Close very last list by assigning hash table pointer manually
    table[hashtag_previous] = list;

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Variables
    int count = 0;
    unsigned int sum = 0;

    // Pointer
    node *ptr = NULL;

    // Iterate through hash table and count words
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];

        while (ptr != NULL)
        {
            ptr = ptr->next;
            count++;
        }

        // Add counted words to sum
        sum += count;

        // Reset count
        count = 0;
    }

    return sum;
}

bool unload(void)
{
    // Iterate through hash table
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        node *temp = NULL;

        while (ptr != NULL)
        {
            //printf("%s\n\n", ptr->word);
            temp = ptr->next;
            free(ptr);
            ptr = temp;
        }
    }
    return true;
}