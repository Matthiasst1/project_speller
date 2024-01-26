# Project: Speller

#### Documentation of my project "Speller"

#### Programming language: C

## Motivation 

This project aims to find misspelled words in a text and present them in a resulting text file "results.txt". It was the final project after 6 weeks of C programming during the CS50 course at Harvard University. All the functions I wrote in C can be found in the file "dictionary.c". 
 
 As provided by the CS50 course, "speller.c" imports "dictionary.c". The program will use the text files in the "dictionaries" folder as a database of all existing words. It then searches the text files in the "texts" folder and saves the misspelled words as "results.txt". The "keys" folder contains all the results from the texts.

 ## Usage 

./speller dictionaries/large texts/lalaland.txt > results.txt 