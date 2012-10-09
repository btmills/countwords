#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prime number. Works well empirically.
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

PairList *table[HASHBINS];

/// Interesting statistics
long comparisons;
long words;

// djb2 hash algorithm
unsigned long hash(char *str)
{
	unsigned long hash = 5381;
	int c;

	while(c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

Coord lookup(char* word)
{
	int i;
	Coord result;
	unsigned long hashVal;

	hashVal = hash(word);

	result.bin = hashVal % HASHBINS;
	result.index = -1; // Initial value of undefined

	if(table[result.bin])
	{
		for(i = 0; i < table[result.bin]->len; i++)
		{
			omparisons++; // Uncomment for performance tuning
			/* Considered storing and comparing hash values (unmodded) but no
			   performance gain was apparent given large enough HASHBINS */
			if(strcmp(word, table[result.bin]->pairs[i]->word) == 0)
			{
				result.index = i;
				break;
			}
		}
	}

	return result;
}

void count(char* word)
{
	int i;
	Coord addr;

	addr = lookup(word);

	if(addr.index != -1) // Already in table
	{
		table[addr.bin]->pairs[addr.index]->count++;
	}
	else // Was not found
	{
		// Create pair and copy word
		Pair *p = malloc(sizeof(Pair));
		p->word = malloc((strlen(word) + 1) * sizeof(char));
		strcpy(p->word, word);
		p->count = 1;

		// Create a hash table row if it does not exist
		if(table[addr.bin] == NULL)
		{
			table[addr.bin] = (PairList*)malloc(sizeof(PairList));
			table[addr.bin]->len = 0;
			table[addr.bin]->size = 0;
		}

		// Add word to hash table row
		table[addr.bin]->len++;

		if(table[addr.bin]->size == 0)
		{
			table[addr.bin]->size = 2;
			table[addr.bin]->pairs = malloc(table[addr.bin]->size * sizeof(Pair*));
		}
		else if(table[addr.bin]->len > table[addr.bin]->size)
		{
			table[addr.bin]->size *= 2;
			table[addr.bin]->pairs = realloc(table[addr.bin]->pairs, table[addr.bin]->size * sizeof(Pair*));
		}

		table[addr.bin]->pairs[table[addr.bin]->len - 1] = p;
	}

	words++;
}

inline bool isWordCharacter(char ch)
{
	if(('a' <= ch && ch <= 'z')
		|| ('A' <= ch && ch <= 'Z')
		|| ch == '\'')
		return true;
	return false;
}

int main()
{
	char ch;
	int size = 0, len = 0;
	char* word;

	ch = getchar();
	while(ch != EOF)
	{
		if(isWordCharacter(ch))
		{
			len++; // Adding another character

			if(size == 0) // Beginning of new word
			{
				size = 32;
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

				len = 0; // Prepare for next word
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
	}

	free(word);

	// Print words
	{
		int i, j;
		for(i = 0; i < HASHBINS; i++)
		{
			if(table[i])
			{
				for(j = 0; j < table[i]->len; j++)
				{
					//printf("(\"%s\", %d)\n", table[i]->pairs[j]->word, table[i]->pairs[j]->count);

					// Clean up
					free(table[i]->pairs[j]->word);
					free(table[i]->pairs[j]);
				}
			}

			free(table[i]);
		}
	}

	printf("A total of %ld string comparisons took place.\n", comparisons);
	printf("%ld words were counted.\n", words);

	return 0;
}
