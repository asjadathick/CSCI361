//
//  main.cpp
//  affinecipher
//
//  Created by Asjad Athick on 28/3/17.
//  Copyright © 2017 Asjad Athick. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
using namespace std;

//alphabet definitions
const char alphabet[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D'};
const int alphabetSize = 14; //must be length of alphabet array
//---------------------

int gcd(int a, int b = alphabetSize) {
	return b == 0 ? a : gcd(b, a % b);
}

bool isValidAKey(int keyA){
	if (!(keyA > 0 && keyA < alphabetSize)) {
		return false;
	}

	return gcd(keyA) == 1;
}

bool isValidBKey(int keyB){
	return ((keyB >= 0) && (keyB < alphabetSize));
}

bool isValidKey(int keyA, int keyB){
	return (isValidAKey(keyA) && isValidBKey(keyB));
}

vector<int> encrypt(vector<int> plainText, int keyA, int keyB){
	//plaintext is validated and in numeric format
	vector<int> cipherText = plainText;
	for (int i = 0; i < plainText.size(); ++i) {
		cipherText[i] *= keyA;
		cipherText[i] += keyB;
		cipherText[i] %= alphabetSize;
	}
	return cipherText;
}

int moduloInverse(int key, int mod){
	for (int i = 1; i < mod; ++i) {
		if (((key * i) % mod) == 1){
			return i;
		}
	}
	return 0;
}

int mod(int val, int mod = alphabetSize) {
	return ((val %= mod) < 0) ? val + mod : val;
}

vector<int> decrypt(vector<int> cipherText, int keyA, int keyB){
	//plaintext is validated and in numeric format
	//keys are validated
	int modInv = moduloInverse(keyA, alphabetSize);
	if (!modInv) {
		cout << "Decryption failed. Could not find modulo inverse for key." << endl; //shouldn't happen if the keys are valid
		exit(-1);
	}

	vector<int> plaintext = cipherText;
	for (int i = 0; i < cipherText.size(); ++i) {
		plaintext[i] -= keyB;
		plaintext[i] *= modInv;
		plaintext[i] = mod(plaintext[i]);
	}
	return plaintext;
}

int main(int argc, const char * argsv[]) {
	//check for correct usage syntax
	//push cmd line args into strings instead of using c-strings for ease of use
	vector<string> argv;
	for (int i = 0; i < argc; ++i) {
		argv.push_back(argsv[i]);
	}

	if (!(argc == 6) && (argv[1] == "-key") && (argv[4] == "-encrypt" || argv[4] == "-decrypt")){
		cout << "Incorrect usage. Ex: YourProgram -key B 3 -encrypt 03ACD45" << endl;
		cout << "Or YourProgram -key B 3 -decrypt 03ACD45" << endl;
		return -1;
	}

	//build key value map
	map<char, int> keyMap;
	map<int, char> reverseKeyMap;
	for (int i = 0; i < alphabetSize; ++i) {
		keyMap.insert(pair<char, int>(alphabet[i], i));
		reverseKeyMap.insert(pair<int, char>(i, alphabet[i]));
	}

	//convert key into numbers
	int keyA = keyMap[argv[2][0]];
	int keyB = keyMap[argv[3][0]];

	//check key validity
	if (!(isValidKey(keyA, keyB))) {
		cout << "Invalid key used. Program aborted." << endl;
		return -1;
	}

	//validate input string
	string plaintext = argv[5];
	vector<int> numericText;
	for (int i = 0; i < plaintext.length(); ++i) {
		if (keyMap.find(plaintext[i]) == keyMap.end()) {
			cout << "The input string contains characters without an alphabet definition. Program aborted." << endl;
			return -1;
		} else {
			//build numeric input vector
			numericText.push_back(keyMap[plaintext[i]]);
		}
	}

	vector<int> result;

	//flow control
	if (argv[4] == "-encrypt") {
		result = encrypt(numericText, keyA, keyB);
	} else {
		result = decrypt(numericText, keyA, keyB);
	}

	//back to chartext
	string resultText;
	for (int i = 0; i < numericText.size(); ++i) {
		resultText += reverseKeyMap[result[i]];
	}

	cout << "Input text: " << plaintext << endl;
	cout << "Operation: " << (argv[4] == "-encrypt" ? "Encryption" : "Decryption") << endl;
	cout << "Key A: " << argv[2] << "; Key B: " << argv[3] << endl;
	cout << (argv[4] == "-encrypt" ? "Encrypted" : "Decrypted") << " text: " << resultText << endl;
    return 0;
}
