/**
 * Implements a dictionary's functionality.
 */

#define _GNU_SOURCE

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

int words_loaded = 0;

// creating a trie DS;
typedef struct node
{
    bool is_word;
    struct node *children[27];
}node;

node *root = NULL;

// adds a word into the trie
bool add_word(char *word)
{
    // initialize root
    if(root == NULL)
        root = (node *) malloc(sizeof(node));
    
    // referance node for traversal
    node *temp = root;
    
    // Adds the given word into the Trie
    for(int i=0, word_len=strlen(word)-1; i<word_len; i++)
    {
        // index 0 corresponds to apostophie, 1  to A, 2 with B, 3 with C ... 26 with Z.
        int index = (word[i] == '\'')? 0: toupper(word[i]) - '@';

        //if node at that index is null, initialize it
        if(temp->children[index] == NULL)
            temp->children[index] = (node *) malloc(sizeof(node));
        
        // iterate for next letter
        temp = temp->children[index];
    }
    temp->is_word = true;
    words_loaded++;
    return true;
}

// Free a node and its children from memory
void clear_memory(node *n)
{
    // if passed node is null simply return
    if(n == NULL)
        return;
    
    // recursively free all its child node
    for(int i=0; i<27; i++)
        if(n->children[i] != NULL)
            clear_memory(n->children[i]);
    
    // finally, free the node
    free(n);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // if no word loaded
    if(root == NULL)
        return false;
    
    node *temp = root;

    // Check each char in the word is into the Trie
    for(int i=0, word_len=strlen(word); i<word_len; i++)
    {
        // index 0 corresponds to apostophie, 1  to A, 2 with B, 3 with C ... 26 with Z.
        int index = (word[i] == '\'')? 0: toupper(word[i]) - '@';

        // if node is null, the word isn't in dictionary
        if(temp->children[index] == NULL)
            return false;
        
        // check for next character
        temp = temp->children[index];
    }
    
    // check if the iterated characters make a word
    return temp->is_word;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary
    FILE *inptr = fopen(dictionary, "r");

    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    // Reading line by line, ref: https://linux.die.net/man/3/getline
    char * line = NULL;
    size_t len = 0;
    size_t read;
    while ((read = getline(&line, &len, inptr)) != -1)
        add_word(line);
    
    fclose(inptr);
    if (line)
        free(line);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words_loaded;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    clear_memory(root);
    return true;
}
