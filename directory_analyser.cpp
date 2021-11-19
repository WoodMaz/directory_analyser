// directory_analyser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class Analyser
{
    fs::path dir_path;

public:
    Analyser(fs::path path) : dir_path(path)
    {
    }

    int numberOfFiles()
    {
        int count = 0;
        for (auto& p : fs::recursive_directory_iterator(dir_path))
        {
            if (p.is_regular_file())
            {
                std::cout << p.path() << '\n';
                ++count;
            }
        }
        return count;
    }
};

int main()
{
    fs::path dirpath;

    do
    {
        std::cout << "Put a path of a directory you want to analyse, or \"0\" if you want to exit:\n";
        std::cin >> dirpath;
        if (dirpath == "0")
            return 0;
    } while (!fs::is_directory(dirpath));
    
    Analyser a(dirpath);
    int c = a.numberOfFiles();
    std::cout << c << "\n";

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
