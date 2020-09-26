#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
using namespace std;


void to_lowercase(char & c) {
	c = tolower(static_cast<unsigned char>(c));
}


vector<string> split(string line, char d = ' ') {
	vector<string> makeVector;
    istringstream SS(line); string token;
    while(getline(SS,token, d)) {
    	istringstream SS2(token);
    	string token2;
    	while(SS2 >> token2) makeVector.push_back(token2);
    }
    return makeVector;
}


void replaceString(string & line, string original, string replaced) {
	// useful reference thispointer.com/find-and-replace-all-occurrences-of-a-sub-string-in-c/
	size_t pos = line.find(original); // the first time it appears
	while(pos != string::npos) { // until it reaches end of string
		// replace from occurance index till length of original with 'replaced'
		line.replace(pos, original.size(), replaced);
		// then find the next occurance, search starting from the last occurance
		pos = line.find(original, pos + replaced.size());
	}
}


void readDoc(unordered_map<string, int> & wC, int & tW, char* const fname) {      
	string generateName = fname;                

	ifstream filein(generateName);
	if (!filein.good()) {
		cout << "File no good: \"" << generateName << "\""<< endl;
		exit(0);
	}
	for(string line; getline(filein, line); ) {
		for_each(line.begin(), line.end(), to_lowercase);
		string removeChars[] = {",", ".", "?", ";", "'", "!", "\"", "(", ")", "[", "]", "$", "/"};
		for (int c = 0; c < 13; ++c)
		{
			replaceString(line, removeChars[c], "");
			if ( c <= 9) {
				replaceString(line, removeChars[c], to_string(c));
			}
		}
		vector<string> splitByWords = split(line);
		for(int j = 0; j < splitByWords.size(); j++) {
			wC[splitByWords[j]] += 1;
			tW += 1;
		}
	}
}


bool sortSecond(const pair<string,float> &a, 
              const pair<string,float> &b) 
{ 
    return (a.second > b.second); 
} 


void printWordFrequency(unordered_map<string, int> wordCount) {
	cout << "Two arguments" << endl;
	vector< pair <string, float> > alphaCount; // its easiar to sort with vector pairs than it is with ordered maps
	unordered_map<string, int>::iterator it = wordCount.begin();
	while (it != wordCount.end()) {
		alphaCount.push_back(make_pair(it->first, it->second)); // convert map to vector of pairs
		it++;
	}
	sort(alphaCount.begin(), alphaCount.end(), sortSecond);
	cout << "Printing 30 most frequent words..." << endl;
	for (int i = 0; i < 30; ++i)
	{
		cout << i+1 << ".) " << alphaCount[i].first << " -- " << alphaCount[i].second << endl;
	}
}


void printWordEnrichment(unordered_map<string, int> wordCount, int totalWords, char *fname) {
	unordered_map<string, int> docCount;
	vector< pair <string, float> > wordEnriched;
	int docTotalWords = 0;
	readDoc(docCount, docTotalWords, fname);
	int PC = 5;
	unordered_map<string, int>::iterator it = docCount.begin();
	while (it != wordCount.end())
	{
		string checkWord = it->first;
		float expected = docTotalWords * (static_cast<float> (wordCount[checkWord])  / totalWords);
		float enriched = static_cast<float>(docCount[checkWord] + PC) / (expected + PC);
		wordEnriched.push_back(make_pair(checkWord, enriched));
		it++;

	}
	int io_width = 15;
	sort(wordEnriched.begin(), wordEnriched.end(), sortSecond);
	cout << setw(io_width) << "WORD" << fixed << setw(io_width) << right << "num(doc) " << setw(io_width) << "freq(doc) " << setw(io_width)
		 << "num(all) " << setw(io_width) << "freq(all) " << setw(io_width)
		 << "expected " << setw(io_width) << "ratio " << endl;
	for (int i = 0; i < 25; ++i)
	{
		string checkWord = wordEnriched[i].first;
		cout << setw(io_width) << checkWord << setw(io_width) << docCount[checkWord] << setw(io_width) << static_cast<float> (docCount[checkWord])  / docTotalWords << setw(io_width)
			 << wordCount[checkWord] << setw(io_width) << static_cast<float> (wordCount[checkWord])  / totalWords << setw(io_width)
			 << docTotalWords * (static_cast<float> (wordCount[checkWord])  / totalWords) << setw(io_width) << wordEnriched[i].second;
		cout << endl;
	}
}


int main(int argc, char **argv)
{
	unordered_map<string, int> wordCount;
	int totalWords = 0;
	readDoc(wordCount, totalWords, argv[1]);
	if (argc == 2) {
		printWordFrequency(wordCount);
	} else if (argc == 3) {
		printWordEnrichment(wordCount, totalWords, argv[2]);
	} else {
		cout << "Syntax Error" << endl;
		cout << "Usage: " << endl;
		cout << " ./words <file 1> -- prints out the number of each word in file 1" << endl;
		cout << "./words <file1> <file2> -- finds enrichment of words in document file 2 based on collection file 1" << endl;
	}
	return 0;
}