//
//  main.cpp
//  sha-collission
//
//  Created by Telstra on 8/5/17.
//  Copyright © 2017 asjad.io. All rights reserved.
//

#include <iostream>
#include "sha1.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    string sentence = "The Cat-In-The-Hat owes Asjad ";
    string s1, s2;
    
    SHA1 h1, h2;
    int cnt = 0;
    
    for (int i = 0; i < INT_MAX; ++i) {
        for (int j = 0; j < INT_MAX; ++j) {
            s1 = sentence + to_string(i) + " dollars";
            s2 = sentence + to_string(j) + " dollars";
            h1.update(s1);
            h2.update(s2);
            const string sh1 = h1.final();
            const string sh2 = h2.final();
            cnt++;
            if ((sh1.substr(0,4) == sh2.substr(0,4)) && (i != j)) {
                cout << "Found collission! v1: " << i << "; v2: " << j << endl;
                cout << s1 << endl;
                cout << sh1 << endl;
                cout << s2 << endl;
                cout << sh2 << endl;
                
                cout << endl << "Iterations:" << cnt << endl;
                return -1;
            }
            
            
        }
    }
    return 0;
}
