#ifndef QUESTION_H
#define QUESTION_H

#include <string>
using namespace std;

class Question {
public:
    string title;        // example: "1895 - Cinematographe"
    string text;         // the question
    int correct;         // 0 = A, 1 = B, 2 = C, 3 = D
    string source;       // citation
    string explanation;  // shown after answering

    Question(string t, string q, int correctAnswer, string s, string e);

    bool IsCorrect(int picked);
};

#endif