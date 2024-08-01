#include "UniqueWordsHelper.h"

#include <fstream>
#include <thread>
#include "../PTC/ProcessTimeCounter.h"

bool UniqueWordsHelper::mIsRunning = false;
Concurrency::concurrent_unordered_set<std::string> UniqueWordsHelper::mUniqueWords;

const size_t cBufferSize = 1024 * 1024 * 64; // Byte * kByte * MByte

void UniqueWordsHelper::processChunk(const std::string& aChunk) 
{
    std::unordered_set<std::string> uniqeWords;

    size_t start = 0;
    size_t end = 0;
    std::string word;

    while ((end = aChunk.find(' ', start)) != std::string::npos) 
    {
        word.assign(aChunk, start, end - start);
        if (!word.empty()) {
            uniqeWords.insert(std::move(word));
        }
        start = end + 1;
    }

    word.assign(aChunk, start, aChunk.size() - start);
    if (!word.empty()) 
    {
        uniqeWords.insert(std::move(word));
    }

    for (const auto& word : uniqeWords) {
        mUniqueWords.insert(word);
    }
}

sUniqueWordsData UniqueWordsHelper::processFile(const std::string& aFilename)
{
    sUniqueWordsData result;

    if (!mIsRunning && !aFilename.empty())
    {
        mIsRunning = true;
        ProcessTimeCounter timer;
        timer.Start();

        size_t thereadsAmount = std::thread::hardware_concurrency();
        if (thereadsAmount == 0)
        {
            thereadsAmount = 1;
        }

        std::ifstream file(aFilename, std::ios::in | std::ios::binary);
        if (!file.is_open()) 
        {
            throw std::runtime_error("Could not open file");
        }

        std::vector<char> buffer(cBufferSize);
        std::vector<std::thread> threads;

        while (file) 
        {
            file.read(buffer.data(), buffer.size());
            std::streamsize sizeOfFile = file.gcount();

            if (sizeOfFile <= 0)
            {
                break;
            }

            std::string chunk(buffer.data(), static_cast<unsigned int>(sizeOfFile));
            
            size_t lastSpace = chunk.find_last_of(' ');
            if (lastSpace != std::string::npos && !file.eof()) 
            {
                file.seekg(-(sizeOfFile - lastSpace - 1), std::ios_base::cur);
                chunk.resize(lastSpace);
            }

            size_t chunkSize = chunk.size() / thereadsAmount;
            for (size_t i = 0; i < thereadsAmount; ++i) 
            {
                size_t start = i * chunkSize;
                size_t end = (i == thereadsAmount - 1) ? chunk.size() : chunk.find(' ', start + chunkSize);
                if (end == std::string::npos)
                {
                    end = chunk.size();
                }

                std::string subChunk = chunk.substr(start, end - start);
                threads.emplace_back(processChunk, std::move(subChunk));
            }

            for (auto& thread : threads) 
            {
                thread.join();
            }
            threads.clear();
        }

        result.set(timer.Stop(), mUniqueWords);
        mUniqueWords.clear();

        file.close();

        mIsRunning = false;
    }

    return result;
}

