#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
typedef struct list {
	char word[30];
	int index;
}L;
int numberOfWords = 0;
int validateWord(char insertWord[30]) // a function to check if the word only contains lower cases
{
	for (int i = 0;i < strlen(insertWord);i++)
	{
		if (insertWord[i] < 'a' && insertWord[i] > 'z')
			return 0;
	}
	return 1;
}
void showHeader() // it displays the header of the interface
{
	printf("/");
	for (int i = 0;i < 40;i++)
		printf("*");
	printf("/\n");
	printf("# Game : Hangman v1.0\n");
	printf("# Author : Alexandru-Florin BELENGEANU\n");
	printf("/");
	for (int i = 0;i < 40;i++)
		printf("*");
	printf("/\n\n");
}
void welcome() // some welcome message
{
	showHeader();
	printf("Hello there, dear guest!\n");
	printf("In this game of Hangman, the computer will choose a random word from a given list of possible words.\nThe chosen word will be the correct answer. After that, your mission is to guess the word, by guessing one letter at a time. \nOnce you guess a letter that fits in the word, all the places in the given string will be filled with that specific letter. \nThe game ends if you have guessed every letter in the word, or when you reach the limit of 7 tries.\n\n**note: A word should only contain lower case letters from the english alphabet!\n\n");
	printf("Now, let's get started!\n");
	system("pause");
}
void readWords(L *words, int *n) // a function to check if there are any words in the list from previous played games, if yes, it adds them again to the list
{
	FILE *f;
	f = fopen("words.in", "rt");
	if (f == NULL)
		printf("Error: The input file could not be open.");
	else {
		while (!feof(f)) {
			(*n)++;
			if (fscanf(f, "%s", (words + *n)->word) != 1) {
				(*n)--;
				break;
			}
			fscanf(f, "%d", &(words + *n)->index);
			numberOfWords++;
		}
		fclose(f);
	}
}
void addWord(L* words, int *n) // adds a word to the list
{
	char insertWord[30];
	printf("Insert the word you would like to add to the list: ");
	scanf("%s", insertWord);
	if (validateWord(insertWord) == 0) {
		printf("The word you tried to add is not valid.\nPlease remember, the words should only contain lower case letters from the english alphabet.\n");
		system("pause");
	}
	else {
		(*n)++;
		strcpy((words + *n)->word, insertWord);
		(words + *n)->index = ++numberOfWords;
	}
}
void deleteWord(L* words, int *n) // a function to delete a specific word from the list
{
	int k = 0;
	char deleteWord[30];
	printf("Insert the word you would like to delete from the list: ");
	scanf("%s", deleteWord);
	for (int i = 0;i <= *n;i++)
	{
		if (strcmp((words + i)->word, deleteWord) == 0) {
			k++;
			for (int j = i;j <= (*n - 1);j++) {
				*(words + j) = *(words + j + 1);
				(words + j)->index -= 1;
			}
		}
	}
	*n -= k;
	numberOfWords -= k;
}
void saveList(L *words, int n) // it saves the list after the player adds/removes some word, in the words.in file
{
	int i;
	FILE *f;
	f = fopen("words.in", "wt");
	if (f == NULL)
		printf("Error: The input file could not be open.");
	else {
		for (i = 0; i <= n; i++)
			fprintf(f, "%s %d\n", (words + i)->word, (words + i)->index);
		fclose(f);
	}
}
void showWordsList(L *words, int n) // a function to show all the words from the list at the time of verification
{
	for (int i = 0;i <= n;i++) {
		printf("%d.  %s\n", (words + i)->index, (words + i)->word);
	}
}
int letterFill(char letter, char randWord[30], char randWordCopy[30]) // a function to check if the letter given by the player fills in the secret word
{
	int match = 0;
	for (int i = 0;i < strlen(randWord);i++)
	{
		if (randWord[i] == letter)
		{
			return -1; // if the letter was already guessed before, return -1
		}
		else if (randWordCopy[i] == letter)
		{
			randWord[i] = letter; // if the letter fills in the word, increment 'match' variabile and replace the '_' character with the given letter
			match++;
		}
	}
	return match; // if match=0, that means the letter does not fill in the secret word
}
void startGame(L* words, int n) // the actual game function
{
	int MAX_TRIES = 7, wrongGuesses=0, match;
	char randWord[30], randWordCopy[30], letter;
	srand(time(0));
	int randIndex = (rand() % numberOfWords)+1; // generates a random index
	for (int i = 0;i <= n;i++)
	{
		if ((words + i)->index == randIndex)
		{
			strcpy(randWord, (words + i)->word); // save the word placed at the random index in 'randWord' variable
			strcpy(randWordCopy, (words + i)->word); // save a copy of the word so we can verify later if the given letters fill in there
			break;
		}
	}
	for (int i = 1;i < strlen(randWord) - 1;i++) {
		if ((randWord[i] != randWord[0]) && (randWord[i] != randWord[strlen(randWord)-1]))
			randWord[i] = '_';
	}
	do {
		system("cls");
		showHeader();
		printf("You got %d tries left.\n\n\t", MAX_TRIES - wrongGuesses);
		for (int i = 0;i < strlen(randWord);i++) {
			putchar(randWord[i]);
			printf(" ");
		}
		if (strcmp(randWord, randWordCopy) == 0) {
			printf("\n\n    Congratulations!! You won this time :)\n");
			system("pause");
			break;
		}
		else {
			printf("\n\nChoose a letter: ");
			scanf(" %c", &letter);
			getchar();
			match = letterFill(letter,randWord,randWordCopy);
			if (match > 0) {
				printf("You have guessed this one right!! Good job :D\n");
				system("pause");
			}
			else if (match == -1) {
				printf("The secret word already contains the letter you have entered.. Try again with another one!\n");
				system("pause");
			}
			else {
				wrongGuesses++;
				printf("The secret word does not contain the letter you have entered.. Try again !\n");
				system("pause");
			}
		}
	} while (wrongGuesses < MAX_TRIES);
	if (wrongGuesses == MAX_TRIES) {
		printf("You lost, Game OVER!! You can play again if you want..");
		system("pause");
	}
}
void showMenu(L* words, int n) // it  displays the game menu, where the user can choose between play, show the list of words or add/delete some word
{
	int opt;
	do {
		system("cls");
		showHeader();
		printf("1. Start the game\n");
		printf("2. Add words to the list\n");
		printf("3. Delete words from the list\n");
		printf("4. Show all the words in the list\n");
		printf("0. Exit\n");
		printf("Insert your option here: ");
		scanf("%d", &opt);
		if (opt < 0 || opt > 4) {
			printf("Your option should be an integer between 0-4!!\n");
			system("pause");
		}
		else {
			switch (opt) {
			case 1:
				if (numberOfWords == 0) {
					printf("Please add some words to your list!\n");
					system("pause");
				}
				else startGame(words, n);
				break;

			case 2:
				addWord(words, &n);
				saveList(words, n);
				break;

			case 3:
				if (numberOfWords == 0) {
					printf("The list is currently empty, there is nothing you can delete.\n");
					system("pause");
				}
				else {
					deleteWord(words, &n);
					saveList(words, n);
				}
				break;
			case 4:
				if (numberOfWords == 0) {
					printf("The list of words is empty!\n");
					system("pause");
				}
				else {
					printf("\n-- Currently, the list contains the following words --\n");
					showWordsList(words, n);
					system("pause");
				}
				break;

			case 0:
				exit(0);
				break;
			}
		}
	} while (1);
}

int main()
{
	L words[50];
	int n = -1;
	welcome();
	readWords(words, &n);
	showMenu(words, n);
	system("pause");
	return 0;
}
