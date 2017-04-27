//
//  main.cpp
//  ldes-ass
//
//  Created by Asjad Athick on 7/4/17.
//  Copyright © 2017 Asjad Athick. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>
#include <sstream>
using namespace std;

map<vector<int>, vector<int> > sbox;

vector<int> binaryStringToIntegerVector(string str){
	vector<int> res;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == '0') {
			res.push_back(0);
		} else {
			res.push_back(1);
		}
	}
	return res;
}

void constructSbox(){
	vector<int> key, value;
	key = binaryStringToIntegerVector("000");
	value = binaryStringToIntegerVector("00");
	sbox.insert(make_pair(key, value));

	key = binaryStringToIntegerVector("001");
	sbox.insert(make_pair(key, value));

	key = binaryStringToIntegerVector("010");
	sbox.insert(make_pair(key, value));

	key = binaryStringToIntegerVector("011");
	value = binaryStringToIntegerVector("01");
	sbox.insert(make_pair(key, value));

	key = binaryStringToIntegerVector("100");
	value = binaryStringToIntegerVector("00");
	sbox.insert(make_pair(key, value));

	key = binaryStringToIntegerVector("101");
	sbox.insert(make_pair(key, value));

	key = binaryStringToIntegerVector("110");
	value = binaryStringToIntegerVector("00");
	sbox.insert(make_pair(key, value));

	key = binaryStringToIntegerVector("111");
	value = binaryStringToIntegerVector("11");
	sbox.insert(make_pair(key, value));
}

//manipulations
void rotateLeft(vector<int> &input){
	vector<int> temp = input;
	for (int i = 0; i < input.size() - 1; ++i) {
		temp[i] = input[i + 1];
	}
	temp[temp.size() - 1] = input[0];
	input = temp;
}

void rotateRight(vector<int> &input){
	vector<int> temp = input;
	for (int i = 1; i < input.size(); ++i) {
		temp[i] = input[i - 1];
	}
	temp[0] = input[temp.size()-1];
	input = temp;
}

void splitWord(vector<int> input, vector<int> &a, vector<int> &b){
	long subn = input.size() / 2;
	a.resize(0);
	b.resize(0);
	for (long i = 0; i < (subn); ++i) {
		a.push_back(input[i]);
	}
	for (long i = subn; i < (input.size()); ++i) {
		b.push_back(input[i]);
	}

}

void mergeWord(vector<int> &value, vector<int> a, vector<int> b){
	value = a;
	for (int i = 0; i < b.size(); ++i) {
		value.push_back(b[i]);
	}
}

void e(vector<int> &x){
	x.push_back(x[0]);
}

vector<int> f_xor(vector<int> a, vector<int> b){
	vector<int> res = a;
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) {
			res[i] = 1;
		} else {
			res[i] = 0;
		}
	}
	return res;
}

vector<int> linear(vector<int> i){
	vector<int> ret;
	ret = sbox[i];
	return ret;
}

vector<int> f(vector<int> x, vector<int> y){
	e(x);
	vector<int> i = f_xor(x, y);
	vector<int> j = linear(i);
	rotateLeft(j);
	return j;
}



vector<int> ldes(vector<int> value, vector<int> key, bool decrypt = false){
	//get keys
	vector<int> k1(3, key[0]);
	vector<int> k2(3, key[1]);

	if (decrypt) {
		vector<int> temp = k2;
		k2 = k1;
		k1 = temp;
	}

	rotateLeft(value);
	vector<int> a0, b0;
	splitWord(value, a0, b0);

	//first
	vector<int> a1, b1;
	a1 = b0;

	b0 = f(b0, k1);
	b1 = f_xor(a0, b0);


	//second
	vector<int> a2, b2;
	a2 = b1;

	b1 = f(b1, k2);
	b2 = f_xor(a1, b1);

	vector<int> c;
	mergeWord(c, b2, a2);
	rotateRight(c);

	return c;
}

void printIntegerVector(vector<int> v){
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i];
	}
}

bool validateBinString(string k){
	for (int i = 0; i < k.size(); ++i) {
		if (k[i] != '0' && k[i] != '1') {
			return false;
		}
	}
	return true;
}

