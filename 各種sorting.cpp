/*
就是各種sorting對下面資料進行排序 ->是否stable & 時間(效能)

data file：
大專校院各校科系別概況										
105 學年度 SY2016-2017										
學校代碼	學校名稱	科系代碼	科系名稱	日間∕進修別	等級別	學生數	教師數	上學年度畢業生數	縣市名稱	體系別
0002	國立清華大學	520114	資訊工程學系	D 日	B 學士	565	43	117	18 新竹市	1 一般
0003	國立臺灣大學	520114	資訊工程學系	D 日	B 學士	520	30	104	30 臺北市	1 一般
0007	國立交通大學	520114	資訊工程學系	D 日	B 學士	788	24	187	18 新竹市	1 一般
0008	國立中央大學	520114	資訊工程學系	D 日	B 學士	475	28	109	03 桃園市	1 一般
0009	國立中山大學	520114	資訊工程學系	D 日	B 學士	234	20	44	50 高雄市	1 一般
.........

*/
#include <iostream> // cout, endl
#include <fstream> // open, is_open
#include <string.h> // string
#include <vector> // vector
#include <cstdlib> // atoi, system
#include <iomanip> // setw, setprecision
#include <ctime> // clock, CLOCKS_PER_SEC
#include <cmath> // float
using namespace std;
time_t cT;

typedef struct Time{
	char* name;
	float stime;
	float btime;
	float mtime;
	float qtime;
	float rtime;
} Time;

class classList{
	typedef struct cT{
		string rawR; // raw data of one record
		int gNO; // number of graduates
	} collegeType;
	
	vector<collegeType> cSet; // set of output records
	
 	const char* fileNO; // number to form a file name
 	const char* sortNAME; // name of a sorting algorithm
 	clock_t sortTIME; // sorting time

 	void mergeMS(int, int, int ); // merge two halves
 	void recurMS( ); // recursive version of merge sort
 	void partQS( int, int, int&); // create two partitions by the pivot
 	void recurQS( ); // recursive version of quick sort
	void iterRS( ); // iterative version of radix sort

	public:
		classList(): fileNO(""), sortNAME(""), sortTIME(0){

		} // constructor: do nothing
		int size() { return cSet.size();}
		void swap(collegeType&, collegeType&);
		bool readF(const char*); // read records from a file
		void show(); // display the entire set of records on screen
		void saveF(const char*); // save all records into a file
		void ssort(); // selection sort
		void bsort(); // bubble sort
		void msort(int, int); // merge sort
		void qsort(int, int); // quick sort
		void rsort(); // radix sort
		void saveTime(char*, float, float, float, float, float);
		void clearUp() {
			cSet.clear();
			delete fileNO;
		} // erase the object content

		~classList() { clearUp(); } // destructor: destroy the object
}; // class classList

bool classList::readF(const char* path){

	cSet.clear();
	cT clt;
	string tmp;


	ifstream f1(path);


	if( !f1.is_open() ) {									// if the file name doesn't exist
		cout << endl << "Failed to open file." << endl;
		return false;
	} // if

	for( int i = 0; i < 3; i++ )			getline( f1, tmp );
	
	for( int i = 0; getline( f1, tmp ); i++ ) { 				// save a line of data into rawR
		clt.rawR = tmp;
		cSet.push_back(clt);
	} // for

	f1.close();
	f1.open(path);
							
	for( int i = 0; i < 3; i++ )	getline( f1, tmp );


	for( int i = 0; f1 >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >>
					tmp >> tmp >> clt.gNO >> tmp >> tmp >> tmp >> tmp; i++ ) {
	    cSet[i].gNO = clt.gNO;		// save graduate numbers into gNO
	} // for

	f1.close();
	return true;
	
}//read

void classList::saveF( const char* path) {	// save the vector into new file
	ofstream f1;

	int count = 0;
	f1.open(path); 												// create file

	if( !f1.is_open() ) {  										// Failed to create file
		cout << "Create File Error" << endl;
		return;
	} // if
	
	for( int i = 0; i < cSet.size(); i++ ) { 					// save the vector into new file
		f1 << cSet[i].rawR << endl;
	} // for

	f1.close();

} // saveFile()

void classList::swap(collegeType& c1, collegeType& c2){
 	collegeType temp;

	temp.rawR = c1.rawR;
	c1.rawR = c2.rawR;
	c2.rawR = temp.rawR;

	temp.gNO = c1.gNO;
	c1.gNO = c2.gNO;
	c2.gNO = temp.gNO;
	
}

void classList::ssort() {

	for (int i = 0; i < cSet.size() - 1; i++) {
		int max = i;
		for (int j = i + 1; j < cSet.size(); j++){
			if (cSet[j].gNO > cSet[max].gNO )		max = j;
		}


		if(max != i)	swap(cSet[i], cSet[max]);

	}
	
}

void classList::bsort() { 

	for (int i = cSet.size() - 1; i > 0; i--) {
		for (int j = 0; j < i; j++)
			if (cSet[j + 1].gNO > cSet[j].gNO) {
				swap(cSet[j], cSet[j + 1]);
			}
	}
	
}


