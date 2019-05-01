#include"input_handle.h"

void string_split( string s , vector<string> & buf , string del){
	char * delimiter = new char [del.length()+1];
	strcpy (delimiter, del.c_str());
	int current = 0;
	int next;
	
	while(1)
	{
		next = s.find_first_of( delimiter , current );
		if (next != current)
		{
			string tmp = s.substr(current, next - current);
			if (tmp.size() != 0) 
				buf.push_back(tmp);
		}
		if (next == string::npos) break;
		current = next + 1; 
	}

}


void InputCSV(string name, string del, vector<vector<string>> &a ){
	ifstream CSVdata( name );

	
	string s ;
    while( getline(CSVdata, s) ){
    	vector<string> buf ;
    	string_split(s,buf,del);
		a.push_back(buf);
    }
}

void InputCSVtest(vector<vector<string>> data){
	cout << data.size() << " " << data[0].size() << endl ;
	for ( int i = 0 ; i < data.size() ; i++ ){
		for ( int j = 0 ; j < data[i].size() ; j++ ){
			cout << data[i][j] << " " ;
		}
		cout << endl ;
	}
}