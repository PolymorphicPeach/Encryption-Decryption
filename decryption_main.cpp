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
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;


class MyDecryption{
private:
    vector <string> encryptedText;
    vector < vector <int> > decryptionKey;

public:
    MyDecryption(vector <string> encryptedfileLines, vector < vector <int> > keyFileLines){
        this->encryptedText = encryptedfileLines;
        this->decryptionKey = keyFileLines;
    }
    void decryptFile(string fileName);

};

bool checkFileLines(std::ifstream &file);
string getFileName();

int main(){
    string fileName {"Unknown"};
    string fileNameKey {"Unknown"};
    string line {"Unknown"};
    bool failureToOpen {false};
    bool validKeyFormat {false};
    vector <string> encryptedFileLines;
    vector < vector <int> > keyFileLines;
    
    //=============================================================================================================
    //                              Input Validation: Does the file actually exist? 
    //=============================================================================================================
    
    //=================================== Checking for encrypted file =============================================
    do{
        cout << "=========================================================" << endl
             << "|  Enter the filename/path of the ENCRYPTED file        |" << endl
             << "|  NOTE: (do not enter the _EncryptionKey.txt file yet) |" << endl
             << "=========================================================" << endl;
        
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
    
    
    //=================================== Checking for key file =================================================
    do{
        cout << "=========================================================" << endl
             << "|      Enter the filename/path of the KEY file          |" << endl
             << "=========================================================" << endl;
        
        failureToOpen = false;
        validKeyFormat = false;
        
        fileNameKey = getFileName(); // setting the path and name of the file
        
        std::ifstream *encryptThisCheck = new std::ifstream (fileNameKey); // opening the file
        
        validKeyFormat = checkFileLines(*encryptThisCheck);
        
        if( !encryptThisCheck->is_open() ){
            failureToOpen = true;
            delete encryptThisCheck;
            cout << endl << "Error: File not found!!!" << endl << endl;
        }
        else{
            encryptThisCheck->close(); // Close the file and delete the object now that it is known to exist
            delete encryptThisCheck;
        }
        
    }while(failureToOpen || !validKeyFormat); // Repeat while failing to open
    
    
    //=============================================================================================================
    //                          Both files are known to exist at this point
    //=============================================================================================================
    
    
    //======================= Adding encrypted file contents to a vector ==========================================
    std::ifstream decryptThis (fileName);
    
    cout << "==========================================================" << endl
         << "|                  Your encrypted text                   |" << endl
         << "==========================================================" << endl << endl;
    
    // Add each line to the fileLines array
    while( getline(decryptThis, line) ){
        encryptedFileLines.push_back(line);
        cout << line << endl;
    }
    decryptThis.close(); //Done with the file
    
    cout << endl << "=========================================================" << endl << endl;
    
    line.clear(); // Just in case
    
    //====================== Adding the key to a vector ===========================================================
    
    std::ifstream validKey(fileNameKey);
    
    int keyLineNumber{0};
    
    // Must be explicitly resized to prevent errors
    keyFileLines.resize(encryptedFileLines.size());
    
    while( getline(validKey, line)){
        std::stringstream streamIt (line);
        int number;
        while(streamIt >> number){
            keyFileLines[keyLineNumber].push_back(number);
        }
        ++keyLineNumber; // Needed to keep track of which part of the vector to be in
    }
    
    cout << "=====================================================" << endl
         << "|                 Encrypted Text                    |" << endl
         << "=====================================================" << endl;
    
    for(int i = 0; i < static_cast <int> (encryptedFileLines.size()); ++i){
        if(static_cast <int> (encryptedFileLines[i].size()) == 0){
            cout << endl;
        }
        else{
            cout << encryptedFileLines[i] << endl;
        }
    }
    
    //============================== Display =================================================
    
    cout << "=====================================================" << endl
         << "|                Encryption Key                     |" << endl
         << "=====================================================" << endl;
    
    
    for(int i = 0; i < keyFileLines.size(); ++i){
        if(keyFileLines[i].empty()){
            cout << endl;
        }
        else{
            for(int j = 0; j < keyFileLines[i].size(); ++j){
                cout << keyFileLines[i][j] << " ";
            }
        }
        cout << endl;
    }
    
    cout << endl;
    
    //=========================================================================================
    //                        MyDecryption Object Creation 
    //=========================================================================================
    
    MyDecryption *EncryptedFile = new MyDecryption(encryptedFileLines, keyFileLines);
    
    EncryptedFile->decryptFile(fileName);
    
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

bool checkFileLines(std::ifstream &file){ //This was a helpful function repurposed from the payroll.dat assignment
    string fileLine {"Not read"};
    bool passCheck = false;
    int numberOfLines {0};
    
    // "file" replaces "cin"
    while(getline(file, fileLine)){
        ++numberOfLines;
        if( fileLine.empty() ){
            continue;
        }
        else{
            for(int i = 0; i < static_cast <int> (fileLine.length()); ++i){
                //    Not a digit         &&   Not a space      &&   Not a negative sign
                if( !isdigit(fileLine[i]) && static_cast <char> (fileLine[i]) != ' ' && static_cast <char> (fileLine[i]) != '-'){
                    cout << endl << endl << "ERROR: Your key file is not in the correct format on line #" << numberOfLines <<" !!!" << endl << endl;
                    return passCheck;
                }
            }
        }
        
    }
    passCheck = true;
    return passCheck;
}

void MyDecryption::decryptFile(string fileName){
    
    vector <string> decryptedText;
    decryptedText.resize(encryptedText.size());
    signed char decryptedChar {'0'};
    
    // Applying the key.
    for(int i = 0; i < static_cast <int> (encryptedText.size()); ++i){
        if(encryptedText[i].empty()){
            continue;
        }
        else{
            for(int j = 0; j < static_cast <int> (encryptedText[i].size()); ++j){
                
                // Applying the key to a character
                decryptedChar = static_cast <signed char> (encryptedText[i][j]) - decryptionKey[i][j];
            
                decryptedText[i].push_back(decryptedChar);
            }
        }
        
    }
    cout << "=====================================================" << endl
         << "|             Your decrypted text                   |" << endl
         << "=====================================================" << endl << endl;   
    
    for(int i = 0; i < static_cast <int> (decryptedText.size()); ++i){
        if(decryptedText[i].empty()){
            cout << endl;
        }
        else{
            cout << decryptedText[i] << endl;
        }
    }
    cout << endl << "=====================================================" << endl << endl;
    
    std::ofstream decryptedFile(fileName);
    
    for(int i = 0; i < static_cast <int> (decryptedText.size()); ++i){
        decryptedFile << decryptedText[i] << endl;
    }
    decryptedFile.close();
    
    cout << "Note: " << fileName << " has also now changed!!" << endl << endl;
    
    
    
    cout << endl;
    
}