#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>

#include "threadpool.h"

#define MUL 1 // uncomment this if you want multithreading

using namespace std;
namespace fs = filesystem;

bool isLetter(char a)
{
    const string alphabet = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
    if (alphabet.find(a))
    {
        return true;
    }
    return false;
}

class Analyser
{
    fs::path dir_path;

    int files_number = 0;
    int lines_number = 0;
    int words_number = 0;
    int letters_number = 0;

#ifdef MUL
    thread_pool pool;
#endif
    mutex m_lines;
    mutex m_words;
    mutex m_letters;

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
            m_lines.lock();
            ++lines_number;
            m_lines.unlock();
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
                m_words.lock();
                ++words_number;
                m_words.unlock();
               numberOfLetters(word);
            }
        };
    }

    void numberOfLetters(string word)
    {
        for (char a : word)
        {
			if (a > 0 && isalpha(a))
			{
				m_letters.lock();
				++letters_number;
				m_letters.unlock();
			}
		}
    }

    
public:
    Analyser(fs::path path) : dir_path(path)
    {
    }

    void analyse()
    {
        

        for (auto& p : fs::recursive_directory_iterator(dir_path))
        {
            if (p.is_regular_file())
            {
                numberOfFiles();
#ifdef MUL
                pool.push_task(bind(&Analyser::numberOfLines, this, p));
#else
                numberOfLines(p);
#endif
            }
        }
#ifdef MUL
        pool.wait_for_tasks();
#endif
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
    a.analyse();

    return 0;
}