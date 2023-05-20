# include <vector>
# include <iostream>
# include <fstream>
# include <string>
# include <iomanip>
# include <algorithm>

using namespace std;

ifstream InputFile;
ofstream OutputFile;

struct Label {	// forward referance
	string label ;
	int position ;
};

struct Code { 	// final output
	int position ;
	vector<string> Mcode ;
	int McodeLength ;
	string line ;
	
	bool have_Label ;
	bool FR ;	// forward referance
	string instruction, operand1, operand2 ;
	
};

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
};

class Token{
	public :
		int position ;
		vector<Label> AllLabel ;	// forward referance
		vector<Code> AllCode ;		// final output
		Label tempLabel ;
		Code tempCode ;
		bool isString ;
		
		
		Token(File* file) {
			string line, token = "", instruction = "", operand1 = "", operand2 = "";
            bool cutdone = false, isLabel = false, isInstruction = false;
            int tablenum ; 
			position = 0;

			while ( getline(InputFile, line) ) {
				tempCode.McodeLength = 0 ;
				tempCode.line = line ;
				tempCode.Mcode.clear() ;
				tempCode.have_Label = false ;
				tempCode.FR = false ;
				tempCode.instruction = tempCode.operand1 = tempCode.operand2 = "" ;
				tempCode.position = position ;
				isString = false ;
				for ( int a = 0 ; a < line.size() ; a++ ) {
					if ( token == "" && line[a] != ' ' && line[a] != '\t' ){ // find the first char of token
						token += line[a];
						if ( line[a] == '\'' ) isString = true ;
						
						if ( a == 0 ) {
							isLabel = true ;
							tempCode.have_Label = true ;
						} // if()
					} // if()
					
					else if ( token == ";" && !isString ) // a comment break out
					  	break ;
					
					else if ( token != "" && line[a] != ' ' && line[a] != '\t' && line[a] != ',' && !isString ) // same token continue adding
						token += line[a];
						
					else if( isString && line[a] != '\'' ) // in the string
					 	token += line[a];
					 	
					else if( isString && line[a] == '\'' ) { // string end
						token += line[a];
						cutdone = true ;
					} // else if()

					else if ( token != "" && ( line[a] == ' ' || line[a] == '\t' || line[a] == ',' ) ) { // token end cut it
						cutdone = true ;
					} // else if()
					
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////// finish cutting
					
					if( cutdone ){
						tablenum = findTable( token, file ) ;
						if ( isLabel ) { // remenber label position instruction need it
							bool samelabel = false ;
							for ( int i = 0 ; i < AllLabel.size() ; i++ ){
								if ( token == AllLabel[i].label ) {
									samelabel = true ;
									break ;
								} // if() 
							} // for
							
							if ( !samelabel ){
								tempLabel.label = token ;
								tempLabel.position = position ;
								AllLabel.push_back( tempLabel ) ;
							} // if()
							
							isLabel = false ;
							
						} // if()
						else if ( !isInstruction ) {
						    if ( tablenum == 1 ) {
								instruction = token;
							    isInstruction = true;
                			} // if()
						    else if ( tablenum == 6 || tablenum == 7 ) // find digit or string turn to mcode and get new position
								SandD_to_mcode_and_newmposition(token, file);
					    } // if ()

					    else { // instruction have 1-2 operand ( only table 3,5,6 has chance to be operand
							  if ( tablenum == 3 || tablenum == 5 || tablenum == 6 ) { 
								  if ( operand1 == "" ) operand1 = token;
								  else operand2 = token;
							  } // if()
					    } // else ()
					    
					    token = "";
					    cutdone = false;
					    isString = false ;
					} // if()
					
		    	} // for()
		    	
		    	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// last one token
		    
		    	if ( token != "" ) {  
					tablenum = findTable( token, file ) ;

					if ( tablenum == 1 ) {
						instruction = token;
						isInstruction = true;
            		} // if()
					else if ( isInstruction && ( tablenum == 3 || tablenum == 5 || tablenum == 6 ) ) { // instruction second operand ( only table 3,5,6 has chance to be operand
					 	if ( operand1 == "" ) operand1 = token;
						else operand2 = token;
					} // else if()
					else if ( tablenum == 6 || tablenum == 7 ) SandD_to_mcode_and_newmposition(token, file);
					token = "";
				} // if()

				if ( isInstruction ) Instruction_to_mcode_and_newposition(instruction, operand1, operand2, file, false, 0);

		      	AllCode.push_back(tempCode);
		      	instruction = operand1 = operand2 = "";
		      	isInstruction = false;
		      	isString = false ;
		      	
		      	/*
		      	cout << tempCode.position << "  " ;
		      	for( int i = 0 ; i < tempCode.Mcode.size() ; i++ )
		      	  	cout << tempCode.Mcode[i] << " ";
		      	cout << tempCode.line ; 
		        system("pause");
						    */
		  	} // while()
		  	
		  	createfinalFile(file) ;
		  
	 	 } // Token()
	  
	  
	  	int findTable(string token, File* file) {
			if ( isString )
				return 7;
			else { 
			  	for ( int a = 1 ; a <= 4 ; a++ )  // check if it's in 1-4 table
				  	for ( int b = 1 ; b < file->TotalTable[a].tokens.size() ; b++ ) {
					  	if ( token == file->TotalTable[a].tokens[b] ||  1 <= a && a <= 3 && token == TransLetter(file->TotalTable[a].tokens[b] ) )  {
						  	return a ; 
					} // for()
  				} // for()

				if ( token.length() >= 3 && isDigit(token[0]) && token[token.length() - 1] == 'H' ) return 6 ;
				else return 5 ;
			} // else()
		} // findTable()
		
