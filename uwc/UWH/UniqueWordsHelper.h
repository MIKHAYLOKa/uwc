#ifndef UNIQUE_WORDS_HELPER
#define UNIQUE_WORDS_HELPER

#include <string>
#include <chrono>
#include <unordered_set>
#include <concurrent_unordered_set.h>

struct sUniqueWordsData
{
	std::chrono::milliseconds processTime;
	std::unordered_set<std::string> words;

	void set(std::chrono::milliseconds aTime, const Concurrency::concurrent_unordered_set<std::string>& aWords)
	{
		processTime = aTime;
		words.insert(aWords.begin(), aWords.end());
	}
};

class UniqueWordsHelper
{
private:

	static bool mIsRunning;
	static Concurrency::concurrent_unordered_set<std::string> mUniqueWords;

private:

	static void processChunk(const std::string& lines);

public:

	static sUniqueWordsData processFile(const std::string& aFilename);
};
#endif  // !UNIQUE_WORDS_HELPER

