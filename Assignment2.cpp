

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int arrDoub = 0;

struct wordItem
{
 string word;
 int count = 0;
};

/*
* Function name: getStopWords
* Purpose: read stop word list from file and store into an array
* @param ignoreWordFile - filename (the file storing ignore words)
* @param ignoreWords - store ignore words from the file (pass by
reference) * @return - none
* Note: The number of words is fixed to 50
*/
void getStopWords(char *ignoreWordFileName, string ignoreWords[]){
    ifstream fr;
	fr.open(ignoreWordFileName);
	if(fr.is_open()){
		string line;
		int index = 0;
		while(getline(fr, line)){

			stringstream ss(line);
			string word;
			getline(ss, word);
			ignoreWords[index] = word;
			index++;
		}
	}
}

/*
* Function name: isStopWord
* Purpose: to see if a word is a stop word
* @param word - a word (which you want to check if it is a stop word)
* @param ignoreWords - the array of strings storing ignore/stop words *
@return - true (if word is a stop word), or false (otherwise)
*/
bool isStopWord(string word, string ignoreWords[]){
    for(int i=0; i<50; i++){
        if(ignoreWords[i] == word){
            return true;
        }
    }
    return false;
}
/*
* Function name: getTotalNumberNonStopWords
* Purpose: compute the total number of words saved in the words array
(including repeated words)
* @param list - an array of wordItems containing non-stopwords
* @param length - the length of the words array
* @return - the total number of words in the words array (including
repeated words multiple times)
*/
int getTotalNumberNonStopWords(wordItem wordItemList[], int numberOfUniqueWords){
    int total = 0;
    for(int i=0; i<numberOfUniqueWords; i++){
        total += wordItemList[i].count;
    }
    return total;
}
/*
* Function name: arraySort
* Purpose: sort an array of wordItems, decreasing, by their count
fields * @param list - an array of wordItems to be sorted
* @param length - the length of the words array
*/
void arraySort(wordItem wordItemList[], int numberOfUniqueWords){
    for(int i=0; i<numberOfUniqueWords; i++){
        for(int j=0; j<numberOfUniqueWords; j++){
            wordItem a = wordItemList[j];
            wordItem b = wordItemList[j+1];
            if(wordItemList[j].count<wordItemList[j+1].count){
                wordItemList[j]=b;
                wordItemList[j+1]=a;
            }           
        }
    }
}
/*
* Function name: printTopN
* Purpose: to print the top N high frequency words
* @param wordItemList - a pointer that points to a *sorted* array of
wordItems * @param topN - the number of top frequency words to print
* @return none
*/
void printTopN(wordItem wordItemList[], int topN){
    for(int i=0; i<topN; i++){
        cout<<wordItemList[i].count<<" - "<<wordItemList[i].word<<endl;
    }
}

/*
* Function name: resize
* Purpose: to make arr larger using array doubling.
* @return none
*/
void resize(wordItem *&oldArray, int *capacity){
	// cout <<  "resize has run" << endl;
	int newCapacity = *capacity * 2;
	wordItem *newArray = new wordItem[newCapacity];
	for(int i=0; i<(*capacity); i++){
		// cout<<"setting newArray["<<i<<"] = oldArray["<<i<<"]"<< endl;
		newArray[i] = oldArray[i];
	}
	delete[] oldArray;
	oldArray = newArray;
	*capacity = newCapacity;
	arrDoub++;
}

int main(int argc, char *argv[]){
	int topN = stoi(argv[1]);
	string bookFile = argv[2];
	char* ignoreFile = argv[3];


	//read in HG.txt to uwArr (unique words Arr)
	ifstream fr;
	fr.open(bookFile);
	int capacity = 100;
	wordItem *uwArr = new wordItem[capacity]; //arr is the uniqueWords Array reading from HG.txt
	string *stArr = new string[50];
	getStopWords(ignoreFile, stArr);
	// for(int i=0; i<50; i++){ //print ignoreWords array
	// 	cout << stArr[i] << endl;
	// }
	int index = 0; //will count the uwArr length
	if(fr.is_open()){
	string line;
		while(!fr.eof()){

			string word;
			fr >> word;
			if(index == capacity){
				resize(uwArr, &capacity);
			}

			bool exists = isStopWord(word, stArr); //checking if word exists in ignoreWords
			if(!exists){
				for(int i=0; i<capacity; i++){ //checking if word already exists in uwArr
					if(uwArr[i].word == word){
						uwArr[i].count += 1;
						exists = true;
					}
				}
			}
			if(!exists){ //if word is new, add it uwArr.word and +1 to uw.count
				uwArr[index].word = word;
				uwArr[index].count += 1;
				index++;
			}
		}
	}
	else{
		cout<<"File didn't open!"<<endl;
	}
	arraySort(uwArr, index);
	printTopN(uwArr, topN);
	cout<<"#"<<endl;
	cout<<"Array doubled: "<<arrDoub<<endl;
	cout<<"#"<<endl;
	cout<<"Unique non-common words: "<<index<<endl;
	cout<<"#"<<endl;
	cout<<"Total non-common words: "<<getTotalNumberNonStopWords(uwArr, index)<<endl;
}