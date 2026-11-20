// ==========================================================
// Calculating Empires Millionaire
// A "Who Wants to Be a Millionaire" style quiz about early cinema
// technology from the Calculating Empires diagram.
//
// SOURCES:
// - Class example.cpp (raylib window, DrawText, game loop pattern)
// - raylib 5.5 cheatsheet: https://www.raylib.com/cheatsheet/raylib_cheatsheet_v5.5.pdf
// - raylib examples: https://www.raylib.com/examples.html
// - raylib colors: https://www.raylib.com/examples/shapes/loader.html?name=shapes_colors_palette
// - Crawford & Joler, "Calculating Empires": https://calculatingempires.net/
// - Game logic code written with help from Claude (Anthropic AI), edited by our team.
// - Question sources are cited inside each Question below.
// ==========================================================

#include <iostream>
#include <vector>
#include <string>
#include "raylib.h"
#include "Question.h"
using namespace std;

// VECTOR FOR QUESTIONS
// Question(title, question, correct answer (0=A..3=D), source, explanation)
vector<Question> questions = {
    // QUESTION 1
    Question(
        "1895 - Cinematographe",
        "What made the Lumiere brothers' Cinematographe different from Edison's Kinetoscope?",
        0,
        "National Science and Media Museum (2009), The Lumiere Brothers: Pioneers of Cinema and Colour Photography",
        "The Cinematographe could project films for an audience, while the Kinetoscope was a peephole viewer."
    ),
    // QUESTION 2
    Question(
        "1891 - Kinetograph",
        "What was the main purpose of the Kinetograph developed in Edison's laboratory?",
        1,
        "Alice Askins (2018), Mechanical Eye: Early History Motion Pictures",
        "The Kinetograph was the camera that recorded moving images on film."
    ),
    // QUESTION 3
    Question(
        "1891 - Kinetoscope",
        "How did people originally watch films using the Kinetoscope?",
        2,
        "Richard Brown (2012), A New Look at Old History - The Kinetoscope: Fraud and Market Development in Britain in 1895",
        "The Kinetoscope was a cabinet viewed through a peephole, one person at a time."
    ),
    // QUESTION 4
    Question(
        "1895 - Kinetophone",
        "What was the Kinetophone designed to combine?",
        1,
        "Jane M. Gaines (2023), Antonia Dickson: The Kineto-Phonograph and the Telephony of the Future",
        "The Kinetophone tried to join moving pictures with recorded sound."
    ),
    // QUESTION 5
    Question(
        "1895 - Lumiere Brothers",
        "What happened during the Lumiere brothers' public screening in Paris on December 28, 1895?",
        1,
        "HISTORY.com Editors (2009), First Commercial Movie Screened",
        "They showed short motion pictures to a paying audience."
    ),
    // QUESTION 6
    Question(
        "1895 - Early Film Shorts",
        "What do early Lumiere films showing workers leaving factories tell us about cinema?",
        1,
        "Pamela Hutchinson (2016), A Window on Infinity: Rediscovering the Short Films of the Lumiere Brothers",
        "Early films could document real people, work, and everyday life."
    ),
    // QUESTION 7
    Question(
        "1891-1895 - Development of Cinema",
        "What was one important difference between Edison's early film system and the Lumiere brothers' Cinematographe?",
        0,
        "Peter Domankiewicz (2021), In the Beginning: Cinema's Murky Origin Story",
        "Edison used individual peephole viewers, while the Cinematographe projected films to a group."
    )
};

// VECTOR FOR CHOICES
// choices[0] belongs to questions[0], choices[1] to questions[1], and so on
vector<vector<string>> choices = {
    // QUESTION 1 CHOICES
    {
        "It allowed films to be projected for an audience.",
        "It only recorded sound.",
        "It allowed one person to watch through a peephole.",
        "It was used only for animation."
    },
    // QUESTION 2 CHOICES
    {
        "To project movies onto large screens.",
        "To record moving images on film.",
        "To record voices and music.",
        "To create computer animations."
    },
    // QUESTION 3 CHOICES
    {
        "On a large cinema screen.",
        "On a television.",
        "Through a peephole, usually one person at a time.",
        "Through a smartphone."
    },
    // QUESTION 4 CHOICES
    {
        "Film and photography.",
        "Moving pictures and recorded sound.",
        "Animation and painting.",
        "Film and radio broadcasting."
    },
    // QUESTION 5 CHOICES
    {
        "They introduced the first talking movie.",
        "They presented short motion pictures to a paying audience.",
        "They introduced color television.",
        "They created the first animated feature film."
    },
    // QUESTION 6 CHOICES
    {
        "Films only showed fictional stories.",
        "Films could document real people, work, and everyday life.",
        "Films were only scientific experiments.",
        "Films were created entirely by computers."
    },
    // QUESTION 7 CHOICES
    {
        "Edison's system used individual peephole viewers, while the Cinematographe could project films to an audience.",
        "Both systems were designed only for recording sound.",
        "Edison's system was used only for animation.",
        "The Cinematographe could not record moving images."
    }
};

