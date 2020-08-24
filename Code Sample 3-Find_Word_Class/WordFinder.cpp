#include "WordFinder.h"
#include <set>

WordFinder::WordFinder(const vector<string> &validDictionary) {
    for (const auto &it : validDictionary) {
        if (isValidWord(it))
            valid_words.insert(pair<string, bool>(it, 1));
    }
}

set<string> WordFinder::findWords(const vector<vector<char>> &grid) {
    set<string> ans;

    string potential_word = "";

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            findWordsHelper(i, j, potential_word, grid, ans);
            potential_word = "";
        }
    }

    cout << "Found Words: ";
    for (auto it : ans) {
        cout << it << " ";
    }
    return ans;
}

void WordFinder::printList(const vector<string> &wordlists) {
    cout << "Given Dictionary Values: ";
    for (auto &it : wordlists) {
        cout << it << " ";
    }
    cout << endl;
}

void WordFinder::printGrid(const vector<vector<char>> &grid) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

void WordFinder::findWordsHelper(int i, int j, string word,
        const vector<vector<char>> &grid, set<string> &ans) {
    visited[i][j] = 1;
    word += grid[i][j];
    if (word.length() >= 3 && (valid_words.find(word) != valid_words.end())) {
        ans.insert(word);
    }

    for (int x = i - 1; x <= i + 1; ++x) {
        for (int y = j - 1; y <= j + 1; ++y) {
            if (x == i && y == j) {
                continue;
            }
            if (isValidCoordinate(x, y))
                findWordsHelper(x, y, word, grid, ans);
        }
    }
    visited[i][j] = 0;
}

bool WordFinder::isValidWord(const string &word) {
    if (word.size() < 3)
        return false;
    for (int i = 0; i < word.size(); i++) {
        if (!(word[i] >= 'a' && word[i] <= 'z')) {
            return false;
        }
    }
    return true;
}

bool WordFinder::isValidCoordinate(int i, int j) {
    return (i >= 0 && i <= 3 && j >= 0 && j <= 3 && (visited[i][j] == 0));
}

