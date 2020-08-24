/*
 * Apple.h
 *
 *  Created on: Aug 16, 2020
 *      Author: abhil
 */

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
using namespace std;

#ifndef WORD_FINDER_H_
#define WORD_FINDER_H_

/**
 * Finds all valid words from the user_defined dictionary inside a NxM grid
 */
class WordFinder {
public:
    /**
     * Takes in a vector of strings. Only valid words will be accepted. Valid words are 3
     * or more characters and are letters from the lowercase alphabet.
     */
    WordFinder(const vector<string> &validDictionary);

    /**
     * Finds all valid words inside the grid and returns the words.
     */
    set<string> findWords(const vector<vector<char>> &grid);
    void printList(const vector<string> &wordlists);
    void printGrid(const vector<vector<char>> &grid);
private:
    void findWordsHelper(int i, int j, string word,
            const vector<vector<char>> &grid, set<string> &ans);
    bool isValidWord(const string &word);
    bool isValidCoordinate(int i, int j);
    bool visited[4][4] = { { 0 } };
    map<string, bool> valid_words;
};

#endif /* WORD_FINDER_H_ */