		string CharToInt16_reString(char c) { // char turn in to Mcode EX: 'G' -> "47" 
   			int num1 = int(c) / 16, num2 = int(c) % 16;
   			string snum1 = "@", snum2 = "@";
   			snum1[0] = num1 + '0';
     
   			if ( 0 <= num2 && num2 <= 9 )
    			snum2[0] = num2 + '0';
   			else
    			snum2[0] = num2 - 10 + 'A';
   
   			return snum1 + snum2;
  		} // CharToInt16_reString()
  		
  		int Ascii(string token) {
			int re = 0;
			
			for ( int a = 0 ; a < token.size() ; a++ )
				re += token[a];

	    return re;
		} // Ascii()
  		
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
		
		string TransLetter(string token) { // ascii + 32
			for ( int a = 0 ; a < token.size() ; a++ )
				token[a] += 32;
				
			return token;
		} // TransLetter()
		
		void SandD_to_mcode_and_newmposition(string str, File* file) {
			if ( isDigit(str[0]) && str[str.size() - 1] == 'H' ) { // is a digit
				position++; 
				tempCode.McodeLength += str.size() ;
				tempCode.Mcode.push_back(str.substr(0, str.size() - 1)) ; // dont need "H"
			} // if ()
			else {                                           // is a string
			  for ( int a = 1 ; a < str.size()-1 ; a++ ) { // dont need "'"
				  position++;
				  tempCode.McodeLength += 3 ;
				  tempCode.Mcode.push_back(CharToInt16_reString(str[a])) ;
			  } // for()
			} // else
		} // SandD_to_mcode_and_newmposition()
  		
