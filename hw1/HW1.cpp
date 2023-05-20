# include <vector>
# include <iostream>
# include <fstream>
# include <string>


using namespace std;

ifstream InputFile;
ofstream OutputFile;

class Table {
	public :
		vector<string> tokens;
		
		Table() {
		  tokens.clear();
		  tokens.push_back("");
		} // constructor
};

class File {
	public :
	  	string inputFileName, outputFileName;
		vector<Table> TotalTable;
		bool isStr;

		File() {
			TotalTable.clear();
			
		  	for ( int a = 0 ; a <= 4 ; a++ ) { 		// 1-4 table(Preset)
		  		Table table;
				TotalTable.push_back(table);
			} // for()
			
			for ( int a = 5 ; a <= 7 ; a++ ) { 		// 5-7 table(hash)
				Table table;
				for ( int b = 0 ; b < 99 ; b++ ) { 	// mod 100 hash function
				  	table.tokens.push_back("");
				} // for()
			  
			  	TotalTable.push_back(table);
			} // for()
		} // constructor

		void readTable(string tableName) {	// 1-4 table(Preset)
			int num = tableName[5] - '0';
			string temp;
			InputFile.open(tableName.c_str());
			
			while ( getline(InputFile, temp) )
			  	TotalTable[num].tokens.push_back(temp);
	
			InputFile.close();
		} // readTable()
		
		void OpenFile() {
			while ( 1 ){
				cout << "Key in the input file name  : ";
        		cin >> inputFileName; 
        		InputFile.open(inputFileName.c_str());
        		
        		if ( !InputFile ) cout << "\n### File does not exist! ###\n\n";
				else break;  
			} // while()
		} // OpenInputFile()

		void createFile() {
			outputFileName = inputFileName;                            
			outputFileName.insert(inputFileName.size() - 4, "_output");
			OutputFile.open(outputFileName.c_str());
		} // createOutputFile()

	    void separate() {
			string temp;
			string token = "";
			isStr = false;

			while ( getline(InputFile, temp) ) {
				OutputFile << temp << "\n";
				
				for ( int a = 0 ; a < temp.size() ; a++ ) {
					if ( token == "" && temp[a] != ' ' ) 			// find the frist one
						token += temp[a];
					else if ( token == ";" && !isStr ) 				// annotation
						break;	
					else if ( isSameKind(token, temp[a]) ) 			// this char is the same kind
						token += temp[a];
					else if ( token != "" && temp[a] == ' ' ) { 	// is a string
						findCoordinate(token);
						token = "";
					} // else if()

					else if ( token != "" && !isSameKind(token, temp[a]) ) { // find the different kind of
						findCoordinate(token);
						token = "";
						a--;
					} // else if()
		    	} // for(a)
		    
		    	if ( token != "" ) {
					findCoordinate(token);
					token = "";
				} // if()
		    
		    	OutputFile << "\n"; 
		  	} // while()
		  	
		  	InputFile.close();
	  	} // separate()
	  	
	  	bool isAlphabet(char ch) { // a ~ z, A ~ Z
			if ( 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' )
				return true;
				
			else
				return false;
		} // isisAlphabet()
		
		bool isDigit(char ch) { // 0 ~ 9, A ~ H
			if ( '0' <= ch && ch <= '9' || 'A' <= ch && ch <= 'H' )
				return true;

			else
				return false;
		} // isisAlphabet()
	  	
	  	bool isSameKind(string token, char ch) {	// in the same table
			if ( isAlphabet(token[0]) && isAlphabet(ch) )
				return true;
				
			else if ( isDigit(token[0]) && isDigit(ch) )
				return true;
				
			else
				return false;
		} // isSameKind()
		
		void findCoordinate(string token) {
			if ( token == "'" && !isStr ) {			// find first '
			  isStr = true;
			  OutputFile << "(4,13)";
	    	} // if()
	    
			else if ( token == "'" && isStr ) {		// find second '
			  isStr = false;
			  OutputFile << "(4,13)";
	    	} // else if()
	    
			else {
				bool find = false;
				
			
			  	for ( int a = 1 ; a <= 7 ; a++ ) {
				  	for ( int b = 1 ; b < TotalTable[a].tokens.size() ; b++ ) {
				  
					  	if ( token == TotalTable[a].tokens[b] ) { // find token from table 1-4 
						  	OutputFile << "(" << a << "," << b << ")";
						  	find = true;
						  	break;
						} // if()
						else if ( 1 <= a && a <= 3 && token == TransLetter(TotalTable[a].tokens[b]) ) { // big and small letter
							OutputFile << "(" << a << "," << b << ")";
							find = true;
						  	break;
						} // else if
          			} // for() 
          
          			if ( find ) break;
			 	} // for() 
			  
			  	if (!find) {
					int num = findHash(token);
					OutputFile << "(" << num / 100 << "," << num % 100 << ")";
				} // if (!find)
			  
			} // else
		} // findCoordinate()
		
		int Ascii(string token) {
			int re = 0;
			
			for ( int a = 0 ; a < token.size() ; a++ )
				re += token[a];

	    return re;
		} // Ascii()

		string TransLetter(string token) { // ascii + 32
			for ( int a = 0 ; a < token.size() ; a++ )
				token[a] += 32;
				
			return token;
		} // TransLetter()

		int findHash(string token) { // table 5-7
			int indexOfTable; 
			int indexOfToken = Ascii(token) % 100;
						
			if ( isStr )						// string
				indexOfTable = 7;
			else if ( isDigit(token[0]) )		// digit
				indexOfTable = 6;
			else  								// unknown command
				indexOfTable = 5;

			while ( 1 ) {
				if ( TotalTable[indexOfTable].tokens[indexOfToken] == "" ) { 
					TotalTable[indexOfTable].tokens[indexOfToken] = token;
					return indexOfTable * 100 + indexOfToken;
				} // if ()
				
				else
					indexOfToken = ( indexOfToken + 1 ) % 100;
			} // while()
			
		} // findHash()

};


int main() {
	int command ;
	do {
		File file;
		file.readTable("Table1.table");
  		file.readTable("Table2.table");
		file.readTable("Table3.table");
		file.readTable("Table4.table");
		file.OpenFile();
  		file.createFile();
	    file.separate();
	
		cout << "\nDONE\n\n" << "continue?[press '0' to out] ";
		cin >> command ;
		cout << "\n";
	} while (command != 0);
	
	cout << "bye bye~";
} // main()
