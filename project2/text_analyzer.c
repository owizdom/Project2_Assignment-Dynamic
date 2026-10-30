/*
 * Dynamic Text Mining and Linguistic Pattern Analyzer
 * Student ID: 993
 */
#include "text_analyzer.h"

/*
 * My paragraph: the closing paragraph of On the Origin of Species,
 * Charles Darwin, first edition (John Murray, London, 1859).
 * Copied from Project Gutenberg eBook #1228. 219 words.
 */
char default_text[] =
	"It is interesting to contemplate an entangled bank, clothed with many "
	"plants of many kinds, with birds singing on the bushes, with various "
	"insects flitting about, and with worms crawling through the damp earth, "
	"and to reflect that these elaborately constructed forms, so different "
	"from each other, and dependent on each other in so complex a manner, "
	"have all been produced by laws acting around us. These laws, taken in "
	"the largest sense, being Growth with Reproduction; Inheritance which is "
	"almost implied by reproduction; Variability from the indirect and "
	"direct action of the external conditions of life, and from use and "
	"disuse; a Ratio of Increase so high as to lead to a Struggle for Life, "
	"and as a consequence to Natural Selection, entailing Divergence of "
	"Character and the Extinction of less-improved forms. Thus, from the war "
	"of nature, from famine and death, the most exalted object which we are "
	"capable of conceiving, namely, the production of the higher animals, "
	"directly follows. There is grandeur in this view of life, with its "
	"several powers, having been originally breathed into a few forms or "
	"into one; and that, whilst this planet has gone cycling on according "
	"to the fixed law of gravity, from so simple a beginning endless forms "
	"most beautiful and most wonderful have been, and are being, evolved.";

int main(void)
{
	/* Menu choice 1 runs actions[0], choice 2 runs actions[1], and so on. */
	MenuAction actions[10] = {
		enter_paragraph, count_words, longest_word, most_frequent,
		display_word_statistics, sort_word_statistics, search_word,
		custom_analysis, memory_inspection, generate_text_summary
	};
	struct Text t = {NULL, NULL, 0, 0};
	char *buffer;
	int choice;

	printf("Student ID: %s\n", STUDENT_ID);
	printf("Project: Dynamic Text Mining and Linguistic Pattern Analyzer\n");
	printf("Source: On the Origin of Species, Charles Darwin (John Murray, 1859), closing paragraph\n");
	printf("Custom Analysis: Lexical Diversity Score\n");

	buffer = malloc(strlen(default_text) + 1);
	if (buffer == NULL) {
		printf("Out of memory.\n");
		return 1;
	}
	strcpy(buffer, default_text);
	t.buffer = buffer;
	if (build_word_stats(&t) == 0) {
		printf("Out of memory.\n");
		free(t.buffer);
		return 1;
	}
	printf("Paragraph loaded. Words: %d  Distinct: %d\n", t.total, t.distinct);

	while (1) {
		printf("\n1.Paste text  2.Count words  3.Longest word  4.Most frequent\n");
		printf("5.Word table  6.Sort  7.Search  8.Lexical diversity\n");
		printf("9.Memory addresses  10.Summary  11.Exit\n");
		choice = read_number("Choice: ");
		if (choice == 11 || feof(stdin))
			break;
		if (choice >= 1 && choice <= 10)
			actions[choice - 1](&t);
		else
			printf("Invalid choice. Enter 1 to 11.\n");
	}

	free(t.buffer);
	free(t.words);
	printf("Memory freed. Goodbye.\n");
	return 0;
}
