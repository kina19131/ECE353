#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "wc.h"

//bucket - to be stored in the hashtable
struct StorWord{
	char *key; 
	int val; 
};

//hashtable
struct wc{
	struct StorWord* words;
	long long int size;	
}; 


//Function to create a (word,freq) combination, storword, to add into the hash table
struct StorWord create_word(char* key, int val){
	struct StorWord word;
	word.key = (char*)malloc(70*sizeof(char));
	strcpy(word.key, key);
	word.val = val;
 	return word;	
}

// Function to create the index for hash table 
unsigned long long int hash_function(char *str){ 
    unsigned long long int hash = 5381; //need long long int to not get malloc() topsized for big files
    int c;

    while (0!=(c = *str++)){
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
}

//Function to do collision handling
void linear_probing(struct wc* wc, unsigned long long int index, struct StorWord inserting_word){ 
	index++;
	struct StorWord current_word = wc -> words[index];
	if (current_word.key == NULL){
		wc -> words[index] = inserting_word;
		return; 
	}

	while (current_word.key != NULL){ //until finds an empty bucket
		if (index > wc -> size){ //if the index is over the hashtable size, reset it to 0 
			index = 0;
		}
		if(strcmp(current_word.key, inserting_word.key) == 0){//found a bucket with the same word
			wc->words[index].val++; //only increment the value (freq)
			return;
		}
		index++;
		current_word = wc -> words[index]; 
	}	
	wc -> words[index] = inserting_word; //adding a word into a new bucket in the hash table
}

struct wc *wc_init(char *word_array, long size){
	struct wc *wc; 
	wc = (struct wc *)malloc(sizeof(struct wc));
	assert(wc); 

	wc -> size = size;  
	wc -> words = (struct StorWord*)malloc((size) * sizeof(struct StorWord)); 
		
	//initialize hashtable 
	for (int i = 0; i<wc->size; i++){
		wc -> words[i].key = NULL;
	}

	//extracting word by word
	long int i = 0; 
   	
	char* ExtWord = (char*)malloc(70*sizeof(char));
	ExtWord[0] = '\0';

	for (i=0; i<size; i++){
		//contains characte
		if (!isspace(word_array[i])){
			strncat(ExtWord, &(word_array[i]), 1); //adding char by char
		}
		
		//whitespace
		else{ 	
			if(strcmp(ExtWord,"")==0){ //handling if there was consecutive empty space - enter before a new paragraph 
				continue; 
			}
		
			//hash index
			unsigned long long int hash = hash_function(ExtWord); 
			unsigned long long int index = hash % (wc->size);

			//object of where in hashtable i am looking at 
			struct StorWord current_word = wc -> words[index]; 
 
			if (current_word.key == NULL){ //if key doesn't exist
				struct StorWord new_word = create_word(ExtWord, 1);	
				wc ->words[index] = new_word;
			}
		
			else{//key does exist in the place
				if(strcmp(ExtWord, current_word.key) == 0){ //if the word trying to add is already in 
					wc->words[index].val++; //only need to increment the frequency 
				}
				else{ //The adding word isn't the same as currently place word - COLLISION HANDLING
					struct StorWord inserting_word = create_word(ExtWord,1);
					linear_probing(wc, index, inserting_word);
				}	
			}		
			ExtWord[0] = '\0';
		}
	}
	free(ExtWord);
	return(wc);
}

//Function that prints out the output
void wc_output(struct wc *wc){
	long size = wc -> size; 

	for (int i = 0; i<size; i++){
		struct StorWord word = wc -> words[i];
		if (word.key != NULL){
			printf("%s", word.key);
			printf(":%d", word.val);
			printf("\n");
		}	
	}
}


//Function that frees malloced memories 
void wc_destroy(struct wc *wc){	
	for(int i = 0; i < wc->size; i++){
		free(wc->words[i].key);	
	}
	free(wc->words);
	free(wc);
}


