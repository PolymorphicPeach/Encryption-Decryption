// Author: Matthew Peach
// Program Status: Complete
//
// Description: 
//    Use enter a file name into encryption_main.cpp, which will change each character into another random character
//  and generate an encryption key file (named "yourFileName_EcryptionKey.txt"). Then, use decryption_main.cpp
//  to enter the file name and the encryption key file name. decryption_main.cpp will overwrite the encrypted file to be
//  understandable again.
//
//    I kept getting warnings about using "vectorName.size()" in loops, so I static_cast'd them to integers. I also explicitly used
//  signed chars because I was getting bugs using unsigned and also read that every compiler doesn't use the same "default char" 
//  (some use signed and some unsigned).  

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

class MyEncryption{
private:
    vector <string> fileLines;
    
    int defaultValue = 0;
    vector < vector <int> > encryptionKey;
public:
    MyEncryption(vector <string> fileLines){
        this->fileLines = fileLines;
    }
    void generateEncryptionKey();
    void overwriteUencryptedFile(string fileName);

};

string getFileName();



int main(){
    srand((unsigned) time(0));
    string line;
    string fileName {"Unknown"};
    bool failureToOpen {false};
    
    //=============================================================================================================
    //                              Input Validation: Does the file actually exist? 
    //=============================================================================================================
    do{
        failureToOpen = false;
        
        fileName = getFileName(); // setting the path and name of the file
        
        std::ifstream *encryptThisCheck = new std::ifstream (fileName); // opening the file
        
        if( !encryptThisCheck->is_open() ){
            failureToOpen = true;
            delete encryptThisCheck;
            cout << endl << "Error: File not found." << endl;
        }
        else{
            encryptThisCheck->close(); // Close the file and delete the object now that it is known to exist
            delete encryptThisCheck;
        }
        
    }while(failureToOpen); // Repeat while failing to open
    

    
    //============================================================================================================
    //                      There is a valid file, so put the lines into a vector
    //============================================================================================================
    
    std::ifstream encryptThis (fileName);
    
    vector <string> fileLines;
    
    cout << "==========================================================" << endl
         << "|                  Your unencrypted text                 |" << endl
         << "==========================================================" << endl << endl;
    
    // Add each line to the fileLines array
    while( getline(encryptThis, line) ){
        fileLines.push_back(line);
        cout << line << endl;
    }
    encryptThis.close(); //Done with the file
    
    cout << endl << "=========================================================" << endl << endl;
    
    
    //=============================================================================================================
    //                        Construct the MyEncryption object
    //=============================================================================================================
    
    MyEncryption *EncryptedFile = new MyEncryption(fileLines);
    EncryptedFile->generateEncryptionKey();
    EncryptedFile->overwriteUencryptedFile(fileName);
    
    cout << "==========================================================" << endl
         << "|                Encryption: Complete!                   |" << endl
         << "==========================================================" << endl
         << endl
         << fileName << " has been overwritten and contains encrypted content." << endl
         << endl
         << fileName.substr(0, fileName.length() - 4) + "_EncryptionKey.txt contains the key to decrypt " << fileName << endl << endl;
    
    return 0;
}

string getFileName(){
    string fileNameInput {"Unset"};
    
    cout << "Enter the file name or path of the file." << endl << endl
         << "(Examples: original_text.txt OR ../original_text.txt)" << endl << endl
         << "(Your IDE may put files \"one up\" on the path tree like mine does," << endl
         << " in which case, use ../original_text.txt)" << endl << endl
         << "Filename/Path: ";
    getline(cin, fileNameInput);
    cin.clear();
    cout << endl << "==================================================================" << endl;

    return fileNameInput;
}

void MyEncryption::generateEncryptionKey(){
    signed char characterOfInterest;
    signed char randomCharacter;
    int encryptionCode;
    
    encryptionKey.resize(fileLines.size()); // Has to be resized to be able to use: encryptionKey[i].pushback()
    
    
    for(int i = 0; i < static_cast <int> (fileLines.size()); ++i){                   // For each line in the file
        for(int j = 0; j < static_cast <int> (fileLines[i].size()); ++j){           //    For each character in the string

            int randomNumber = (rand() % (126 - 33)) + 33;                        // Generate random *printable* ASCII character                                                                                
            randomCharacter = randomNumber;                                      // Range: 33 - 126
                                      
            characterOfInterest = static_cast <signed char> (fileLines[i][j]);    // Select the current character in the file     
            encryptionCode = randomCharacter - characterOfInterest;              // Encryption code for this character = difference between it and the random character
            fileLines[i][j] = randomCharacter;                                  // Current character in file is now the random character
            encryptionKey[i].push_back(encryptionCode);                        // The character to be added or subtracted to this character is
                                                                              //   added to the encryption key
        }
    }
    
    cout << "==========================================================" << endl
         << "|                  Your encrypted text                   |" << endl
         << "==========================================================" << endl << endl;
    
    for(int i = 0; i < fileLines.size(); ++i){
        if(fileLines[i].empty()){
            cout << endl;
        }
        else{
            for(int j = 0; j < fileLines[i].size(); ++j){
                cout << fileLines[i][j];
                if( j == fileLines[i].size() - 1){
                    cout << endl;
                }
            }
        }
    }
    cout << endl << "==========================================================" << endl << endl;
    
    cout << "==========================================================" << endl
         << "|                 Your encryption Key                    |" << endl
         << "==========================================================" << endl << endl;
    
    
    for(int i = 0; i < encryptionKey.size(); ++i){
        if(encryptionKey[i].empty()){
            cout << endl;
        }
        else{
            for(int j = 0; j < encryptionKey[i].size(); ++j){
                cout << encryptionKey[i][j];
                if(j == encryptionKey[i].size() - 1){
                    cout << endl;
                }
            }
        }
    }
    cout << endl << "==========================================================" << endl << endl;
}

void MyEncryption::overwriteUencryptedFile(string fileName){
    
    //======================= Overwriting the original file========================================================
    
    std::ofstream encryptedFile(fileName, std::ofstream::trunc); // Overwriting the original file
    
    for(int i = 0; i < fileLines.size(); ++i){
        if(fileLines[i].empty()){
            encryptedFile << endl;
        }
        else{
            for(int j = 0; j < fileLines[i].size(); ++j){
                encryptedFile << fileLines[i][j];
                if( j == fileLines[i].size() - 1){
                    encryptedFile << endl;
                }
            }
        }
    }
    encryptedFile.close();
    
    //============== Creating an Encryption key file ================================================================
    
                                               // cutting off the ".txt" part
    string encryptionKeyFileName = fileName.substr(0, fileName.length() - 4) 
                                                    + "_EncryptionKey.txt"; // It should be clear which file it goes with
    
    std::ofstream encryptionKeyFile(encryptionKeyFileName);
    
    for(int i = 0; i < encryptionKey.size(); ++i){
        if(encryptionKey[i].empty()){
            encryptionKeyFile << endl;
        }
        else{
            for(int j = 0; j < encryptionKey[i].size(); ++j){
                encryptionKeyFile << encryptionKey[i][j] << ' ';
                if(j == encryptionKey[i].size() - 1){
                    encryptionKeyFile << endl;
                }
            }
        }
    }
    
    encryptionKeyFile.close();
}