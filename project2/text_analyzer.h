#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

/* The structs and every function the program uses, except main. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STUDENT_ID "993"
#define MAX_WORD 40	/* 39 letters plus the '\0' at the end */

/* One distinct word and how many times it appears. */
struct WordStat {
	char name[MAX_WORD];
	int count;
};

/* The paragraph and its word table, both kept in memory from malloc. */
struct Text {
	char *buffer;
	struct WordStat *words;
	int distinct;	/* entries in words */
	int total;	/* every word in the text, repeats included */
};

/* A menu option: a function that takes the text. */
typedef void (*MenuAction)(struct Text *t);

/* A sort rule for qsort: compares two word entries. */
typedef int (*CompareFunction)(const void *a, const void *b);

/*
 * Shows a prompt and reads one line of text.
 * fgets stops at size - 1 characters, so the text never overflows.
 */
void read_line(const char *prompt, char *text, int size)
{
	int length;
	int c;

	printf("%s", prompt);
	fflush(stdout);	/* show the prompt now, before waiting for input */
	if (fgets(text, size, stdin) == NULL) {
		text[0] = '\0';		/* no more input */
		return;
	}
	length = strlen(text);
	if (length > 0 && text[length - 1] == '\n') {
		text[length - 1] = '\0';	/* remove the Enter key */
	} else {
		/* The line was too long: skip the rest of it. */
		c = getchar();
		while (c != '\n' && c != EOF)
			c = getchar();
	}
}

/* Reads a whole number. Returns -1 if the user did not type one. */
int read_number(const char *prompt)
{
	char line[32];
	int number;

	read_line(prompt, line, sizeof(line));
	if (sscanf(line, "%d", &number) != 1)
		return -1;
	return number;
}

/*
 * Reads a paragraph of any length, up to an empty line or the end of input.
 * The buffer starts at 64 bytes and doubles with realloc each time it is full.
 * Returns the text, or NULL if nothing was typed or memory ran out.
 */
char *read_paragraph(void)
{
	int size = 64;
	int length = 0;
	int grew = 0;
	int previous = '\n';
	int c;
	char *buffer;
	char *bigger;

	buffer = malloc(size);
	if (buffer == NULL)
		return NULL;

	c = getchar();
	while (c != EOF && !(c == '\n' && previous == '\n')) {
		if (length == size - 1) {
			bigger = realloc(buffer, size * 2);
			if (bigger == NULL) {
				printf("Out of memory.\n");
				free(buffer);
				return NULL;
			}
			buffer = bigger;
			size = size * 2;
			grew = grew + 1;
		}
		buffer[length] = c;
		length = length + 1;
		previous = c;
		c = getchar();
	}
	buffer[length] = '\0';

	if (length == 0) {
		free(buffer);
		return NULL;
	}
	printf("Read %d characters. Buffer: 64 bytes at the start, doubled %d times to %d bytes.\n", length, grew, size);
	return buffer;
}

