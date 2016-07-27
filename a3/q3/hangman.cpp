#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <fstream>
#include <vector>

void print (const std::vector<std::pair<char, bool> >& word, const std::string& word_string) {
    int i = 0;
    for (auto it = word.begin(); it != word.end(); it++) {
        if (it->second) {
            std::cout << word_string[i];
        }
        else {
            std::cout << '-';
        }
        i++;
    }
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
    int seed = 0;
    std::string word_file = "";
    std::vector<std::string> valid_words;
    if (argc < 2) {
        std::cout << "Error: No input file specified" << std::endl;
        return 1;
    }
    else if (argc > 2) {
        seed = atoi(argv[2]);
    }
    if (argc > 1) {
        word_file = argv[1];
    }

    // load the file
    std::ifstream fs(word_file);
    std::ofstream  ofs("gamewords");
    if (fs.fail()) {
        std::cout << "Error: Could not open file \"" << word_file << "\"." << std::endl;
        return 1;
    }
    while (!fs.eof()) {
        std::string temp;
        fs >> temp;
        bool check = true;
        if (temp.length() >= 6) {
            // check if temp has any invalid characters
            for (unsigned int i = 0; i < temp.length() && check; i++) {
                if ((temp[i] < 'a' || temp[i] > 'z') && (temp[i] < 'A' || temp[i] > 'Z')) {
                    check = false;
                }
            }
            if (check) {
                valid_words.push_back(temp);
                ofs << temp << std::endl;
            }
        }
    }

    if (valid_words.size() < 1) {
        std::cout << "Error: Pool of game words is empty." << std::endl;
        return 1;
    }
    
    fs.close();
    ofs.close();

    static std::mt19937 rng(seed);
    while (true) {
        //pick the random game word from the collection of valid words
        std::string word_string = valid_words.at(rng() % valid_words.size());

        // create a map of the characters in the picked 
        std::vector<std::pair<char, bool> > game_word;
        std::string lower_case = word_string;
        std::transform (lower_case.begin(), lower_case.end(), lower_case.begin(), ::tolower);
        // go through the word and populate the map
        for (int i = 0; i < lower_case.size(); i++) {
            game_word.push_back(std::pair<char, bool>(lower_case[i], false));
        }

        std::string used_letters = "";
        int correct_guesses = 0;
        int lives = 5;

        while (correct_guesses < word_string.size() && lives > 0) {
            // print the game board
            std::cout << "Word: ";
            print(game_word, word_string);
            std::cout << "Letters used:" << used_letters << std::endl;
            if (lives > 1) {
                std::cout << "You have " << lives << " lives left." << std::endl;
            }
            else {
                std::cout << "You have " << lives << " life left." << std::endl;
            }


            std::cout << "Next guess: ";
            std::string guess, lower_guess;
            std::cin >> guess;
            // convert to lower case
            lower_guess = guess;
            std::transform(lower_guess.begin(), lower_guess.end(), lower_guess.begin(), ::tolower);

            // check if the whole word is a guess
            if (guess.length() > 1) {
                if (lower_guess.compare(lower_case) == 0) {
                    std::cout << "You WIN!" << std::endl;
                    break;
                }
                else {
                    lives = 0;
                }
            }
            else {  // for single character guesses
                // check if guess has already been used
                if (used_letters.find(lower_guess) != std::string::npos) {
                    std::cout << "You already guessed letter \"" << lower_guess << "\"." << std::endl;
                }
                else {
                    auto iter = std::find(game_word.begin(), game_word.end(), std::pair<char, bool>(lower_guess[0], false));
                    if (iter != game_word.end()) {
                        while (iter != game_word.end()) {
                            iter->second = true;
                            correct_guesses++;
                            iter = std::find(game_word.begin(), game_word.end(), std::pair<char, bool>(lower_guess[0], false));
                        }
                    }
                    else { // incorrect guess
                        lives--;
                    }
                    used_letters += " " + lower_guess;
                }
            }

            // winner screen
            if (correct_guesses == word_string.size()) {
                std::cout << "You WIN!  The word was \"" << word_string << "\"." << std::endl;
                break;
            }
            else if (lives == 0) {
                std::cout << "You LOSE!  The word was \"" << word_string << "\"." << std::endl;
                break;
            }
        }

        // at the end of the game
        std::cout << "Do you want to play again? [Y/N] ";
        std::string play;
        std::cin >> play;
        if (play[0] != 'Y' && play[0] != 'y') {
            return 1;
        }
    }

    return 0;
}
