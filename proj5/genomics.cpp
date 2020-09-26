#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;


struct Gene
{
	string description;
	string startC;
	string endC;
	string strand;
	string size;
	string dontcare1;
	string dontcare2;
	string geneName;
	string orfid;
};


void makeData(char C, int & streak, char & streak_holder, int & pos) {
	if (streak_holder != C) {
		streak = 1;
		streak_holder = C;
	} else {
		streak++;
	}
	pos++;
}


vector<string> read_genome(char* fileName) {
	vector<string> makeVector;
	int basepair = 0;
		ifstream filein(fileName);
		if (!filein.good()) {
			cout << "File name not found. Exitting program...";
			exit(0);
		}
		bool readHeader = false;
		for (string line; getline(filein, line); ) { 
			if (!readHeader) {
				cout << "\nReading: "<< line.substr(line.find('>')+1, line.size()) << endl; // +1 because .substr() index is inclusive
				readHeader = true;

			} else {
				makeVector.push_back(line);
			}
	    }
	return makeVector;
}


vector<Gene> read_prot_table(char* fname) {
	int basepair = 0;
	vector<Gene> holdData;
		ifstream filein(fname);
		if (!filein.good()) {
			cout << "File name not found. Exitting program...";
			exit(0);
		}
		bool readHeader = false;
		for (string line; getline(filein, line); ) { 
			vector<string> tokens;
			Gene readGene;
		    istringstream iss(line);
		    string token;
		    while (getline(iss, token, '\t')) {
		    	tokens.push_back(token);
		    }
		    readGene.description = tokens[0];
		    readGene.startC = tokens[1];
		    readGene.endC = tokens[2];
		    readGene.strand = tokens[3];
		    readGene.size = tokens[4];
		    readGene.dontcare1 = tokens[5];
		    readGene.dontcare2 = tokens[6];
		    readGene.geneName = tokens[7];
		    readGene.orfid = tokens[8];
		    holdData.push_back(readGene);
	    }
	return holdData;
}


void analyze_genome(vector<string> V, vector<int> & Vref, bool readNuc, int start, int end) {
	int streak = 0; char streak_holder = 'T'; int max_streak = 0; char max_streak_holder;
	int basepair = 0; int getPos = 1; int maxPos = 0;
	vector<int> nucleotides_count(4, 0);
	vector<char> nucleotides;
	nucleotides.push_back('G');
	nucleotides.push_back('T'); 
	nucleotides.push_back('C');
	nucleotides.push_back('A');
	int breakline = 0;
	int lineamt = 0;
	for (int i = 0; i < V.size(); i++) { // iterates through each line
    	for (int y = 0; y < V[i].size(); y++) { // iterates through each character in line
    		for (int z = 0; z < nucleotides_count.size(); z++) { // iterates through possible nucleotides
    			if(V[i][y] == nucleotides[z]) { 
    				makeData(nucleotides[z], streak, streak_holder, getPos);
    				nucleotides_count[z] += 1;
    				break; // if match found, break z loop
    			}
    		}
	    	if (streak > max_streak) {
	    		max_streak = streak;
	    		max_streak_holder = streak_holder;
	    		maxPos = getPos;
	    	}
		    if (readNuc) {
		    	if (getPos > start && getPos < end) {
		    		if (lineamt == 0) {
		    			cout << endl << 1 << " ";
		    			lineamt++;	
		    		}
		    		if(breakline == 71) {
		    			cout << endl << lineamt*71 << " ";
		    			lineamt++;
		    			breakline = -1;
		    		}
		    		cout << V[i][y];
		    		breakline++;
		    	}
		    }
    	}
    }
    for (int z = 0; z < nucleotides_count.size(); z++) {
    	basepair += nucleotides_count[z];
	}
	float GC_ratio = static_cast<float>(nucleotides_count[2] + nucleotides_count[0]) / (0.01 * basepair);
	if (!readNuc) {
		cout << "GC content: " << GC_ratio << "%" << endl;
	    cout << "Longest homopolymer: " << string(max_streak, max_streak_holder);
	    cout << " (len= " << max_streak << " bp) " << "at coord " << maxPos - max_streak - 1 << endl;
	    cout << "Length: " << basepair << endl;
	}
    Vref.push_back(basepair);
    Vref.push_back(max_streak);
    Vref.push_back(max_streak_holder);
    Vref.push_back(maxPos);
}


