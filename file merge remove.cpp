// 給多筆學生資料按照要求將資料合併刪除

#include <iostream>
#include <fstream> 
#include <string.h>
#include <cstdlib> 
#include <iomanip> 
#include <vector> 

using namespace std;

typedef struct cT{
	string rawR; 			// raw data of one record
	string cname; 			// college name
	string dname; 			// department name
	int sNO; 				// number of students
	int gNO; 				// number of graduates
} collegeType;

class FileIO {
	private:
		bool readFile(vector<cT>&, const char*); 	// read file
		void saveFile(const char*, vector<cT>&); 	// save file
		void Task1();								// case1
		void Task2();								// case2
		void Task3();
		void Task4();							// case3
		void removeRecords(vector<cT>&, int, int);  // remove
		void mergeData(vector<cT>&, vector<cT>&);	// merge		
	public:
		FileIO(){}	
		void menu();								//	choose implement
}; // FileIO 

bool FileIO::readFile( vector<cT> &cSet, const char* path ) { 	// read the file into the vector 
	cSet.clear();
	cT clt;
	ifstream f1(path);
	
	if( !f1.is_open() ) { 										// if the file name doesn't exist
		cout << endl << "Failed to open file." << endl;
		return false;
	} // if

	string tmp;
	if ( path[0] != 'c' ) { 									// prevent the "input" file with the title
		for( int i = 0; i < 3; i++ )
			getline( f1, tmp );
	} // if

	for( int i = 0; getline( f1, tmp ); i++ ) { 				// save a line of data into rawR
		clt.rawR = tmp;
		cSet.push_back(clt);
	} // for

	f1.close();
	f1.open(path);
	
	if ( path[0] != 'c' ) { 									// prevent the "input" file with the title
		for( int i = 0; i < 3; i++ )
			getline( f1, tmp );
	} // if

	for( int i = 0; f1 >> tmp >> clt.cname >> tmp >> clt.dname
		>> tmp >> tmp >> tmp >> tmp >> clt.sNO >> tmp >> clt.gNO >> tmp >> tmp >> tmp >> tmp; i++ ) {

	    cSet[i].cname = clt.cname; 	// save college name into cname
	    cSet[i].dname = clt.dname; 	// save department name into dname
	    cSet[i].sNO = clt.sNO;		// save student number into sNO
	    cSet[i].gNO = clt.gNO;		// save graduate numbers into gNO
	    
	} // for

	f1.close();
	return true;

} // readfile()


void FileIO::saveFile( const char* path, vector<cT> &clt ) {	// save the vector into new file
	ofstream f1;
	int count = 0;
	f1.open(path); 												// create file
	
	if( !f1.is_open() ) {  										// Failed to create file
		cout << "Create File Error" << endl;
		return;
	} // if
	for( int i = 0; i < clt.size(); i++ ) { 					// save the vector into new file  
		count++;
		f1 << clt[i].rawR << endl;
	} // for

	cout << endl << "Number of records = " << count <<endl; 	// cout the number of records
	f1.close();

} // saveFile()


void FileIO::removeRecords( vector<cT> &cSet, int sNo, int gNo ) { 		// filter out records to reduce a file

	for( int i = 0; i < cSet.size(); i++ ) {  							// check the vector from the first one
		if( cSet[i].sNO < sNo || cSet[i].gNO < gNo ) { 					// if not fulfill the stardards
	        cSet.erase( cSet.begin()+i, cSet.begin()+i+1 );				// remove the data
	        i--;
	    } // if
	}//for

} // removeRecords()


void FileIO::mergeData( vector<cT> &file1, vector<cT> &file2 ) {		// merge two sets of records

	for( int i = 0; i < file2.size(); i++ ) { 							// check the second file one by one
		int index  = 0;
		for( int n = 0; n < file1.size(); n++ ) { 						// scan the first file to find the same college name
			if ( file1[n].cname == file2[i].cname ) {
				while( file1[n].dname != file2[i].dname 
					&& n < file1.size() -1 ) n++; 						// find the same department name
				
				file1.insert( file1.begin()+n+1, file2[i] ); 			// insert the data 
				index = 1; 												// record
				break;
			}//if	
		}//for
		if ( index == 0 ) file1.insert( file1.end(), file2[i] ); 		// if do not have same college
	}// for
	
} // mergerData

