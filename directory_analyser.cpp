// directory_analyser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

using namespace std;
namespace fs = filesystem;

class Analyser
{
    fs::path dir_path;

    int files_number = 0;
    int lines_number = 0;
    int words_number = 0;
    int letters_number = 0;

    void numberOfFiles()
    {
        ++files_number;
    }

    void numberOfLines(fs::directory_entry p)
    {
        ifstream file(p);
        string line;
        while (getline(file, line))
        {
            ++lines_number;
            numberOfWords(line);
        }
    }

    void numberOfWords(string line)
    {
        istringstream words(line);
        string word;
        regex pattern(".*[[:alpha:]].*");

        while (words >> word)
        {
            if (regex_match(word, pattern))
            {
                ++words_number;
                numberOfLetters(word);
            }
        };
    }

    void numberOfLetters(string word)
    {
        for (char a : word)
        {
            if (isalpha(a))
            {
                ++letters_number;
            }
        }
    }

public:
    Analyser(fs::path path) : dir_path(path)
    {
    }

    void analise()
    {
        for (auto& p : fs::recursive_directory_iterator(dir_path))
        {
            if (p.is_regular_file())
            {
                numberOfFiles();
                numberOfLines(p);
            }
        }

        cout << "Directory: " << dir_path << endl
             << "Number of files: " << files_number << endl
             << "Number of lines: " << lines_number << endl
             << "Number of words: " << words_number << endl
             << "Number of letters: " << letters_number << endl;
    }
};

int main()
{
    fs::path dirpath;

    do
    {
        cout << "Put a path of a directory you want to analyse, or \"0\" if you want to exit:\n";
        cin >> dirpath;
        if (dirpath == "0")
            return 0;
    } while (!fs::is_directory(dirpath));
    
    Analyser a(dirpath);
    a.analise();

    return 0;
}
