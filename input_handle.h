#ifndef function_h
#define function_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std; 

void InputCSV(string name, string del, vector<vector<string>> &a );
void InputCSVtest(vector<vector<string>> data);
void string_split( string s , vector<string> & buf , string del);

#endif