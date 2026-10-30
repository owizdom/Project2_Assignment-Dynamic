# Project 1: Dynamic Academic Performance and Risk Analysis Engine

## 1. Source code

```
gcc *.c -o academic_risk_engine
./academic_risk_engine
```

- [`academic_risk_engine.c`](academic_risk_engine.c): the main program. It holds the 12 starting students and `main`, which runs the menu through an array of function pointers and frees the memory at the end.
- [`academic_risk.h`](academic_risk.h): the structs and every other function. That covers safe input with `fgets`, the student table, and add and remove, where the array grows and shrinks with `realloc`. It also has the three sorts with `qsort`, search with `strcmp`, the average, the top student, the Academic Risk Index and the summary.

## 2. Sample output

[`sample_run/output.txt`](sample_run/output.txt) is a real run, typed from [`sample_run/input.txt`](sample_run/input.txt). It shows adding, removing, the table, all three sorts, the average, the top student, both searches, the risk index and the summary.

## 3. Custom analysis: Academic Risk Index

The Academic Risk Index (ARI) shows how far below the pass mark of 50 a student is, so it is clear who needs help first.

ARI = 0 for a grade of 50 or more. Below 50, ARI = (50 - grade) x 2, rounded down and at least 1, so it runs from 0 to 100. Bands: 0 Safe, 1 to 39 Watch, 40 to 69 At risk, 70 to 100 Critical.

A pointer walks the students and prints each one's ARI and band, then the class mean ARI and the count in each band.

In the sample run the class mean ARI is 7.2, with Safe 8, Watch 3, At risk 1 and Critical 0. David Kariuki (grade 27, ARI 46) is the one student At risk, so he needs help first.

## 4. Adding and removing records

From the sample run, which starts with 12 students:

```
Choice: 1
Student id: 1001
Rejected: id 1001 already exists.

Choice: 1
Student id: 1013
Name: Ngozi Eze
Course: Physics
Grade (0-100): 150
Rejected: the grade must be from 0 to 100.

Choice: 1
Student id: 1013
Name: Ngozi Eze
Course: Physics
Grade (0-100): 64.5
Added Ngozi Eze. Records: 13

Choice: 7
Id to remove: 1006
Removed id 1006. Records: 12

Choice: 7
Id to remove: 9999
No student with that id.
```

## Author

Wisdom Okechukwu
