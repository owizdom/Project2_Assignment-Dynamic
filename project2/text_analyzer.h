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
