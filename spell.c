
// Created by Julio C Nunez on 2019-09-28.
//
// Assistance and guidance from:
// - NYU Cyber Fellows Cohort of 2021
// - Professor Kevin Gallagher
// - Ben Halperin Github


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

bool check_word(const char* word, hashmap_t hashtable[]) {
    int word_length = strlen(word);
    char lower_word[LENGTH+1];

    // Convert word to lowercase to accurately compare to hash table.
    for (int i = 0; i < word_length; i++)
    {
        // If character is uppercase, make it lowercase.
        if(isupper(word[i]))
        {
            lower_word[i] = tolower(word[i]) ;
        }
            // Otherwise it's already lowercase or it's not a letter.
        else
        {
            lower_word[i] = word[i];
        }
    }
    // Add null character to end of char array.
    lower_word[word_length] = '\0';
    // Use hash function to find correct "bucket" to place word.
    int bucket = hash_function(lower_word);
    // Set cursor to first node in bucket.
    node* cursor = hashtable[bucket];
    // Until the end of the linked list is reached (cursor == NULL),
    // compare each word stored in each node to lower_word.  If they're
    // the same, then the word is in the dictionary and is not mispelled.
    // Otherwise, it is spelled incorrectly.
    while (cursor != NULL)
    {
        if (strcmp(lower_word, cursor->word) == 0)
        {
            // If lowercase'd word is the same as another in the bucket,
            // it's a match and return true.
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}



bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
    for(int i = 0; i < HASH_SIZE; i++){ // Loop through the hash table and set all values to NULL.
        hashtable[i] = NULL;
    }

    // Opening files now.
    // Create a FILE* and call it dict_file
    FILE* dict;
    dict = fopen(dictionary_file, "r"); // Open the dictionary file to read it.

    if(dict == NULL)
    {
        return false; // Well, nothing inside of it. Might as well.
    }

    char word_buffer[LENGTH+1]; // Gonna need a buffer when we loop the file. Not in the pseudocode.
    // Might not need it but will use it for now and see how it works.

    while (fscanf(dict, "%45s", word_buffer) > 0) // While we're not at the EOF.
    {
        // Create new_node, allocate the memory to keep the code safe and not to break it.
        node* new_node = malloc(sizeof(node)); // Needs the same amount of memory - can't create a new node otherwise. Might break.

        // Set new_node's next to NULL
        new_node->next = NULL;

        // Lower-case all the words that are read in.
        int str_length = strlen(word_buffer);
        for(int i = 0; i < str_length; i++){
            word_buffer[i] = tolower(word_buffer[i]);
        }

        // Use strcopy to make new_node->word equal to the buffer.
        strcpy(new_node->word, word_buffer);



        int bucket = hash_function(new_node->word);

        if (hashtable[bucket] == NULL)
        {
            hashtable[bucket] = new_node;
        }

        else{
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
    }
    fclose(dict);
    return true; // To return a value to the function.
}


int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]){
    int num_misspelled = 0;

    // To make a buffer to stop any issues from arising with a buffer overflow while
    // reading the file.

    char word_buffer[LENGTH+1];

    if(fp == NULL){
        // Exit the program if a file isn't read in or it has nothing.
        return 0;
    }

    while (fscanf(fp, "%45s", word_buffer) > 0) {
        node* new_node = malloc(sizeof(node));

        new_node->next = NULL;

        strcpy(new_node->word, word_buffer);



        if (ispunct(word_buffer[0])) {
            memmove(word_buffer, word_buffer + 1, strlen(word_buffer));
        }

        if (ispunct(word_buffer[strlen(word_buffer) - 1])) {
            word_buffer[strlen(word_buffer) - 1] = '\0';
        }


//        for(int i=0; i < strlen(word_buffer); i++){
//            if(ispunct(word_buffer[i])){
//                misspelled[num_misspelled] = malloc(strlen(word_buffer) + 1);
//                misspelled[num_misspelled] = strcpy(misspelled[num_misspelled], word_buffer);
//                num_misspelled++;
//            }
//        }



        if (check_word(word_buffer, hashtable) == false) {
            misspelled[num_misspelled] = malloc(strlen(word_buffer) + 1);
            misspelled[num_misspelled] = strcpy(misspelled[num_misspelled], word_buffer);
            num_misspelled++;
            }
        }
    return num_misspelled;
}

