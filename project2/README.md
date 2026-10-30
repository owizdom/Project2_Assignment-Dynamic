# Project 2: Dynamic Text Mining and Linguistic Pattern Analyzer

## 1. Source code

```
gcc *.c -o text_analyzer
./text_analyzer
```

- [`text_analyzer.c`](text_analyzer.c): the main program. It holds the Darwin paragraph and `main`, which runs the menu through an array of function pointers and frees the memory at the end.
- [`text_analyzer.h`](text_analyzer.h): the structs and every other function. That covers reading a pasted paragraph into a buffer that doubles with `realloc`, and splitting the text into words with a pointer. It builds the word table with `strcmp` and `strcpy`, and has count, longest word, most frequent, display, sort and search. It also has the Lexical Diversity Score, the memory addresses and the summary.

## 2. Selected paragraph and source

Charles Darwin, *On the Origin of Species*, first edition (John Murray, London, 1859), closing paragraph, from Project Gutenberg eBook #1228. 219 words.

> It is interesting to contemplate an entangled bank, clothed with many
> plants of many kinds, with birds singing on the bushes, with various
> insects flitting about, and with worms crawling through the damp earth,
> and to reflect that these elaborately constructed forms, so different
> from each other, and dependent on each other in so complex a manner,
> have all been produced by laws acting around us. These laws, taken in
> the largest sense, being Growth with Reproduction; Inheritance which is
> almost implied by reproduction; Variability from the indirect and
> direct action of the external conditions of life, and from use and
> disuse; a Ratio of Increase so high as to lead to a Struggle for Life,
> and as a consequence to Natural Selection, entailing Divergence of
> Character and the Extinction of less-improved forms. Thus, from the war
> of nature, from famine and death, the most exalted object which we are
> capable of conceiving, namely, the production of the higher animals,
> directly follows. There is grandeur in this view of life, with its
> several powers, having been originally breathed into a few forms or
> into one; and that, whilst this planet has gone cycling on according to
> the fixed law of gravity, from so simple a beginning endless forms most
> beautiful and most wonderful have been, and are being, evolved.

Menu option 1 replaces it with any pasted paragraph.

## 3. Sample output

[`sample_run/output.txt`](sample_run/output.txt) is a real run, typed from [`sample_run/input.txt`](sample_run/input.txt). It shows both sorts, the longest and most frequent words, three searches, the score, the memory addresses, the summary, and two pastes.

## 4. Custom analysis: Lexical Diversity Score

LDS = distinct words / total words x 100. Below 40 is Low (many repeats), 40 to 69.9 is Medium, and 70 or more is High (mostly new words).

The word table holds each distinct word once, so the score is the number of entries divided by the total word count. A pointer then walks the table to count the words used only once.

Darwin's paragraph scores 135 / 219 x 100 = 61.6, which is Medium. 105 of its 135 words appear only once, and the repeats are joining words such as "and" (12), "of" (11) and "the" (11).

## 5. Punctuation, repeated words and growing input

- A word is a run of letters and digits. Any other character separates words.
- An apostrophe or hyphen stays inside a word when a letter follows it (`cat's`, `well-known`). Otherwise it separates.
- Words are stored in lowercase, so `The` and `the` are one word.
- Words longer than 39 characters are cut to 39. Curly quotes and accented letters separate words.

From the sample run:

```
Choice: 1
Paste a paragraph, then press Enter on an empty line:
The cat's toy -- the CAT'S toy!!   The well-known toy,
so  well-known... 'quoted' end-
pneumonoultramicroscopicsilicovolcanoconiosis is 45 letters long.

Read 153 characters. Buffer: 64 bytes at the start, doubled 2 times to 256 bytes.
Text loaded. Words: 18  Distinct: 12

Choice: 5
the             3    cat's           2    toy             3    well-known      2
so              1    quoted          1    end             1    pneumonoultramicroscopicsilicovolcanoco  1
is              1    45              1    letters         1    long            1
Distinct words: 12

Choice: 1
Paste a paragraph, then press Enter on an empty line:

No words were entered. The old text is kept.
```

Punctuation is dropped, `The` and `the` count as one word, the 45-letter word is cut to 39, and the buffer grew from 64 to 256 bytes.

## Author

Wisdom Okechukwu