/* Letters and digits are the only characters a word is made of. */
int is_word_char(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

/*
 * Moves a pointer to the next word and copies it into word, in lowercase.
 * An apostrophe or hyphen is kept only when a letter follows it (don't, less-improved).
 * Letters after the 39th are skipped, so word can never overflow.
 * Returns a pointer just past the word, or NULL when no word is left.
 */
char *next_word(char *p, char *word)
{
	int length = 0;

	while (*p != '\0' && !is_word_char(*p))
		p++;	/* skip spaces and punctuation */
	if (*p == '\0')
		return NULL;

	while (is_word_char(*p) || ((*p == '\'' || *p == '-') && is_word_char(*(p + 1)))) {
		if (length < MAX_WORD - 1) {
			word[length] = tolower(*p);
			length = length + 1;
		}
		p++;
	}
	word[length] = '\0';
	return p;
}

/* Moves a pointer through the word table. Returns the entry, or NULL. */
struct WordStat *find_word(struct Text *t, char *word)
{
	struct WordStat *p;

	for (p = t->words; p < t->words + t->distinct; p++) {
		if (strcmp(p->name, word) == 0)
			return p;
	}
	return NULL;
}

/* Reads every word in the text and counts how often each one appears. */
int build_word_stats(struct Text *t)
{
	char word[MAX_WORD];
	struct WordStat *found;
	struct WordStat *bigger;
	char *p;

	free(t->words);
	t->words = NULL;
	t->distinct = 0;
	t->total = 0;

	p = next_word(t->buffer, word);
	while (p != NULL) {
		t->total = t->total + 1;
		found = find_word(t, word);
		if (found != NULL) {
			found->count = found->count + 1;
		} else {
			/* A new word: make the table one bigger and put it at the end. */
			bigger = realloc(t->words, (t->distinct + 1) * sizeof(struct WordStat));
			if (bigger == NULL) {
				free(t->words);
				t->words = NULL;
				t->distinct = 0;
				t->total = 0;
				return 0;
			}
			t->words = bigger;
			strcpy(t->words[t->distinct].name, word);	/* word is at most 39 letters */
			t->words[t->distinct].count = 1;
			t->distinct = t->distinct + 1;
		}
		p = next_word(p, word);
	}
	return 1;
}

/* Returns 1 (and says so) when the text has no words. */
int is_empty(struct Text *t)
{
	if (t->total == 0) {
		printf("No words to analyze.\n");
		return 1;
	}
	return 0;
}

void enter_paragraph(struct Text *t)
{
	char word[MAX_WORD];
	char *buffer;

	printf("Paste a paragraph, then press Enter on an empty line:\n");
	buffer = read_paragraph();
	if (buffer == NULL || next_word(buffer, word) == NULL) {
		printf("No words were entered. The old text is kept.\n");
		free(buffer);
		return;
	}
	free(t->buffer);	/* the old paragraph is replaced */
	t->buffer = buffer;
	if (build_word_stats(t) == 0) {
		printf("Out of memory: the word table is empty.\n");
		return;
	}
	printf("Text loaded. Words: %d  Distinct: %d\n", t->total, t->distinct);
}

void count_words(struct Text *t)
{
	printf("Total words: %d\n", t->total);
}

/* The longest word. On a tie, the one first in the alphabet. */
void longest_word(struct Text *t)
{
	struct WordStat *p;
	struct WordStat *best;

	if (is_empty(t))
		return;
	best = t->words;
	for (p = t->words; p < t->words + t->distinct; p++) {
		if (strlen(p->name) > strlen(best->name))
			best = p;
		else if (strlen(p->name) == strlen(best->name) && strcmp(p->name, best->name) < 0)
			best = p;
	}
	printf("Longest word: %s (%d characters)\n", best->name, (int)strlen(best->name));
}

/* The most frequent word. On a tie, the one first in the alphabet. */
void most_frequent(struct Text *t)
{
	struct WordStat *p;
	struct WordStat *best;

	if (is_empty(t))
		return;
	best = t->words;
	for (p = t->words; p < t->words + t->distinct; p++) {
		if (p->count > best->count)
			best = p;
		else if (p->count == best->count && strcmp(p->name, best->name) < 0)
			best = p;
	}
	printf("Most frequent word: %s (count %d)\n", best->name, best->count);
}

/* Prints every distinct word and its count, 4 to a row. */
void display_word_statistics(struct Text *t)
{
	struct WordStat *p;
	int column = 0;

	if (is_empty(t))
		return;
	for (p = t->words; p < t->words + t->distinct; p++) {
		if (column > 0)
			printf("    ");
		printf("%-14s%3d", p->name, p->count);
		column = column + 1;
		if (column == 4) {
			printf("\n");
			column = 0;
		}
	}
	if (column != 0)
		printf("\n");
	printf("Distinct words: %d\n", t->distinct);
}