void FileIO::Task1() {
	int count = 0;
    char name[300];
    char input[300] = {'i','n','p','u','t'};
    cout << endl << "Input 201, 202, ...[0]Quit): "; 	// input file name
    cin >> name;
    if ( strcmp( name, "0" ) == 0 ) return; 			// quit
    
	if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
		strcat( input, name );
		strcat( input, ".txt" );
	} // if
	else 	strcpy( input, name );						// if input full file name
	
   	ifstream ifs( input, ios::in );						// open file
   	
   	if ( !ifs.is_open() ) { 							// if file doesn't exist
    	cout << endl << "Failed to open file." << endl;
    	Task1();
	} // if
	else { 												// if file opened
		ofstream copy;
		char copyFile[300] = {'c','o','p','y'};
		strcat( copyFile, name );
		strcat( copyFile, ".txt" );
		copy.open( copyFile );
		string s;
		
    	while ( getline( ifs, s ) ) { 					// count how many lines
    		count++;
    		if ( count > 3 ) copy << s << endl; 		// save the data without first three lines
		} // while
		
		ifs.close();
		copy.close();
       	cout << endl << "Number of records = " << count - 3 << endl; // cout number of records
    } // else
}// Task1()

void FileIO::Task2() {
	char* name = new char[100], * copy = new char[100];
	char* sno = new char[100], * gno = new char[100];
	int sNo = 0, gNo = 0;
	vector<cT> clt;
	
	do{ 	// copy file read and input 
		strcpy( copy, "copy" );
		cout << endl << "Input 201, 202, ...[0]Quit): ";
		cin >> name;
		if ( strcmp( name, "0" ) == 0 ) break; 				// quit

		if ( name[0] > '0' && name[0] < '9' ) {				// if input only numbers
			strcat( copy, name );
			strcat( copy, ".txt" );
		} // if
		else strcpy( copy, name );							// if input full file name
		
	} while( !readFile( clt, copy ) );

	if ( strcmp( name, "0" ) != 0 ) { 						// if doesn't quit
		do {	// input students lower bound
			cout << endl << "Input a lower bound on the number of students: ";
			cin >> sno;				
		} while( !( sno[0] <= '9' && sno[0] >= '0' ) );
		
		sNo = atoi ( sno );

		do {	// input graduates lower bound
			cout << endl << "Input a lower bound on the number of graduates: ";
			cin >> gno;				
		} while( !( gno[0] <= '9' && gno[0] >= '0' ) );
		
		gNo = atoi( gno );
		removeRecords( clt, sNo, gNo );
		saveFile( copy, clt );
	}//if
	
}//Task2()

void FileIO::Task3(){
	char* name = new char[100];
	char* copy1 = new char[100];
	char* copy2 = new char[100];
	char* name1 = new char[100];
	char* name2 = new char[100];
	char* output = new char[100];
	vector<cT> file1;
	vector<cT> file2;
	
	do {	// file1 read and input
 		strcpy( copy1, "copy" );
 		cout << endl << "Input 201, 202, ...[0]Quit): ";
		cin >> name;

		if ( strcmp( name, "0" ) == 0 )	break;			// quit
		
		strcpy( name1, name );
		if ( name[0] >= '0' && name[0] <= '9' ) {		// if input only numbers
			strcat( copy1, name );
			strcat( copy1, ".txt" );
		} // if
		else strcpy( copy1, name );						// if input full file name
		
	} while( !readFile( file1, copy1 ) );

	if( strcmp( name, "0" ) != 0 ) { 					// if doesn't quit
		do { 	// file2 read and input
			strcpy( copy2, "copy" );
			cout << endl << "Input 201, 202, ...[0]Quit): ";
			cin >> name;

			if ( strcmp( name, "0" ) == 0 ) break;		//quit

			strcpy( name2, name );
			if ( name[0] >= '0' && name[0] <= '9' ) {	// if input only numbers
				strcat( copy2, name );
				strcat( copy2, ".txt" );
			} // if
			else strcpy( copy2, name );					// if input full file name

		} while( !readFile( file2, copy2 ) );

		if ( strcmp( name, "0" ) != 0 ) { 				// if doesn't quit
			mergeData( file1, file2 ); 					// merge
						
			strcpy( output, "output" );
			strcat( output, name1 );
			strcat( output, "_" );
			strcat( output,name2 );
			strcat( output,".txt" );
			
			saveFile( output, file1 );					// save the merged file
		}//if
	}//if
}//	Task3