  		void Instruction_to_mcode_and_newposition(string instruction, string operand1, string operand2, File* file, bool fowardreferance, int frposition) {
  			////////////////////////////////////////////////////////////////////////////// INT
			if ( instruction == "INT" ) {
				tempCode.McodeLength += 3;
				tempCode.Mcode.push_back("CD");
				position++; 
				SandD_to_mcode_and_newmposition(operand1, file);
			} // if()
			////////////////////////////////////////////////////////////////////////////// RET
			else if ( instruction == "RET" ) {
				tempCode.McodeLength += 3; 
				tempCode.Mcode.push_back("C3");
				position++;
			} // else if()
			////////////////////////////////////////////////////////////////////////////// CALL
			else if ( instruction == "CALL" ) {
				int Lposition = -1;
				for ( int i = 0 ; i < AllLabel.size() ; i++ ){
					if ( operand1 == AllLabel[i].label ) Lposition = AllLabel[i].position ;
				} // for()
				
				if ( Lposition != -1 ) {
          			if ( fowardreferance ) { // pass2
						AllCode[frposition].Mcode.push_back("E8");
						AllCode[frposition].Mcode.push_back(int10_to_string16(Lposition - AllCode[frposition].position - 3, true));
					} // if()
					else { // pass1
						tempCode.Mcode.push_back("E8");
						tempCode.Mcode.push_back(int10_to_string16(Lposition - position - 3, true)); 
					} // else
				} // if ()
				else { // forward reference
				  	tempCode.FR = true ;
					tempCode.instruction = "CALL";
					tempCode.operand1 = operand1;
				} // else()
				
				position += 3, tempCode.McodeLength += 8;
			} // else if ()
			////////////////////////////////////////////////////////////////////////////// MOV
			else if ( instruction == "MOV" ) {
				int operand1_type = findTable( operand1, file ) ;
				int operand2_type = findTable( operand2, file ) ;
				int op1position = -1;
				for ( int i = 0 ; i < AllLabel.size() ; i++ ){
					if ( operand1 == AllLabel[i].label ) op1position = AllLabel[i].position ;
				} // for()
				int op2position = -1;
				for ( int i = 0 ; i < AllLabel.size() ; i++ ){
					if ( operand2 == AllLabel[i].label ) op2position = AllLabel[i].position ;
				} // for()
			
				// Symbol to Accumulator
				if ( ( operand1 == "AX" || operand1 == "AH" || operand1 == "AL" ) && operand2_type == 5 ) { 
					if ( operand1 == "AX" ) 
						tempCode.Mcode.push_back("A1"); 
					else                    
						tempCode.Mcode.push_back("A0");

					tempCode.Mcode.push_back(int10_to_string16(op2position, true));
					tempCode.Mcode.push_back("R");
					position += 3;
					tempCode.McodeLength += 10;
				} // if ()
				
				// Symbol to Register
				else if ( operand1_type == 3 && operand2_type == 5 ) { 
					int kindOfRegister = instruction_mov_kind(operand1);
					
					if ( kindOfRegister % 10 == 0 ) 
						tempCode.Mcode.push_back("8A");
					else                            
						tempCode.Mcode.push_back("8B"); 
			
					tempCode.Mcode.push_back(int10_to_string16(kindOfRegister / 10 * 8 + 6, false)); 
					tempCode.Mcode.push_back(int10_to_string16(op2position, true));
					tempCode.Mcode.push_back("R");
					position += 4; 
					tempCode.McodeLength += 13;
				} // else if ()
				
				// Register to segment Register
				else if ( ( operand1 == "CS" || operand1 == "DS" || operand1 == "ES" || operand1 == "SS" ) && operand2_type == 3 ) { 
					int Mcode_base10 = 192; // turn to base10 ( 6 * 2^5
					int r1_type = instruction_mov_kind(operand1); 
					int r2_type = instruction_mov_kind(operand2);
					Mcode_base10 += r1_type % 10 * 8 + r2_type / 10;
					tempCode.Mcode.push_back("8E");
					string tempstr = int10_to_string16(Mcode_base10, false);
					reverse(tempstr.begin(),tempstr.end());
					tempCode.Mcode.push_back(tempstr);
					position += 2;
					tempCode.McodeLength += 6;
				} // else if ()
				
				// Immediate to Register
				else if ( operand1_type == 3 && operand2_type == 6 ) { 
					int Mcode_base10 = 176; // turn to base10 ( 11 * 2^4
					int r_type = instruction_mov_kind(operand1);
					
					if ( r_type % 10 == 1 ) Mcode_base10 += 8;
					Mcode_base10 += r_type / 10;
					string tempstr = int10_to_string16(Mcode_base10, false);
					reverse(tempstr.begin(),tempstr.end());
					tempCode.Mcode.push_back(tempstr);
					
					if ( Mcode_base10 >= 184 ) {
					  	tempCode.Mcode.push_back(operand2.substr(2, 2)); // switch the hl lw
					  	position++;
					  	tempCode.McodeLength += 3;                
					} // if ()
					
					tempCode.Mcode.push_back(operand2.substr(0, 2));                                 
					position += 2, 
					tempCode.McodeLength += 6;
				} // else if ()
			} // else if ()
			
		} // Instruction_to_mcode_and_newposition()
		