vector<int> hexToBinaryVector(char hex){
	unsigned int intval;
	stringstream ss;
	ss << hex;
	ss >> std::hex >> intval;
	string temp = bitset<4>(intval).to_string();
	return binaryStringToIntegerVector(temp);
}

char binaryToHexString(string str) {
	unsigned int temp = (unsigned int)bitset<4>(str).to_ulong();
	stringstream result;
	result << std::hex << temp;
	return result.str()[0];
}


string integerVectorToString(vector<int> in){
	string res;
	for (int i = 0; i < in.size(); ++i) {
		res += (in[i] == 0 ? '0' : '1');
	}
	return  res;
}

int main(int argc, const char * argsv[]) {
	constructSbox();

	vector<string> argv;
	for (int i = 0; i < argc; ++i) {
		argv.push_back(argsv[i]);
	}

	if (argc < 7) {
		cout << "The number of arguments provided is less than the minimum number of required arguments." << endl;
		return -1;
	}

	if (!(argv[1] == "-key") && (argv[3] == "-mode")) {
		cout << "Incorrect usage.\nYourProgram -key 10 -mode ECB -encrypt f4a5a32" << endl << "YourProgram -key 00 -mode CBC -iv 4 -decrypt b412ab2" << endl;
		return -1;
	}

	string keyStr = argv[2];

	if (!validateBinString(keyStr)) {
		cout << "Invalid key. Key needs to be in binary" << endl;
		return -1;
	}

	vector<int> key = binaryStringToIntegerVector(keyStr);

	bool isECB = true;

	if (argv[4] == "CBC") {
		isECB = false;
	} else if(argv[4] != "ECB"){
		cout << "Invalid mode. Needs to be ECB or CBC." << endl;
		return -1;
	}

	bool decrypt = false;

	vector<int> initVec;

	string hexStr;

	if (!isECB) {
		if (argc < 9){
			cout << "Incorrect number of arguments for CBC." << endl;
			return -1;
		}
		if (argv[5] != "-iv") {
			cout << "Initialisation vector not entered." << endl;
			return -1;
		} else {
			initVec = hexToBinaryVector(argv[6][0]);
		}
		if (argv[7] != "-encrypt" && argv[7] != "-decrypt") {
			cout << "Operation not specified." << endl;
			return -1;
		} else if (argv[7] == "-decrypt"){
			decrypt = true;
		}
		hexStr = argv[8];
	} else {
		if (argc < 7) {
			cout << "Incorrect number of arguments for EBC." << endl;
			return -1;
		}
		if (argv[5] != "-encrypt" && argv[5] != "-decrypt") {
			cout << "Operation not specified." << endl;
			return -1;
		} else if (argv[5] == "-decrypt"){
			decrypt = true;
		}
		hexStr = argv[6];
	}

	if (isECB) {
		for (int i = 0; i < hexStr.size(); ++i) {
			vector<int> val = hexToBinaryVector(hexStr[i]);
			vector<int> result = ldes(val, key, decrypt);
			string strval = integerVectorToString(result);
			cout << binaryToHexString(strval);
		}
		cout << endl;
	} else {
		if (decrypt) {
			for (int i = 0; i < hexStr.size(); ++i) {
				vector<int> val = hexToBinaryVector(hexStr[i]);
				vector<int> result = ldes(val, key, decrypt);
				if (i == 0) {
					result = f_xor(result, initVec);
				} else {
					vector<int> prevCipher = hexToBinaryVector(hexStr[i-1]);
					result = f_xor(result, prevCipher);
				}
				string strval = integerVectorToString(result);
				cout << binaryToHexString(strval);
			}
		} else {
			vector<int> prevVal;
			for (int i = 0; i < hexStr.size(); ++i) {
				vector<int> val = hexToBinaryVector(hexStr[i]);
				if (i == 0) {
					val = f_xor(val, initVec);
				} else {
					val = f_xor(val, prevVal);
				}
				vector<int> result = ldes(val, key, decrypt);
				prevVal = result;
				string strval = integerVectorToString(result);
				cout << binaryToHexString(strval);
			}
		}
	}

	cout << endl;

	return 0;
}