void analyze_genes(vector<Gene> V, vector<int> length) {
	int numGenes = 0;
	int smalletGene = atoi(V[0].size.c_str());
	int largestGene = atoi(V[0].size.c_str());
	int largestGap = 0; int smallestGap = 0; int startingPos;
	int sumGap = 0;
	float sum = 0;
	bool foundLongest = false;
	string longest_homo_name;
	for (int i = 0; i < V.size(); i++) {
		numGenes++;
		int iterSize = 3 * atoi(V[i].size.c_str());
		sum += iterSize;
		if (iterSize < smalletGene) {
			smalletGene = iterSize;
		} else if (iterSize > largestGene) {
			largestGene = iterSize;
		}
		if (i+1 < V.size()) startingPos = atoi(V[i+1].startC.c_str());
		int endingPos = atoi(V[i].endC.c_str());
		int gap = startingPos - endingPos;
		if (gap > largestGap) {
			largestGap = gap;
		} else if (gap < smallestGap) {
			smallestGap = gap;
		}
		sumGap += gap;
		if ((endingPos > length[3] - length[1] - 1) && (!foundLongest)) {
			longest_homo_name = V[i-1].orfid;
			foundLongest = true;
		}
	}

	cout << "num genes: " << numGenes << endl;
	cout << "gene sizes: [" << smalletGene << ", ";
	cout << largestGene << "], ";
	float mean = sum / numGenes;
	cout << "mean = " << mean << "bp, ";

	float deviance = 0;
	for (int i = 0; i < V.size(); i++) {
		int iterSize = atoi(V[i].size.c_str());
		deviance += pow(abs((iterSize) - mean), 2);
	}
	float std_dev = sqrt(deviance / numGenes);
	cout << "stdev = " << std_dev << endl;
	cout << "coding fraction: " << 100*sum  / length[0] << "%" << endl;
	cout << "mean size of intergenic regions: " << sumGap/(numGenes-1) << endl;
	cout << "largest intergenic region: " << largestGap << endl;
	cout << "smallest intergenic region: " << smallestGap << endl;

	cout << "Longest homopolymer: " << string(length[1], length[2]);
	cout << " (len= " << length[1] << " bp) " << "at coord " << length[3] - length[1] - 1;
	cout << " after " << longest_homo_name << endl;
}


void printSeq(string userInput, vector<Gene> V, vector<string> splitvector, vector<int> genomeData) {
	int startingPos = 0; int endingPos = 0;
	for (int i = 0; i < V.size(); i++) {
		if (userInput == V[i].orfid || userInput == V[i].geneName) {
			startingPos = atoi(V[i].startC.c_str());
			endingPos = atoi(V[i].endC.c_str());
			analyze_genome(splitvector, genomeData, true, startingPos, endingPos);
			exit(0);
		} 
	}
	cout << "Gene not found." << endl;
}

int main(int argc,char** argv) {
	if (argc < 3) { cout << "Syntax: ./genomics <file name>"; return 0; }
	vector<int> genomeData;
	vector<string> splitvector = read_genome(argv[1]);
    analyze_genome(splitvector, genomeData, false, 0, 0);
    vector<Gene> GeneVector = read_prot_table(argv[2]);
    analyze_genes(GeneVector, genomeData);
    if (argc == 4) {
    	printSeq(argv[3], GeneVector, splitvector, genomeData);
	}
}