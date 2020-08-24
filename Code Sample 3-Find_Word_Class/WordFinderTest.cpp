//============================================================================
// Name        : Apple.cpp
// Author      : Abhi M
//============================================================================

#include "WordFinder.h"
using namespace std;

void testGeneralCases() {
    cout << "Running TEST 1: General Cases" << endl;
    vector<vector<char>> grid = {
            { 'r', 'a', 'e', 'l' },
            { 'm', 'o', 'f', 's' },
            { 't', 'e', 'o', 'k' },
            { 'n', 'a', 't', 'i' }
    };
    vector<string> wordlists = { "atom", "roam", "meat", "apple", "rotate",
            "ram", "dsffsdfr54", "rae", "leaf", "leafs", "lear", "nation",
            "tool", "ra" };
    set<string> expected = { "atom", "leaf", "leafs", "lear", "meat", "rae",
            "ram", "roam", "rotate" };
    WordFinder wordFinder(wordlists);

    wordFinder.printGrid(grid);
    wordFinder.printList(wordlists);
    if (expected == wordFinder.findWords(grid))
        cout << endl << "Test 1 PASSED: Expected Values are generated" << endl;
    else
        cout << endl << "Test 1 FAILED: Expected Values are not generated"
                << endl;
}

void testEdgeCases() {
    cout << "Running TEST 2: Edge Cases" << endl;
    vector<vector<char>> grid = {
            { 'a', 'b', 's', 'e' },
            { 'n', 't', 'm', 'i' },
            { 'n', 'd', 'e', 'd' },
            { 'n', 'e', 's', 's' }
    };
    //validWords rename, add .h file with documentation
    vector<string> wordlists = { "annness", "ates", "annnedtbsmessdie",
            "absemdness" };
    set<string> expected =
            { "annness", "ates", "annnedtbsmessdie", "absemdness" };
    WordFinder wordFinder(wordlists);

    wordFinder.printGrid(grid);
    wordFinder.printList(wordlists);
    if (expected == wordFinder.findWords(grid))
        cout << endl << "Test 2 PASSED: Expected Values are generated" << endl;
    else
        cout << endl << "Test 2 FAILED: Expected Values are not generated"
                << endl;
}

void testInvalidCases() {
    cout << "Running TEST 3: Invalid Cases" << endl;
    vector<vector<char>> grid = {
            { 't', 'i', 'm', 'a' },
            { 'w', 'w', 'w', 'p' },
            { 'a', 'a', 'e', 'p' },
            { 'a', 'a', 'a', 'l' }
    };
    vector<string> wordlists = { "23@#", "1", "InvalidString123", "apple", "",
            "a", "aa", "timapple" };
    set<string> expected = { "apple", "timapple" };
    WordFinder wordFinder(wordlists);

    wordFinder.printGrid(grid);
    wordFinder.printList(wordlists);
    if (expected == wordFinder.findWords(grid))
        cout << endl << "Test 3 PASSED: Expected Values are generated" << endl;
    else
        cout << endl << "Test 3 FAILED: Expected Values are not generated"
                << endl;
}

int main() {
    testGeneralCases();
    cout << endl;
    testEdgeCases();
    cout << endl;
    testInvalidCases();
    return 0;
}
