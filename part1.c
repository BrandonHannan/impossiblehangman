#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#define Max_Word_Length 30

char **words_by_specific_length; //Stores all the words with the desired length from the dicitonary file into an array
int count_words = 0; //Is the length of words_by_specific_length
int max_words = 0; //Is the number of words of the longest row in the 2D array created by LargestCategoryOfWords
int max_index = 0; //Is the row index of the longest row in the 2D array created by LargestCategoryOfWords

void DictionaryFileToWordsBySpecificLength(char *fname, int length){
    FILE *input = fopen(fname, "r");
    if (input == NULL) {
        printf("Failed to open the dictionary file.\n");
        exit(1);
    }
    char word[Max_Word_Length];
    while (fscanf(input, "%s", word) == 1){
        int len = strlen(word);
        int signal = 0;
        //Determines if the word read from the dictionary file contains duplicate characters, if not then it is counted
        if (len == length){
            for (int i = 0; i<len; i++){
                for (int j = i + 1; j<len; j++){
                    if (word[i] == word[j]){
                        signal++;
                        break;
                    }
                }
                if (signal>0){
                    break;
                }
            }
            if (signal == 0){
                count_words++;
            }
        }
    }

    fclose(input);


    words_by_specific_length = (char **)malloc(count_words * sizeof(char*));
    input = fopen(fname, "r");
    if (input == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    int index = 0;
    //Adds the words that contain no duplicates into the words_by_specific_length
    while (fscanf(input, "%s", word) == 1){
        int len = strlen(word);
        int signal = 0;
        if (len == length){
            for (int i = 0; i<len; i++){
                for (int j = i + 1; j<len; j++){
                    if (word[i] == word[j]){
                        signal++;
                        break;
                    }
                }
                if (signal>0){
                    break;
                }
            }
            if (signal == 0){
                words_by_specific_length[index] = (char *)malloc(len * sizeof(char) + 1);
                strcpy(words_by_specific_length[index++], word);
            }
        }
    }
    fclose(input);
    
}

bool CharacterAlphaChecker(char character){
    if (isalpha(character)){
        if (islower(character)){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

bool CharacterDupeChecker(char character, char *used_letters, int length_of_used_letters){
    for (int i = 0; i<length_of_used_letters; i++){
        if (used_letters[i] == character){
            return true;
        }
    }
    return false;
}



char **LargerCategoryOfWords(char **array, int length_of_word, char character, int length_of_array){
    char ***seperated_words;
    int *counter = (int*)calloc(length_of_word+1, sizeof(int)); // An array that stores the counts of words at the specific index that matches the specific character
    seperated_words = (char ***)malloc((length_of_word+1) * sizeof(char **)); //A 2D array that stores the words, where i represents the index that the specific character is located and j is the counts 

    //Determines the count for each word dependent on the specific index of the character
    for (int i = 0; i<length_of_array; i++){
        int signal = 0;
        for (int j = 0; j<length_of_word; j++){
            if (array[i][j] == character){
                counter[j]++;
                signal++;
            }
        }
        if (signal == 0){
            counter[length_of_word]++;
        }
    }
    //Initialises the seperated_words array to the length of the counts and resets the counter array as an index
    for (int i = 0; i < length_of_word+1; i++) {
        seperated_words[i] = (char **)malloc(counter[i] * sizeof(char *));
        counter[i] = 0; 
    }
    //Places each word from the words_by_length array into their respective row in the seperated_words array
    for (int i = 0; i<length_of_array; i++){
        int signal = 0;
        for (int j = 0; j<length_of_word; j++){
            if (array[i][j] == character){
                seperated_words[j][counter[j]] = (char *)malloc((length_of_word + 1) * sizeof(char));
                strcpy(seperated_words[j][counter[j]], array[i]);
                signal++;
                counter[j]++;
            }
        }
        if (signal == 0){
            seperated_words[length_of_word][counter[length_of_word]] = (char *)malloc((length_of_word + 1) * sizeof(char));
            strcpy(seperated_words[length_of_word][counter[length_of_word]], array[i]);
            counter[length_of_word]++;
        }
    }
    //Determines which row in the seperated_words 2D array is the longest 
    max_words = 0;
    max_index = 0;
    for (int i = 0; i<length_of_word+1; i++){
        if (counter[i]>max_words){
            max_words = counter[i];
            max_index = i;
        }
    }

    //Creates an array that stores strings which are the words from the longest row in the seperated_words array
    char **row = (char **)malloc(counter[max_index] * sizeof(char *));
    for (int j = 0; j < counter[max_index]; j++) {
        row[j] = seperated_words[max_index][j];
    }   
    return row; 

}

void GuessSimulater(int len, int n, char character){
    char *letters_used = (char*)calloc(n,sizeof(char)); //A string that stores the characters used
    char *remaining_letters = (char*)calloc(len, sizeof(char));//A string that represents the characters at the specific indexes
    int number_of_guesses = 0;

    for (int i = 0; i<len; i++){
        remaining_letters[i] = '_';
    }

    while (1){
        for (int j = 0; j<len; j++){
            printf("%c", remaining_letters[j]);
        }
        printf(" Enter a letter: ");
        scanf("%c", &character);
        while(getchar() != '\n');
        if (!CharacterAlphaChecker(character)){
            printf("%c is not a letter\n", character);
        }
        else{
            break;
        }
    }
    letters_used[number_of_guesses] = character;

    char **row;
    row = LargerCategoryOfWords(words_by_specific_length, len, character, count_words);
    //Checks whether the character is at the specific index for all words in the greatest row of words and updates the ____
    if (max_index == len){
        for (int i = 0; i<len; i++){
            remaining_letters[i] = '_';
        }
    }
    else{
        for (int i = 0; i<len; i++){
            if (i == max_index){
                remaining_letters[i] = character;
            }
            else{
                remaining_letters[i] = '_';
            }
        }
    }

    for (number_of_guesses = 1; number_of_guesses<n; number_of_guesses++){
        printf("Guess %d/%d, Words Left %d, Letters used = %s\n", number_of_guesses, n, max_words, letters_used);
        while (1){
            for (int j = 0; j<len; j++){
                printf("%c", remaining_letters[j]);
            }
            printf(" Enter a letter: ");
            scanf("%c", &character);
            while(getchar() != '\n');
            if (!CharacterAlphaChecker(character)){
                printf("%c is not a letter\n", character);
            }
            else if (CharacterDupeChecker(character, letters_used, n)){
                printf("%c already used\n", character);
            }
            else{
                break;
            }
        }
        letters_used[number_of_guesses] = character;
        row = LargerCategoryOfWords(row, len, character, max_words);
        if (max_index != len){
            remaining_letters[max_index] = character;
        }
        if (max_words == 1){
            break;
        }
    }
    if (max_words != 1){
        printf("Guess %d/%d, Words Left %d, Letters used = %s\n", number_of_guesses, n, max_words, letters_used);
        printf("Word was %s", row[0]);
    }
    else{
        printf("Guess %d/%d, Words Left %d, Letters used = %s\n", number_of_guesses, n, max_words, letters_used);
        printf("Congratulations You Guessed The Word, %s", row[0]);
    }
}

int main(int argc, char *argv[]){
    char *file = argv[1];
    int length = atoi(argv[2]);
    int guesses = atoi(argv[3]);
    
    DictionaryFileToWordsBySpecificLength(file, length);

    GuessSimulater(length, guesses, 'c');

    for (int i = 0; i<count_words; i++){
        free(words_by_specific_length[i]);
    }

    free(words_by_specific_length);
    return 0;
}
