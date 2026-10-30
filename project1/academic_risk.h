#ifndef ACADEMIC_RISK_H
#define ACADEMIC_RISK_H

/* The structs and every function the program uses, except main. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_ID "993"
#define PASS_MARK 50

struct Student {
	int id;
	char name[50];
	char course[40];
	float grade;
};

/* All the students, kept in memory from malloc, and how many there are. */
struct Roster {
	struct Student *list;
	int count;
};

/* A menu option: a function that takes the roster. */
typedef void (*MenuAction)(struct Roster *r);

/* A sort rule for qsort: compares two students. */
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
