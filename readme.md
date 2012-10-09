# Word Counter

Built as a much faster implementation of a program written for a class, this program counts occurrences of unique words in a text document. For example, in the sentence "the cow jumped over the moon" the word "the" occurred twice, while all other words occurred once.

## Brute-Force Implementation

This implementaiton stores the counted words in a list and searches linearly each time a word is counted. It's slow.

## Hashing Implementation

Using the djb2 hash algorithm, words are stored in a hash table. Performance improvements of up to 100x were found relative to the class's reference implementation, which also used a hash table.

## Building and Running

Build either the brute-force or hash files using gcc. Run by directing a text file to stdin as in `./count_hash < warandpeace.txt`.