void FileIO::Task4(){
	char* name = new char[100];
	char* copy1 = new char[100];
	char* copy2 = new char[100];
	char* name1 = new char[100];
	char* name2 = new char[100];
	char* output = new char[100];
	char* cnum = new char[100];
	int num = 0;
	strcpy( output, "output" );
	vector<cT> file1;
	vector<cT> file2;

	do {	// file1 read and input
 		strcpy( copy1, "copy" );
 		cout << endl << "Input 201, 202, ...[0]Quit): ";
		cin >> name;

		if ( strcmp( name, "0" ) == 0 )	break;			// quit

		strcpy( name1, name );
		if ( name[0] >= '0' && name[0] <= '9' ) {		// if input only numbers
			strcat( copy1, name );
			strcat( copy1, ".txt" );
		} // if
		else strcpy( copy1, name );						// if input full file name

	} while( !readFile( file1, copy1 ) );
	
	strcat( output, name1 );
	
	if( strcmp( name, "0" ) != 0 ) {

		do{
			cout << endl << "Input the number of files to be merged: ";
   			cin >> cnum;
		}while(!(cnum[0] <= '9' && cnum[0] >= '0'));

		num = atoi(cnum);
		
		for ( int i = 0; i < num && strcmp( name, "0" ) != 0; i++ ) {
			do { 	// file2 read and input
				strcpy( copy2, "copy" );
				cout << endl << "Input 201, 202, ...[0]Quit): ";
				cin >> name;

				if ( strcmp( name, "0" ) == 0 ) break;		//quit

				strcpy( name2, name );
				if ( name[0] >= '0' && name[0] <= '9' ) {	// if input only numbers
					strcat( copy2, name );
					strcat( copy2, ".txt" );
				} // if
				else strcpy( copy2, name );					// if input full file name

			} while( !readFile( file2, copy2 ) );
			
			if ( strcmp( name, "0" ) != 0 ) { 				// if doesn't quit
				mergeData( file1, file2 ); 					// merge
				strcat( output, "_" );
				strcat( output,name2 );
			}//if
			
		}// for
		
		strcat( output,".txt" );
		saveFile( output, file1 );			// save the merged file
	}//if
	
}//	Task4

void FileIO::menu() {
	int command = 0;
	do {
		vector<collegeType> cSet;
		cout << endl << "*** File Object Manipulator ***"; 		// introduce
 		cout << endl << "* 0. QUIT *";
 		cout << endl << "* 1. COPY (Read & Save a file) *";
 		cout << endl << "* 2. FILTER (Reduce a file) *";
 		cout << endl << "* 3. MERGE (Join two files) *";
 		cout << endl << "* 4. MERGE (Join M files)   *";
 		cout << endl << "*********************************";
 		cout << endl << "Input a choice(0, 1, 2, 3, 4): ";
 		cin >> command;
 		
 		switch ( command ) {
			case 0: break; 		// Quit
			
			case 1:
				Task1(); 		// Case1
 				break;
 				
 			case 2:
				Task2(); 		// Case2
				break;
				 				
 			case 3:
				Task3(); 		// Case3
 				break;
 				
			case 4:
				Task4();
				break;
				 	 		
 			default:	cout << endl << "Command does not exist!" << endl; // error input
				
 		} // switch
 	} while ( command != 0 );
} // menu

int main( void ){
	
	FileIO fIO; 
	fIO.menu();
 	
 	system("pause"); 	
 	return 0;
 	
} // main()
