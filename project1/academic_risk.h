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

/* Returns 1 (and says so) when there are no students. */
int is_empty(struct Roster *r)
{
	if (r->count == 0) {
		printf("No student records.\n");
		return 1;
	}
	return 0;
}

void print_student(struct Student *s)
{
	printf("%-6d %-20s %-18s %6.2f\n", s->id, s->name, s->course, s->grade);
}

/* Prints every student by moving a pointer through the array. */
void display_students(struct Roster *r)
{
	struct Student *p;

	if (is_empty(r))
		return;
	printf("%-6s %-20s %-18s %6s\n", "ID", "Name", "Course", "Grade");
	for (p = r->list; p < r->list + r->count; p++)
		print_student(p);
	printf("Total records: %d\n", r->count);
}

/* Moves a pointer through the array. Returns the student, or NULL. */
struct Student *find_by_id(struct Roster *r, int id)
{
	struct Student *p;

	if (r->count == 0)
		return NULL;
	for (p = r->list; p < r->list + r->count; p++) {
		if (p->id == id)
			return p;
	}
	return NULL;
}

void add_student(struct Roster *r)
{
	struct Student s;
	struct Student *bigger;
	char line[32];

	s.id = read_number("Student id: ");
	if (s.id <= 0) {
		printf("Rejected: the id must be a number above 0.\n");
		return;
	}
	if (find_by_id(r, s.id) != NULL) {
		printf("Rejected: id %d already exists.\n", s.id);
		return;
	}
	read_line("Name: ", s.name, sizeof(s.name));
	read_line("Course: ", s.course, sizeof(s.course));
	if (strlen(s.name) == 0 || strlen(s.course) == 0) {
		printf("Rejected: name and course cannot be empty.\n");
		return;
	}
	read_line("Grade (0-100): ", line, sizeof(line));
	if (sscanf(line, "%f", &s.grade) != 1 || s.grade < 0 || s.grade > 100) {
		printf("Rejected: the grade must be from 0 to 100.\n");
		return;
	}

	/* Make the array one bigger and put the new student at the end. */
	bigger = realloc(r->list, (r->count + 1) * sizeof(struct Student));
	if (bigger == NULL) {
		printf("Out of memory: student not added.\n");
		return;	/* the old array is unchanged */
	}
	r->list = bigger;
	r->list[r->count] = s;
	r->count = r->count + 1;
	printf("Added %s. Records: %d\n", s.name, r->count);
}