		string int10_to_string16(int n, bool to4digit) { // change base10 int -> base16 string
      		int temp;
      		char ctemp;
      		string re = "";
      		bool negative = false;
      		if ( n < 0 ) negative = true;

			if ( !negative ) {
			  	do {
          			temp = n % 16;

				 	if ( 0 <= temp && temp <= 9 ) ctemp = temp + '0';
          			else ctemp = temp - 10 + 'A';

				  	re += ctemp;
					n /= 16;
        		} while ( n );
        
        		if ( to4digit ) {
		  	  		temp = re.length();
		      		for ( int a = 1 ; a <= 4 - temp ; a++ ) re = "0" + re; // + "0" to 4 digit
		    	} // if ()
			} // if()
			else {
				int binary[16] = {0}, n = -n ; 
				for ( int i = 15 ; n > 0 ; i-- ) { 
					if ( n % 2 == 1 ) binary[i] = 1;
				  	n /= 2;
        		} // for()

				// 1st Complement
				for ( int a = 0 ; a < 16 ; a++ ) {  
					if ( binary[a] == 0 ) binary[a] = 1;
				  	else binary[a] = 0;
        		} // for()

                // 2nd Complement
        		int carry = 1;                         
				for ( int a = 15 ; a >= 0 ; a-- ) {
					if ( binary[a] + carry == 1 ) {
						binary[a] = 1 ;
						carry = 0;
					} // if ()
					else if ( binary[a] + carry == 2 ) {
						binary[a] = 0 ;
						carry = 1;
					} // else if()
				} // for()
				
				// base2 -> base16
				for ( int a = 0 ; a <= 12 ; a = a + 4 ) {
				  	temp = binary[a] * 8 + binary[a+1] * 4 + binary[a+2] * 2 + binary[a+3];
          			if ( 0 <= temp && temp <= 9 ) ctemp = temp + '0';
          			else ctemp = temp - 10 + 'A';
          			re += ctemp;
        		} // for()
			} // else
			
      		return re;
    	} // int10_to_string16()
		
		int instruction_mov_kind(string str) {
			int re;
			if      ( str == "AL" || str == "AX" ) 
				re =  0;
			else if ( str == "CL" || str == "CX" ) 
				re = 10;
			else if ( str == "DL" || str == "DX" ) 
				re = 20;
			else if ( str == "BL" || str == "BX" ) 
				re = 30;
			else if ( str == "AH" || str == "SP" ) 
				re = 40;
			else if ( str == "CH" || str == "BP" ) 	
				re = 50;
			else if ( str == "DH" || str == "SI" ) 
				re = 60;
			else if ( str == "BH" || str == "DI" ) 
				re = 70;
			else if ( str == "ES" || str == "CS" || str == "SS" || str == "DS" ) 
				re = 80;
				
            ////////////////////////////////////////////////////////////
			 
			if      ( str == "AL" || str == "CL" || str == "DL" || str == "BL" || str == "AH" || str == "CH" || str == "DH" || str == "BH" )  
				re += 0;
			else if ( str == "AX" || str == "CX" || str == "DX" || str == "BX" || str == "SP" || str == "BP" || str == "SI" || str == "DI" )  		  
				re += 1;
			else if ( str == "ES" )                                                              
				re += 0;
			else if ( str == "CS" )                                                              
				re += 1;
			else if ( str == "SS" )                                                              
				re += 2;
			else if ( str == "DS" )                                                              
				re += 3;
			
			return re;
		} // instruction_mov_kind()
		
		
		void createfinalFile(File* file) {
			file->outputFileName = file->inputFileName;                            
			file->outputFileName.insert(file->inputFileName.size() - 4, "_output");
			OutputFile.open(file->outputFileName.c_str());

	    	for ( int i = 0 ; i < AllCode.size() ; i++ ) {
				if ( AllCode[i].have_Label  || AllCode[i].Mcode.size() != 0 || AllCode[i].instruction != "" )
				 	OutputFile << " " << std::hex << std::uppercase << right << setw(4) << setfill('0') << AllCode[i].position << "    ";
				else
					OutputFile << "         "; 
					
				/////////////////////////////////////////////////////////////
				
				if ( AllCode[i].FR )
					Instruction_to_mcode_and_newposition(AllCode[i].instruction, AllCode[i].operand1, AllCode[i].operand2, file, AllCode[i].FR, i);
				
				for ( int j = 0 ; j <  AllCode[i].Mcode.size() ; j++ ) 
					OutputFile << AllCode[i].Mcode[j] << " ";
				for ( int j = 1 ; j <= 26 - AllCode[i].McodeLength ; j++ ) 
					OutputFile << " "; 
				OutputFile << "\t";
				/////////////////////////////////////////////////////////////
				
				OutputFile << AllCode[i].line << "\n";
			} // for ()
			
			OutputFile.close();
    	} // createfinalFile()
    	
};
	
int main() {
	File file;
	file.readTable("Table1.table"); // instruction
  	file.readTable("Table2.table"); // pseudo
	file.readTable("Table3.table"); // register
	file.readTable("Table4.table"); // dilimiter
	file.OpenFile();
  	Token token(&file);
	cout << "\nDONE\n";
} // main()
