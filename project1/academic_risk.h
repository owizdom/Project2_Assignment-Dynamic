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

void remove_student(struct Roster *r)
{
	struct Student *target;
	struct Student *p;
	struct Student *smaller;
	int id;

	if (is_empty(r))
		return;
	id = read_number("Id to remove: ");
	target = find_by_id(r, id);
	if (target == NULL) {
		printf("No student with that id.\n");
		return;
	}

	/* Move every student after the removed one down one place. */
	for (p = target; p < r->list + r->count - 1; p++)
		*p = *(p + 1);
	r->count = r->count - 1;

	/* Make the array one smaller, or free it when it is empty. */
	if (r->count == 0) {
		free(r->list);
		r->list = NULL;
	} else {
		smaller = realloc(r->list, r->count * sizeof(struct Student));
		if (smaller != NULL)
			r->list = smaller;	/* if this fails, the old array still works */
	}
	printf("Removed id %d. Records: %d\n", id, r->count);
}

/* Sort rules for qsort. Each one returns a negative number when a goes first. */
int by_grade(const void *a, const void *b)
{
	const struct Student *x = a;
	const struct Student *y = b;

	if (x->grade > y->grade)
		return -1;	/* highest grade first */
	if (x->grade < y->grade)
		return 1;
	return 0;
}

int by_name(const void *a, const void *b)
{
	const struct Student *x = a;
	const struct Student *y = b;

	return strcmp(x->name, y->name);	/* alphabetical */
}

int by_id(const void *a, const void *b)
{
	const struct Student *x = a;
	const struct Student *y = b;

	return x->id - y->id;	/* smallest id first */
}

void sort_students(struct Roster *r)
{
	/* The user's choice picks which sort rule qsort uses. */
	CompareFunction rules[3] = {by_grade, by_name, by_id};
	int choice;

	if (is_empty(r))
		return;
	choice = read_number("Sort by 1) grade  2) name  3) id: ");
	if (choice < 1 || choice > 3) {
		printf("Invalid choice.\n");
		return;
	}
	qsort(r->list, r->count, sizeof(struct Student), rules[choice - 1]);
	display_students(r);
}

/* Finds a student by id (if a number is typed) or by full name. */
void search_student(struct Roster *r)
{
	char text[50];
	struct Student *p;
	int id;

	if (is_empty(r))
		return;
	read_line("Id or full name: ", text, sizeof(text));
	id = atoi(text);	/* 0 if the text is a name */

	for (p = r->list; p < r->list + r->count; p++) {
		if (p->id == id || strcmp(p->name, text) == 0) {
			print_student(p);
			return;
		}
	}
	printf("No student matches \"%s\".\n", text);
}

/* Adds up every grade by moving a pointer through the array. */
void calculate_average(struct Roster *r)
{
	struct Student *p;
	float total = 0;

	if (is_empty(r))
		return;
	for (p = r->list; p < r->list + r->count; p++)
		total = total + p->grade;
	printf("Average grade: %.2f\n", total / r->count);
}

void find_top_student(struct Roster *r)
{
	struct Student *p;
	struct Student *best;

	if (is_empty(r))
		return;
	best = r->list;
	for (p = r->list; p < r->list + r->count; p++) {
		if (p->grade > best->grade)
			best = p;
	}
	printf("Top student:\n");
	print_student(best);
}

/*
 * Academic Risk Index (ARI): how far a grade is below the pass mark,
 * from 0 to 100. ARI = (50 - grade) x 2, or 0 if the student passed.
 */
int risk_index(float grade)
{
	int ari;

	if (grade >= PASS_MARK)
		return 0;
	ari = (PASS_MARK - grade) * 2;
	if (ari == 0)
		ari = 1;	/* a fail just under 50 still counts as some risk */
	return ari;
}

char *risk_band(int ari)
{
	if (ari == 0)
		return "Safe";
	if (ari < 40)
		return "Watch";
	if (ari < 70)
		return "At risk";
	return "Critical";
}
