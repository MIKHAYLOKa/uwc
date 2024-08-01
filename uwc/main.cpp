#include <iostream>
#include "UWH/UniqueWordsHelper.h"
#include "PTC/ProcessTimeCounter.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
   
    try {
        UniqueWordsHelper wordsManager;
        auto result = wordsManager.processFile(argv[1]);
        std::cout << "Amount of unique words: " << result.words.size() << std::endl;
        std::cout << "Process time: " << result.processTime.count() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}