// worked with Nicko Martinez
#include "HashTable.h"
#include "WordEntry.h"
#include <list>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

HashTable::HashTable (int input) {
	size = input;
    hashTable = new list<WordEntry>[input]; // makes hashTable an array of lists of size s
}

// from Zybooks section 8.5 as suggested by Parker Newton
int HashTable::computeHash(const string &input2) {
    int stringHash = 5381;
    int stringSize = input2.size();
    int i;
   for(i = 0; i < stringSize; i++){
      stringHash = ((stringHash * 33)%size) + input2.at(i);
   }
   return stringHash % size;
}

void HashTable::put(const string &input3, int score) {
    int hashVal3 = computeHash(input3);
    list<WordEntry>::iterator it3;
    // iterates through the list
    for (it3 = hashTable[hashVal3].begin(); (it3 != hashTable[hashVal3].end()); ++it3) {
        if (it3->getWord() == input3) {
            it3->addNewAppearance(score);
            return;
        }
    }
    
    WordEntry newEntry = WordEntry(input3, score);
    hashTable[hashVal3].push_back(newEntry); // throws the new entry onto the list
}

double HashTable::getAverage(const string &input4){
    int hashVal = computeHash(input4);
    list<WordEntry>::iterator it;
    // iterator to circle through the bucket
    for(it = hashTable[hashVal].begin(); (it != hashTable[hashVal].end()); it++){
        if(it->getWord() == input4){
            double average = it->getAverage();
            return average;
        }
    }
    return 2.0;
}

bool HashTable::contains(const string &input5) {
    int hashVal2 = computeHash(input5);
    list<WordEntry>::iterator it2;
    // iterator to circle through the bucket
    for(it2 = hashTable[hashVal2].begin(); (it2 != hashTable[hashVal2].end()); ++it2) {
        if(it2->getWord() == input5) {
        return true;
        }
    }
    return false;
}

// main function for zybooks

int main() {
	// declare a few needed variables for inputing the data
	string line;    
	int score;
	string message = " ";
	
	// open input file
	ifstream myfile("movieReviews.txt");
	if (myfile.fail()) {
	    cout << "could not open file" << endl;
	    exit(1);
	}
	
	//create hash table
	HashTable table(20071);
	
	while (!myfile.eof()) {
	    myfile >> score;     // get score
	    myfile.get();        // get blank space
	    getline(myfile, line);
	    int len = line.size();
	    while(len > 0) {     // identify all individual strings
	        string sub;
	        len = line.find(" ");
	        if (len > 0) {
	            sub = line.substr(0, len);
	            line = line.substr(len + 1, line.size());
	        }
	        else {
	            sub = line.substr(0, line.size() - 1);
	        }
	        table.put(sub, score); // insert string with the score
	    }
	}
	
	// after data is entered in hash function
	// prompt user for a new movie review
	while(message.length() > 0) {
	    cout << "enter a review -- Press return to exit: " << endl;
	    getline(cin, message);
	
	    // used for calculating the average
	    double sum = 0;
	    int count = 0;
	    
	    double sentiment = 0.0;
	    
	    size_t len = message.size();
	    // get each individual word from the input
	    while(len != string::npos) {
	        string sub;
	        len = message.find(" ");
	        if (len != string::npos) {
	            sub = message.substr(0, len);
	            message = message.substr(len + 1, message.size());
	        }
	        else {
	            sub = message;
	        }
	        // calculate the score of each word
	        sum += table.getAverage(sub);
	        ++count;
	    }
	
	    if (message.size() > 0) {
	    	sentiment = sum / count;
	        cout << "The review has an average value of " << sentiment << endl;
	        if (sentiment >= 3.0) {
	        	cout << "Positive Sentiment" << endl;
	        }
	        else if (sentiment >= 2.0) {
	        	cout << "Somewhat Positive Sentiment" << endl;
	        }
	        else if (sentiment >= 1.0) {
	        	cout << "Somewhat Negative Sentiment" << endl;
	        }
	        else {
	        	cout << "Negative Sentiment" << endl;
	        }
	        cout << endl;
	    }
	}
	
	return 0;
}