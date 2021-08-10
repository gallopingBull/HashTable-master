/*
 * @file hash.cpp  Main functions to assist hash function. 
 * 
 * @brief
 *    Implements several different functions (print, processFile,
 *search, remove, etc.) that assists hash function (hash_function.cpp). 
 *    
 * @author Joshua Salcido
 * @date 3/31/2016
 */

#include "hash.h" 
#include <string>
#include <fstream>
#include <list> 
#include <iostream> 

using namespace std; 

/*
 * Hash Constructor, initializes member variables.
*/
Hash::Hash(){
    longestList = 0; 
    collisions = 0; 
    runningAvgListLength = 0; 
}

/*
 * Iterates and prints out each element in the hash table. 
*/ 
void Hash::print(){
    list<string>::iterator iter;

    for (int i = 0; i < HASH_TABLE_SIZE; i++){
        cout << i << ":" << '\t'; 

        for (iter = hashTable[i].begin(); iter != hashTable[i].end(); iter++){
            cout << *iter << ", "; 
        }
        cout << endl; 
    }
}

/*
 * Reads input from file, then stores input into 
 * list. Incerements runningAvgListLength at each time an 
 * element is added into a slot by calling avgListLength();
 * Then longest_List() is called to get the length
 * of the longest list.
 *
 * @param 'word' The string that will be searched for 
 * in the list.  
*/ 
void Hash::processFile(string filename){
    ifstream ifile(filename.c_str());
    string word; 

    if (ifile.is_open()){         
        while (ifile >> word){
            int key = hf(word);
            if (!hashTable[key].empty()){
                hashTable[key].push_back(word); 
                collisions++; 
                runningAvgListLength = avgListLength(); 
            }
            else{
                hashTable[key].push_back(word);
                runningAvgListLength = avgListLength(); 
            }
        }
        ifile.close(); 
    }
    longestList = longest_List(); 
}

/*
 * Search through hash table for a word specified by 
 * user. Return 'true' if word is found, otheriwiese
 * 'false.'
 *
 * @param 'word' The string that will be searched for 
 * in the list. 
 *
 * @return Bool
*/ 
bool Hash::search(string word){
    list<string>::iterator iter;
    int key = hf(word); 

    for (int i = 0; i < HASH_TABLE_SIZE; i++){
        for (iter = hashTable[key].begin(); iter != hashTable[key].end(); iter++){
            if (*iter == word){
                return true;
            }
        }
    }
    return false; 
}

/*
 * Removes a word from the list that the user
 * specified. 
 *
 * @param 'word' The string that will be searched for 
 * in the hash table then removed.
*/ 
void Hash::remove(string word){
    list<string>::iterator iter;

    for (int i = 0; i < HASH_TABLE_SIZE; i++){        
        for (iter = hashTable[i].begin(); iter != hashTable[i].end(); iter++){
            if (*iter == word){
                hashTable[i].remove(*iter);
                runningAvgListLength = avgListLength(); 
                return; 
            }
        }
    }
}

/*
 * Prints hash table to a text file.  
 *
 * @param 'filename' string Name for output file.  
 *
*/

void Hash::output(string filename){
    ofstream ofile;
    ofile.open(filename);
    list<string>::iterator iter;

    if (ofile.is_open()){    
        for (int i = 0; i < HASH_TABLE_SIZE; i++){
            ofile << i << ":" << '\t'; 
            for (iter = hashTable[i].begin(); iter != hashTable[i].end(); iter++){
                ofile << *iter << ", ";
            }
            ofile << endl; 
        }
        ofile.close(); 
    }
}

/*
 * Prints number of collisions, longest
 * list, and the average lengths for each slot in the
 * hash table. 
*/
void Hash::printStats(){
    cout << "Total Collisions = " << collisions << endl; 
    cout << "Longest List Ever = " << longestList << endl; 
    cout << "Average List Length Over Time = " << runningAvgListLength << endl;
    cout << "Load Factor = " << loadFactor() << endl; 
}

/*
 * Returns the longest list after its found by iterating 
 * through the hash table. 
 *
 * @return unsigned int Contains the longest list. 
*/ 
unsigned int Hash::longest_List(){
    list<string>::iterator iter; 
    unsigned int max = 0; 
    unsigned int min = 0; 

    for (int i = 0; i < HASH_TABLE_SIZE; i++){
        if (max < min){
            max = min;
            list<string>::iterator iter;
            
            for (int i = 0; i < HASH_TABLE_SIZE; i++){
                cout << i << ":" << '\t';
                for (iter = hashTable[i].begin(); iter != hashTable[i].end(); iter++){
                    cout << *iter << ", ";
                    }
            }
            min = 0;

            for (iter = hashTable[i].begin(); iter != hashTable[i].end(); iter++){
                min++;
            }
        }
    }
    return max; 
}

/*
 * Returns the average length of all the 
 * lists in the hash table aftercalculating 
 * it. 
 *
 * @returns double Contains the average list length. 
*/ 
double Hash::avgListLength(){
    list<string>::iterator iter;
    double currentAvgLength = 0;
    double size = 0; 
   
    for (int i = 0; i < HASH_TABLE_SIZE; i++){     
        currentAvgLength += hashTable[i].size();
        if (!hashTable[i].empty()){
            size++; 
        }
    }
    currentAvgLength = currentAvgLength/size;
    return runningAvgListLength = (currentAvgLength + runningAvgListLength) / 2.0;
}

/*
 * Returns the hash table's load factor after 
 * calulating it. 
 *
 * @returns double Contains the load factor. 
*/ 
double Hash::loadFactor(){
    list<string>::iterator iter;
    double lf = 0;
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++){
        lf += hashTable[i].size();
    }
    lf = lf/HASH_TABLE_SIZE;
    return lf; 
}

