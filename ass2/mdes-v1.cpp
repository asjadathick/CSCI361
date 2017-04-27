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



vector<int> ldes(vector<int> value, vector<int> key, bool decrypt = false, bool ecb = true, vector<int> iv = vector<int>()){
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


int main(int argc, const char * argv[]) {
	constructSbox();
	for (int i = 0; i < 4; ++i) {
		vector<int> key = binaryStringToIntegerVector(bitset<2>(i).to_string());

		for (int j = 0; j < 16; ++j) {
			vector<int> value = binaryStringToIntegerVector(bitset<4>(j).to_string());
			vector<int> result = ldes(value, key);
			cout << "Key: ";
			printIntegerVector(key);
			cout << " Value: ";
			printIntegerVector(value);
			cout << " Encrypted: ";
			printIntegerVector(result);
			result = ldes(result, key, true);

			/*
			 cout << " Decrypted: ";
			 printIntegerVector(result);
			*/
			cout << endl;
		}
		cout << "-----------------------------------" << endl;
	}
	return 0;
}
