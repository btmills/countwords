#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHBINS 977

#define bool char
#define true 1
#define false 0

typedef struct
{
	char* word;
	int count;
} Pair;

struct PairList
{
	int size;
	int len;
	Pair **pairs;
} counted;

//long comparisons;
//long words;

// djb2 hash algorithm
unsigned long hash(char *str)
{
	unsigned long hash = 5381;
	int c;

	while(c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

int lookup(char *word)
{
	int i, result = -1;

	for(i = 0; i < counted.len; i++)
	{
		//comparisons++;
		if(strcmp(word, counted.pairs[i]->word) == 0)
		{
			result = i;
			break;
		}
	}

	return result;
}

void count(char *word)
{
	int i, loc;
	loc = lookup(word);

	if(loc != -1) // Already in table
	{
		counted.pairs[loc]->count++;

		free(word);
	}
	else // Was not found
	{
		Pair *p;

		p = (Pair*)malloc(sizeof(Pair));
		p->word = word;
		p->count = 1;

		counted.len++;

		if(counted.size == 0)
		{
			counted.size = 32;
			counted.pairs = malloc(counted.size * sizeof(Pair*));
		}
		else if(counted.len > counted.size)
		{
			counted.size *= 2;
			counted.pairs = realloc(counted.pairs, counted.size * sizeof(Pair*));
		}

		counted.pairs[counted.len - 1] = p;
	}
	//words++;
}

bool isWordCharacter(int ch)
{
	if(('a' <= ch && ch <= 'z')
		|| ('A' <= ch && ch <= 'Z')
		|| ch == '\'')
		return true;
	return false;
}

int main()
{
	int ch, size, len;
	char* word;

	ch = getchar();
	while(ch != EOF)
	{
		if(isWordCharacter(ch))
		{
			len++; // Adding another character

			if(size == 0) // Beginning of new word
			{
				size = 4;
				word = malloc(size * sizeof(char));
			}
			else if(len >= size) // Need to allocate more memory for word
			{
				size *= 2;
				word = realloc(word, size * sizeof(char));
			}

			word[len - 1] = ch;
		}
		else // ch is not a word character
		{
			if(len > 0) // A word has been prepared
			{
				word[len] = '\0'; // Add null-terminator
				count(word);

				// Prepare for next word
				//free(word);
				//word = NULL;
				size = 0;
				len = 0;
			}
			// Else ignore ch
		}

		ch = getchar();
	}

	// Finish the last word if necessary
	if(len > 0) // A word has been prepared
	{
		word[len] = '\0'; // Add null-terminator
		count(word);

		// Prepare for next word
		//free(word);
		//word = NULL;
		//size = 0;
		//len = 0;
	}

	//printf("%d unique words counted:\n", counted.len);
	{
		int i;
		for(i = 0; i < counted.len; i++)
		{
			/*printf("%d: (\"%s\", %d), hash %ld\n", i + 1, counted.pairs[i]->word,
				counted.pairs[i]->count, hash(counted.pairs[i]->word) % HASHBINS);*/
			printf("%d: (\"%s\", %d)\n", i + 1, counted.pairs[i]->word,
				counted.pairs[i]->count);

			// Clean up
			free(counted.pairs[i]->word);
			free(counted.pairs[i]);
		}
	}

	//printf("A total of %ld string comparisons took place.\n", comparisons);
	//printf("%ld words were counted.\n", words);

	free(counted.pairs);

	return 0;
}
