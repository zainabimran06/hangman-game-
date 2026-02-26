#include <string>
#include <iostream>
#include <ctime>
using namespace std;

#define MAX_GUESSES 7
#define MAX_ATTEMPTS 6 // To match drawing stages

// Hangman drawings
const string HANGMAN_STAGES[MAX_ATTEMPTS + 1] = {
R"(
     +---+
     |   |
         |
         |
         |
         |
    =========)",

R"(
     +---+
     |   |
     O   |
         |
         |
         |
    =========)",

R"(
     +---+
     |   |
     O   |
     |   |
         |
         |
    =========)",

R"(
     +---+
     |   |
     O   |
    /|   |
         |
         |
    =========)",

R"(
     +---+
     |   |
     O   |
    /|\  |
         |
         |
    =========)",

R"(
     +---+
     |   |
     O   |
    /|\  |
    /    |
         |
    =========)",

R"(
     +---+
     |   |
     O   |
    /|\  |
    / \  |
         |
    =========)"
};

//  Show clues Func
string getClueForShow(const string& showName) {
    if (showName == "Fargo") return " A small-town crime snowballs into a wild ride. Also the name of a North Dakota city.";
    if (showName == "Ozark") return " A financial advisor turns criminal in the lakeside wilderness.";
    if (showName == "House") return " Medical mysteries solved by a brilliant but grumpy doctor with a limp.";
    if (showName == "Girls") return " Millennials in New York navigating friendship, art, and identity.";
    if (showName == "Fringe") return " Government agents explore weird science and parallel universes.";
 else{
     " No clue available for this show.";
 }
}

//  Word bank
class WordBank {
private:
    static const int WORD_COUNT = 5;
    string words[WORD_COUNT] = {"Fargo", "Ozark", "House", "Girls", "Fringe"};

public:
    string getWordByIndex(int index) {
        if (index < WORD_COUNT) return words[index];
        return words[rand() % WORD_COUNT];
    }

    int getWordCount() const {
        return WORD_COUNT;
    }
};

//  Tracks guessed letters
class GuessTracker {
private:
    char guessedLetters[MAX_GUESSES];
    int guessedCount = 0;

public:
    bool alreadyGuessed(char letter) {
        for (int i = 0; i < guessedCount; i++)
            if (guessedLetters[i] == letter)
                return true;
        return false;
    }

    void addGuess(char letter) {
        guessedLetters[guessedCount++] = letter;
    }

    void displayGuessedLetters() {
        for (int i = 0; i < guessedCount; i++)
            cout << guessedLetters[i] << " ";
    }
};

//  Word state
class WordState {
private:
    string secretWord;
    string currentWord;

public:
    WordState(string word) : secretWord(word), currentWord(word.length(), '_') {}

    bool update(char letter) {
        bool correctGuess = false;
        for (int i = 0; i < secretWord.length(); i++) {
            if (tolower(secretWord[i]) == letter) {
                currentWord[i] = secretWord[i];
                correctGuess = true;
            }
        }
        return correctGuess;
    }

    bool isComplete() {
        return currentWord == secretWord;
    }

    void display() {
        cout << "Word: " << currentWord << endl;
    }

    string getSecretWord() {
        return secretWord;
    }
};

//  Displays Hangman art
class HangmanDisplay {
public:
    void draw(int attemptsLeft) {
        int index = MAX_ATTEMPTS - attemptsLeft;
        if (index >= 0 && index <= MAX_ATTEMPTS)
            cout << HANGMAN_STAGES[index] << endl;
    }
};

//  Game logic
class GameManager {
private:
    int attemptsLeft = MAX_ATTEMPTS;
    GuessTracker tracker;
    HangmanDisplay display;
    WordState* wordState;
    string secret;

public:
    GameManager(const string& chosenWord) {
        secret = chosenWord;
        wordState = new WordState(secret);
    }

    ~GameManager() {
        delete wordState;
    }

    void play() {
        cout << "\nWelcome to Hangman!\nCategory: Five-letter TV Shows\nAttempts: " << attemptsLeft << endl;
        cout << "Clue: " << getClueForShow(secret) << endl;

        while (attemptsLeft > 0) {
            wordState->display();
            cout << "Attempts left: " << attemptsLeft << "\nGuessed letters: ";
            tracker.displayGuessedLetters();
            cout << "\nGuess a letter: ";
            char guess;
            cin >> guess;
            guess = tolower(guess);

            if (!isalpha(guess)) {
                cout << "Please enter a valid alphabet.\n";
                continue;
            }

            if (tracker.alreadyGuessed(guess)) {
                cout << "You've already guessed that letter.\n";
                continue;
            }

            tracker.addGuess(guess);
            bool correct = wordState->update(guess);
            if (correct)
                cout << "Nice! That one is in there.\n";
            else {
                cout << "Oops! No match.\n";
                attemptsLeft--;
                display.draw(attemptsLeft);
            }

            if (wordState->isComplete()) {
                wordState->display();
                cout << " You nailed it! The show was: " << secret << endl;
                return;
            }
        }

        wordState->display();
        display.draw(attemptsLeft);
        cout << " Game over! The show was: " << secret << endl;
    }
};

//  Ask if user wants to continue
bool playAgain() {
    char choice;
    cout << "\nWould you like to play again with another show? (y/n): ";
    cin >> choice;
    return tolower(choice) == 'y';
}

// Main
int main() {
    WordBank bank;
    int gameCount = 0;

    do {
        string chosenWord = bank.getWordByIndex(gameCount % bank.getWordCount());
        GameManager game(chosenWord);
        game.play();
        gameCount++;
    } while (playAgain());

    cout << "\nThanks for playing! Stay tuned for more!" << endl;
    return 0;
}
