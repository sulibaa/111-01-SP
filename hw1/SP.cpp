# include <iostream>
# include <fstream>
# include <string>
# include <vector>
using namespace std;



class OneTable {
	public :
		vector<string> tokens;

	public :
		OneTable() {
		  tokens.clear();
		  tokens.push_back("");
		} // constructor
}; // class OneTable



class File {
	public :
	  ifstream inf;
	  ofstream outf;
	  string inputFileName, outputFileName;
		vector<OneTable> allTable;

	public :
		File() {
			allTable.clear();

		  for ( int a = 0 ; a <= 4 ; a++ ) { // no.0 is useless
				OneTable table;
				allTable.push_back(table);
			} // for(a)
			
			for ( int a = 5 ; a <= 7 ; a++ ) {
				OneTable table;
				
				for ( int b = 0 ; b < 99 ; b++ ) // table.tokens.size() == 1, need to +99
				  table.tokens.push_back("");
			  
			  allTable.push_back(table);
			} // for(a)
		} // File()

		void readTable(string tableName) {
			int no = tableName[5] - '0';
			string token;
			inf.open(tableName.c_str());
			
			while ( getline(inf, token) )
			  allTable[no].tokens.push_back(token);
			
			inf.close();
		} // readTable()
		
		void OpenInputFile() {
			do { // is the file exist ?
        cout << "Key in the input file name  : ";
        cin >> inputFileName;
        inf.open(inputFileName.c_str()); // try to open file
        
        if ( !inf )
				  cout << "\n### File does not exist! ###\n\n";
				  
      } while (!inf); // while (!inf)
		} // OpenInputFile()

		void createOutputFile() {
			outputFileName = inputFileName;                             //    ex: input = 1.txt
			outputFileName.insert(inputFileName.size() - 4, "_output"); // then, output = 1_output.txt
			outf.open(outputFileName.c_str());
		} // createOutputFile()

}; // class File



class Token{
	public :
		bool isString;

	public :
	  Token(File* file) {
			string line, token = "";
			isString = false;

			while ( getline(file->inf, line) ) {
				file->outf << line << "\n";
				
				for ( int a = 0 ; a < line.size() ; a++ ) {
					if ( token == "" && line[a] != ' ' ) // find the first char of token
						token += line[a];
						
					else if ( token == ";" && !isString ) // comment
						break;
						
					else if ( isTokensSame(token, line[a]) ) // same token
						token += line[a];

					else if ( token != "" && line[a] == ' ' ) { // bump into ' ', cut token
						findTokensHouse(token, file);
						token = "";
					} // else if()

					else if ( token != "" && !isTokensSame(token, line[a]) ) { // different token bumped into, cut token
						findTokensHouse(token, file);
						token = "", a--;
					} // else if()
		    } // for(a)
		    
		    if ( token != "" ) {
					findTokensHouse(token, file);
					token = "";
				} // if()
		    
		    file->outf << "\n"; // all of coordinate in this line is output
		  } // while(getline(inf, line))
		  
	  } // Token()

		bool isAlphabet(char ch) { // Alphabet = a ~ z, A ~ Z
			if ( 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' )
				return true;
				
			else
				return false;
		} // isisAlphabet()
		
		bool isDigit(char ch) { // Digit = 0 ~ 9, A ~ H
			if ( '0' <= ch && ch <= '9' || 'A' <= ch && ch <= 'H' )
				return true;

			else
				return false;
		} // isisAlphabet()
		
		bool isTokensSame(string token, char ch) {
			if ( isAlphabet(token[0]) && isAlphabet(ch) )
				return true;
				
			else if ( isDigit(token[0]) && isDigit(ch) )
				return true;
				
			else
				return false;
		} // isTokensSame()
		
		void findTokensHouse(string token, File* file) {
			if ( token == "'" && !isString ) {
			  isString = true;
			  file->outf << "(4,13)";
	    } // if()
	    
			else if ( token == "'" && isString ) {
			  isString = false;
			  file->outf << "(4,13)";
	    } // else if()
	    
			else {
				bool find = false;
			
			  for ( int a = 1 ; a <= 7 ; a++ ) {
				  for ( int b = 1 ; b < file->allTable[a].tokens.size() ; b++ ) {
				  
					  if ( token == file->allTable[a].tokens[b] || // token = MOV
						     1 <= a && a <= 3 && token == TransToSmallLetter(file->allTable[a].tokens[b]) ) { // token = mov
						  file->outf << "(" << a << "," << b << ")";
						  find = true;
						  break;
						} // if()
          } // for(b)
          
          if ( find ) break;
			  } // for(a)
			  
			  if (!find) {
					int index = findHash(token, file);
					file->outf << "(" << index / 100 << "," << index % 100 << ")";
				} // if (!find)
			  
			} // else
		} // findTokensHouse()
		
		int TransToAscii(string token) {
			int re = 0;
			
			for ( int a = 0 ; a < token.size() ; a++ )
				re += token[a];

	    return re;
		} // TransToAscii()

		string TransToSmallLetter(string token) { // MOV -> mov
			for ( int a = 0 ; a < token.size() ; a++ )
				token[a] += 32;
				
			return token;
		} // TransToSmallLetter()

		int findHash(string token, File* file) {
			int re, indexOfTable = TransToAscii(token) % 100;
						
			if ( isString )
				re = 7;
			else if ( isDigit(token[0]) )
				re = 6;
			else  // instruction, ex : MAIN
				re = 5;

			while (1) {
				if ( file->allTable[re].tokens[indexOfTable] == "" ) {  // now, re is the table's NO.
					file->allTable[re].tokens[indexOfTable] = token;
					return re * 100 + indexOfTable;
				} // if ()
				
				else
					indexOfTable = ( indexOfTable + 1 ) % 100;
			} // while (1)
			
		} // whichTable_5to7()
		
}; // class Token


int main() {
	File file;
	file.readTable("Table1.table");
  file.readTable("Table2.table");
	file.readTable("Table3.table");
	file.readTable("Table4.table");
	file.OpenInputFile();
  file.createOutputFile();
  
	Token token(&file);
	
	cout << "Done ~";
} // main()