void classList::msort( int first, int last ) {
	if ( first < last ) {
		int mid = (first + last)/2;
		msort( first, mid );
		msort( mid+1, last );
		mergeMS( first, mid, last );
	} // if
} // msort

void classList::mergeMS( int first, int mid, int last ){
	int head1 = first, tail1 = mid;
	int head2 = mid+1, tail2 = last;
	int i = first;
	vector<collegeType> ans = cSet;

	for ( i = first; ( head1 <= tail1 ) && ( head2 <= tail2 ); i++ ) {
		if ( cSet[head1].gNO >= cSet[head2].gNO ) {
			swap( cSet[head1], ans[i] );
			head1++;
		} // if
		else {
			swap( cSet[head2], ans[i] );
			head2++;
		} // else
	} // for

	while( head1 <= tail1 ) {
		swap( cSet[head1], ans[i] );
		head1++;
		i++;
	} // while
	while( head2 <= tail2 ) {
		swap( cSet[head2], ans[i] );
		head2++;
		i++;
	} // while

	for ( i = first; i <= last; i++ ) {
		swap( cSet[i], ans[i] );
	} // for
} // mergeMS

void classList::qsort( int first, int last ) {
	int pivotIndex;
	if ( first < last ) {
		partQS( first, last, pivotIndex );
		qsort( first, pivotIndex-1 );
		qsort( pivotIndex+1, last );
	} // if

} // qsort

void classList::partQS( int first, int last, int &pIndex) {
	int lastS1 = first;
	pIndex = first;
	for ( int i = first+1; i <= last; i++ ) {
		if ( cSet[i].gNO > cSet[pIndex].gNO ) {
			lastS1++;
			swap( cSet[i], cSet[lastS1] );
		} // if
	} // for
	swap( cSet[lastS1], cSet[pIndex] );
	pIndex = lastS1;
} // partQS



void classList::rsort(){
	vector< vector<collegeType> > list;

	vector<collegeType> temp;
	
	int max = 0, digit = 1, num = 0, index = 0, m = 1;

	for (int i = 1; i < cSet.size() - 1; i++) {
		if (cSet[i].gNO > cSet[max].gNO )		max = i;
	} //for
	
	num = cSet[max].gNO;

	while( num > 9){
		num = num / 10;
		digit ++;
	}
	

	for(int i = 1; i <= digit; i++){
		
		list.clear();
		
		//for(int j = 0 ; j<list.size(); j++)     list[j].clear();

		for(int j = 0 ; j < cSet.size(); j++){
			index = (cSet[j].gNO / m) % 10;
			
			
			int bonk = 0;
			
			if (list.size() == 0){
			
				temp.push_back(cSet[j]);
				list.push_back(temp);
				temp.clear();
			}
			
			else{

				for(int n = 0; n < list.size() ; n++){

					int x = (list[n][0].gNO / m) % 10;

					if(index == x){
						list[n].push_back(cSet[j]);
						bonk = 1;
	  					break;
					}//if

					else if (index > x){
						temp.push_back(cSet[j]);
						list.insert(list.begin()+n, temp);

						temp.clear();
						bonk = 1;
	  					break;
					}//else if

					
				}//for

			
				if(bonk == 0){
					temp.push_back(cSet[j]);
					list.push_back(temp);
					temp.clear();
				}//if

			}//else
			
		}//for

		
		cSet.clear();

		for(int j = 0 ; j < list.size(); j++){
			for(int n = 0; n < list[j].size(); n++){
				cSet.push_back(list[j][n]);
			}//for
		}//for-

		
		m = m *10;
		
	}//for
}

void classList::saveTime(char* name, float stime, float btime, float mtime, float qtime, float rtime) {
	ifstream f2("sort_time.txt");
	int type = 0;
	if( !f2.is_open() ) {									// if the file name doesn't exist
		type = 1;
	} // if
	
    fstream f1;
	f1.open("sort_time.txt", ios::app);

	if (type) f1 << "�ɮ׽s��\t��ܱƧ�\t��w�Ƨ�\t�X�ֱƧ�\t�ֳt�Ƨ�\t��ƱƧ�" << endl;
    
	f1 << name << "\t" << stime	<< "\t"	<< btime << "\t" << mtime
		<< "\t" << qtime<< "\t" << rtime << endl ;


	f1.close();

} // saveTime

