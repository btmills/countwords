#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHBINS 9973

#define bool char
#define true 1
#define false 0

typedef struct
{
	char* word;
	int count;
} Pair;

typedef struct
{
	int size;
	int len;
	Pair **pairs;
} PairList;

typedef struct
{
	int bin;
	int index;
} Coord;

PairList **table;

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

Coord *lookup(char *word)
{
	int i;
	Coord *result;

	result = malloc(sizeof(Coord)); // Create new Coord
	if(result == NULL)
	{
		printf("Out of memory.\n");
		exit(1);
	}
	result->bin = hash(word) % HASHBINS;
	result->index = -1;

	if(table[result->bin])
	{
		for(i = 0; i < table[result->bin]->len; i++)
		{
			//comparisons++;
			if(strcmp(word, table[result->bin]->pairs[i]->word) == 0)
			{
				result->index = i;
				break;
			}
		}
	}

	return result;
}

void count(char *word)
{
	int i;
	Coord *coord;

	coord = lookup(word);

	if(coord->index != -1) // Already in table
	{
		table[coord->bin]->pairs[coord->index]->count++;

		free(word);
	}
	else // Was not found
	{
		Pair *p;

		p = (Pair*)malloc(sizeof(Pair));
		p->word = word;
		p->count = 1;

		if(table[coord->bin] == NULL)
		{
			table[coord->bin] = (PairList*)malloc(sizeof(PairList));
			table[coord->bin]->len = 0;
			table[coord->bin]->size = 0;
		}

		table[coord->bin]->len++;

		if(table[coord->bin]->size == 0)
		{
			table[coord->bin]->size = 32;
			table[coord->bin]->pairs = malloc(table[coord->bin]->size * sizeof(Pair*));
		}
		else if(table[coord->bin]->len > table[coord->bin]->size)
		{
			table[coord->bin]->size *= 2;
			table[coord->bin]->pairs = realloc(table[coord->bin]->pairs, table[coord->bin]->size * sizeof(Pair*));
		}

		table[coord->bin]->pairs[table[coord->bin]->len - 1] = p;
	}
	//words++;
}

inline bool isWordCharacter(int ch)
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

	table = malloc(HASHBINS * sizeof(PairList*));
	if(table == NULL)
	{
		printf("Could not allocate memory for hash table.\n");
		exit(1);
	}

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
				//free(word); // word is freed by count() if not needed
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

	// Print words
	{
		int i, j;
		for(i = 0; i < HASHBINS; i++)
		{
			if(table[i])
			{
				for(j = 0; j < table[i]->len; j++)
				{
					printf("(\"%s\", %d)\n", table[i]->pairs[j]->word, table[i]->pairs[j]->count);

					// Clean up
					free(table[i]->pairs[j]->word);
					free(table[i]->pairs[j]);
				}
			}

			free(table[i]);
		}
	}

	//printf("A total of %ld string comparisons took place.\n", comparisons);
	//printf("%ld words were counted.\n", words);

	free(table);

	return 0;
}
