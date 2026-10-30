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