int main(void){
	vector<Time> time;
	Time temp;
	int command = 0; // user command
	classList list;
	do{
	    char name[300];
	    char input[300] = {'i','n','p','u','t'}, sel[300] = {'s','e','l','e','c','t','_','s','o','r','t'};
	    char bub[300] = {'b','u','b','b','l','e','_','s','o','r','t'}, rad[300] ={'r','a','d','i','x','_','s','o','r','t'};
	    char mer[300] = {'m','e','r','g','e','_','s','o','r','t'}, qui[300] = {'q','u','i','c','k','_','s','o','r','t'};
		float stime=0, btime = 0, rtime = 0;
		float mtime = 0, qtime = 0;
		
		cout << endl << "******** Sorting Algorithms *********";
		cout << endl << "* 0. Quit *";
		cout << endl << "* 1. Selection sort vs. Bubble sort *";
		cout << endl << "* 2. Merge sort vs. Quick sort      *";
		cout << endl << "* 3. Radix sort                     *";
		cout << endl << "* 4. Comparisons on five methods    *";
		cout << endl << "*******************************";
		cout << endl << "Input a command(0, 1, 2, 3, 4): ";
		cin >> command; // get the command

		switch (command){

			case 0: break;

			case 1:
				cout << "Input 501, 502, ...[0]Quit): ";
				cin >> name;
				if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( input, name );
					strcat( input, ".txt" );
				} // if
				else 	strcpy( input, name );						// if input full file name
				
				if(list.readF(input)){
					cT = clock();
					list.ssort();
					cT = clock() - cT;
					stime = (float)cT*1000/CLOCKS_PER_SEC;
					strcat( sel, name );
					strcat( sel, ".txt" );
					list.saveF(sel);
					
				} 
				
				if(list.readF(input)){
					cT = clock();
					list.bsort();
					cT = clock() - cT;
					btime = (float)cT*1000/CLOCKS_PER_SEC;
					strcat( bub, name );
					strcat( bub, ".txt" );
					list.saveF(bub);
				}

				cout << "Selection sort: " << stime << " ms"<< endl;
				cout << "Bubble sort: " << btime << " ms" << endl;

				break;

			case 2:
				cout << "Input 501, 502, ...[0]Quit): ";
				cin >> name;
				if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( input, name );
					strcat( input, ".txt" );
				} // if
				else 	strcpy( input, name );						// if input full file name
				
				if(list.readF(input)){
					cT = clock();
					list.msort( 0, list.size()-1 );
					cT = clock() - cT;
					mtime = (float)cT*1000/CLOCKS_PER_SEC;
					strcat( mer, name );
					strcat( mer, ".txt" );
					list.saveF(mer);
					
				}
			
				
				if(list.readF(input)){
					cT = clock();
					list.qsort( 0, list.size()-1 );
					cT = clock() - cT;
					qtime = (float)cT*1000/CLOCKS_PER_SEC;
					strcat( qui, name );
					strcat( qui, ".txt" );
					list.saveF(qui);
				}
				
				
				cout << "Merge sort: " << mtime << " ms"<< endl;
				cout << "Quick sort: " << qtime << " ms" << endl;
				
				break;

			case 3:
				cout << "Input 501, 502, ...[0]Quit): ";
				cin >> name;
				if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( input, name );
					strcat( input, ".txt" );
				} // if
				else 	strcpy( input, name );						// if input full file name
				
				if(list.readF(input)) {
					cT = clock();
					list.rsort();
					cT = clock() - cT;
					rtime = (float)cT*1000/CLOCKS_PER_SEC;
					strcat( rad, name );
					strcat( rad, ".txt" );
					list.saveF(rad);
				}
				cout << "Radix sort: " << rtime  << " ms" << endl;
				break;

			case 4:
				cout << "Input 501, 502, ...[0]Quit): ";
				cin >> name;
				if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( input, name );
					strcat( input, ".txt" );
				} // if
				else 	strcpy( input, name );						// if input full file name
				
				
				if(list.readF(input)){
					cT = clock();
					list.ssort();
					cT = clock() - cT;
					stime = (float)cT*1000/CLOCKS_PER_SEC;
				} // if
				
				else    break;
				
				if(list.readF(input)){
					cT = clock();
					list.bsort();
					cT = clock() - cT;
					btime = (float)cT*1000/CLOCKS_PER_SEC;
				} // if
				
				if(list.readF(input)){
					cT = clock();
					list.msort( 0, list.size()-1 );
					cT = clock() - cT;
					mtime = (float)cT*1000/CLOCKS_PER_SEC;
				} // if
				
				if(list.readF(input)){
					cT = clock();
					list.qsort( 0, list.size()-1 );
					cT = clock() - cT;
					qtime = (float)cT*1000/CLOCKS_PER_SEC;
				} // if
				
				if(list.readF(input)) {
					cT = clock();
					list.rsort();
					cT = clock() - cT;
					rtime = (float)cT*1000/CLOCKS_PER_SEC;
				} // if

				cout << endl << "Selection sort: " << stime << " ms"<< endl;
				cout << endl << "Bubble sort: " << btime << " ms" << endl;
				cout << endl << "Merge sort: " << mtime << " ms"<< endl;
				cout << endl << "Quick sort: " << qtime << " ms" << endl;
				cout << endl<< "Radix sort: " << rtime  << " ms" << endl;
				
				list.saveTime(name, stime, btime, mtime, qtime, rtime);
				
				break;

			default: cout << endl << "Command does not exist!" << endl;

		} // end switch

	} while (command != 0); // '0': stop the program

	return 0;

} // end of main