// DrawWrapped: draws long text on several lines.
// Written with help from Claude (Anthropic AI), edited by me (Rafiu). on Friday 20th at 12:00 a.m.
// raylib DrawText (raylib 5.5 cheatsheet).
void DrawWrapped(string text, int x, int y, int maxChars, int fontSize, Color color) {
    while ((int)text.length() > maxChars) {
        size_t cut = text.rfind(' ', maxChars);
        if (cut == string::npos) cut = maxChars;
        DrawText(text.substr(0, cut).c_str(), x, y, fontSize, color);
        text = text.substr(cut + 1);
        y = y + fontSize + 4;
    }
    DrawText(text.c_str(), x, y, fontSize, color);
}

int main() {
       // TERMINAL PART: intro and player name (like example.cpp)
    string name;
    cout << "Welcome to Calculating Empires Millionaire!" << endl;
    cout << "What is your name?" << endl;
    cin >> name;
    cout << "Good luck, " << name << "! Look at the game window." << endl;

    // InitWindow(width, height, title): opens the raylib window
    InitWindow(1000, 800, "Calculating Empires Millionaire");
    // SetTargetFPS(fps): limits the loop to 30 frames per second
    SetTargetFPS(30);

    // prize ladder: prizes[0] is for question 1, and so on (7 questions)
    vector<int> prizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    int total = questions.size();

    int current = 0;     // index of the question we are on
    int money = 0;       // prize won so far
    int gameState = 0;   // 0 = playing, 1 = lost, 2 = won
    int picked = -1;     // which button was clicked (-1 = none yet)

    // Rectangle {x, y, width, height}: the 4 answer buttons (A, B, C, D)
    Rectangle buttons[4] = {
        {100, 300, 380, 100}, {520, 300, 380, 100},
        {100, 420, 380, 100}, {520, 420, 380, 100}
    };

    // Game loop: repeats every frame until the window closes
    // WindowShouldClose(): true when the user presses ESC or clicks X
    while (!WindowShouldClose()) {

        Question q = questions[current];            // the current question
        vector<string> labels = choices[current];   // its 4 answer choices
        Vector2 mouse = GetMousePosition();         // mouse x and y

        // ---- Buttons logic to be inserted here (papa) ----

        // Drawing logic
        BeginDrawing();
        ClearBackground(DARKBLUE);

        DrawText(TextFormat("Question %d of %d", current + 1, total), 100, 40, 28, WHITE);
        DrawText(TextFormat("Prize: $%d", money), 700, 40, 28, GOLD);

        if (gameState == 0) {
            DrawText(q.title.c_str(), 100, 100, 24, GOLD);
            DrawWrapped(q.text, 100, 140, 70, 24, WHITE);

            for (int i = 0; i < 4; i++) {
                Color c = BLUE;   // normal button color
                // hover: lighter color when the mouse is over the button
                if (picked == -1 && CheckCollisionPointRec(mouse, buttons[i])) c = SKYBLUE;
                // after answering: right answer green, wrong pick red
                if (picked != -1) {
                    if (i == q.correct) c = GREEN;
                    else if (i == picked) c = RED;
                }
                DrawRectangleRec(buttons[i], c);
                DrawWrapped(labels[i], buttons[i].x + 15, buttons[i].y + 12, 36, 20, WHITE);
            }

            if (picked != -1) {
                DrawWrapped(q.explanation, 100, 560, 80, 22, YELLOW);
                DrawWrapped("Source: " + q.source, 100, 650, 100, 16, LIGHTGRAY);
                DrawText("Press SPACE for next", 100, 740, 24, WHITE);
            }
        }

        if (gameState == 1) {
            DrawText("GAME OVER", 350, 300, 60, RED);
            DrawText(TextFormat("You leave with $%d", money), 350, 400, 30, WHITE);
        }
        if (gameState == 2) {
            DrawText("YOU WON!", 350, 300, 60, GREEN);
            DrawText(TextFormat("Total: $%d", money), 350, 400, 30, WHITE);
        }

        EndDrawing();
    }

    // CloseWindow(): closes the raylib window and frees its resources
    CloseWindow();
    return 0;
}