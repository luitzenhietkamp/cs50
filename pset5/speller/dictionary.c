// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Node struct
typedef struct _Node
{
    bool end_val;
    struct _Node *children[27];
} Node;

// Global variables
int db_size = 0;
Node *main_Node;
Node empty_node;

// Additional prototypes
void insert_substr(Node *curr_node, const char *substr);
bool read_word(FILE *file, char *word);
int alphabetical_index(const char *c);
bool search_tree(const Node *curr_node, const char *substr);
void destroy_structure(Node *curr_node);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    return search_tree(main_Node, word);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary_file)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary_file, "r");
    if (!file)
    {
        return false;
    }

    // Create a main noad
    main_Node = malloc(sizeof(Node));
    *main_Node = empty_node;

    // Read a word from dictionary and add to structure
    char word[LENGTH + 1];
    while (true)
    {
        if (read_word(file, word))
        {
            ++db_size;
            insert_substr(main_Node, word);
        }
        else
        {
            break;
        }
    }

    // Close file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return db_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Destroy the structure main_Node points to
    // ...then free the space
    destroy_structure(main_Node);
    free(main_Node);
    return true;
}

// Correctly inserts a substring in database at specified node
void insert_substr(Node *curr_node, const char *substr)
{
    // find the alphabetical index of the first char
    int m = alphabetical_index(&substr[0]);

    // if key is not in structure...
    if (!curr_node->children[m])
    {
        // ...add key to structure
        curr_node->children[m] = malloc(sizeof(Node));
        *curr_node->children[m] = empty_node;
    }

    // if substring is 1 character long no key needs to be added...
    // ...but tell node that it can be an end point
    if (strlen(substr) == 1)
    {
        curr_node->children[m]->end_val = true;
    }
    // if substring is longer, recursively add remainder of...
    // substring to structure
    else
    {
        insert_substr(curr_node->children[m], &substr[1]);
    }
}

// Reads a word from file and indicates whether word could be read
bool read_word(FILE *file, char *word)
{
    int index = 0;

    while (true)
    {
        // Store character at file pointer in c
        char c = fgetc(file);

        if (isalpha(c) || c == '\'')
        {
            // Append character to word
            word[index++] = c;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while ((c = fgetc(file)) != EOF && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }
        // We must have found a whole word
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';
            return true;
        }
        else
        {
            // indicate end of dictionary
            return false;
        }
    }
}

// Returns the array position where the character can be found
int alphabetical_index(const char *c)
{
    return *c == '\'' ? 26 : toupper(*c) - 'A';
}

// Search substring in tree
bool search_tree(const Node *curr_node, const char *substr)
{
    int m = alphabetical_index(&substr[0]);
    // If the alphabetical index can be found in the tree...
    if (curr_node->children[m] != NULL)
    {
        // ...check if the substring ends there...
        if (strlen(substr) == 1)
        {
            return curr_node->children[m]->end_val;
        }
        else
        {
            // ...or recursively search sub-substring in subtree
            return search_tree(curr_node->children[m], &substr[1]);
        }
    }
    // Key is not in the tree
    return false;
}

// Recursively destroy the structure and free resources
void destroy_structure(Node *curr_Node)
{
    for (int i = 0; i < 27; ++i)
    {
        if (curr_Node->children[i])
        {
            destroy_structure(curr_Node->children[i]);
            free(curr_Node->children[i]);
        }
    }
}