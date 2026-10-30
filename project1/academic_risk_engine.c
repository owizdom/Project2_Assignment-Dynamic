/*
 * Dynamic Academic Performance and Risk Analysis Engine
 * Student ID: 993
 */
#include "academic_risk.h"

/* My dataset: 12 students to start with. */
struct Student seed[] = {
	{1001, "Amara Nwosu", "Data Structures", 78.5},
	{1002, "Kwame Mensah", "Calculus", 42.0},
	{1003, "Aisha Bello", "Data Structures", 91.0},
	{1004, "Jean Habimana", "Linear Algebra", 55.5},
	{1005, "Fatou Diallo", "Calculus", 67.0},
	{1006, "Tunde Bakare", "Programming in C", 12.0},
	{1007, "Grace Uwase", "Programming in C", 84.0},
	{1008, "Samuel Osei", "Linear Algebra", 38.5},
	{1009, "Zainab Yusuf", "Data Structures", 73.0},
	{1010, "Eric Mugisha", "Calculus", 49.5},
	{1011, "Halima Abdi", "Programming in C", 60.0},
	{1012, "David Kariuki", "Linear Algebra", 27.0}
};

int main(void)
{
	/* Menu choice 1 runs actions[0], choice 2 runs actions[1], and so on. */
	MenuAction actions[9] = {
		add_student, display_students, sort_students,
		calculate_average, find_top_student, search_student,
		remove_student, custom_analysis, generate_summary_report
	};
	struct Roster r;
	int choice;

	r.count = 12;
	r.list = malloc(r.count * sizeof(struct Student));
	if (r.list == NULL) {
		printf("Out of memory.\n");
		return 1;
	}
	memcpy(r.list, seed, r.count * sizeof(struct Student));

	printf("Student ID: %s\n", STUDENT_ID);
	printf("Project: Dynamic Academic Performance and Risk Analysis Engine\n");
	printf("Custom Analysis: Academic Risk Index\n");

	while (1) {
		printf("\n1.Add  2.Display  3.Sort  4.Average  5.Top student\n");
		printf("6.Search  7.Remove  8.Risk index  9.Summary  10.Exit\n");
		choice = read_number("Choice: ");
		if (choice == 10 || feof(stdin))
			break;
		if (choice >= 1 && choice <= 9)
			actions[choice - 1](&r);
		else
			printf("Invalid choice. Enter 1 to 10.\n");
	}

	free(r.list);
	printf("Memory freed. Goodbye.\n");
	return 0;
}
