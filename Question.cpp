#include "Question.h"

Question::Question(string t, string q, int correctAnswer, string s, string e) {
    title = t;
    text = q;
    correct = correctAnswer;
    source = s;
    explanation = e;
}

bool Question::IsCorrect(int picked) {
    return picked == correct;
}