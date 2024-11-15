/*
用queue做排班

data file如下：

OID	Arrival	Duration	TimeOut
103	6	7	15
104	11	9	22
112	5	6	13
101	3	9	12
106	6	9	17
108	6	8	18
105	10	6	20

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


typedef struct jT{
	int OID; // order identifier
	int arrival; // arrival time
	int duration; // job duration
	int timeout; // expire time
} jobType;


int getNO(int, int); // get a number from user


class JobList {
	
	vector<jobType> aList; // list of jobs with four columns
	int total;
	const char* fileID; // file identifier

	void reset(){ 	// definition: initial set up
		aList.clear();
		total = 0;
	} //end reset

	void putAll(const char*); // declaration: write all as a file


	public:
		

		JobList() { reset(); } // constructor for initialization
		~JobList() { reset(); } // destructor for initialization
		
		bool isEmpty() {
			if(aList.empty())   return true;
			
			return false;

		} // check whether it is empty or not
		
		int getTotal() { return total; }

		const char* &getFileID() { return fileID; } // get the file identifier


		bool readFile(const char*); // declaration: read all from a file

		void showAll(); // declaration: output all on screen

		bool getSorted(const char*); // declaration: read all from a file and sort them

		jobType nextJob(); // declaration: get & remove the next job

		bool readFile();

		void inputID(const char* name) { fileID = name; }
}; //end JobList

void JobList::putAll( const char* name ) {
	ofstream f1;
	f1.open(name);

	if( !f1.is_open() ) {  										// Failed to create file
		cout << "Create File Error" << endl;
		return;
	} //
	f1 << "OID	Arrival	Duration	TimeOut" << endl;
	for( int i = 0; i < aList.size(); i++ ) { 					// save the vector into new file
		f1 << aList[i].OID << "\t" << aList[i].arrival
			<< "\t" << aList[i].duration << "\t" << aList[i].timeout << endl;
	} // for

	f1.close();
} // putAll

void JobList::showAll() {
	cout << "\tOID	Arrival	Duration	TimeOut" << endl;
	for( int i = 0; i < aList.size(); i++ ) {
		cout << "(" << i+1 << ")\t" << aList[i].OID << "\t" << aList[i].arrival
			<< "\t" << aList[i].duration << "\t" << aList[i].timeout << endl;
	} // for
} // showAll

bool JobList::readFile(const char* name) { 	// read the file into the vector
	aList.clear();
	jobType temp;
	ifstream f1(name);
	string tmp;
	if( !f1.is_open() ) { 										// if the file name doesn't exist
		cout << endl << "### " << name << " does not exist! ###" << endl;
		return false;
	} // if

	getline( f1, tmp );
	for( int i = 0; f1 >> temp.OID >> temp.arrival >> temp.duration >> temp.timeout; i++ ) {
	    aList.push_back(temp);
	    total++;
	} // for

	f1.close();
	return true;
} // readfile()

jobType JobList::nextJob(){

	jobType nextjob = aList[0];
	aList.erase(aList.begin());

	return nextjob;
}


bool JobList::getSorted( const char* name) {
	char input[300] = {'i','n','p','u','t'};
	char sort[300] = {'s','o','r','t'};
	if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
		strcat( input, name );
		strcat( input, ".txt" );
	} // if
	else 	strcpy( input, name );						// if input full file name
	cT = clock();
	if ( !readFile(input) ) return false;
	cT = clock() - cT;
	float readT = (float)cT*1000/CLOCKS_PER_SEC;
	showAll();
	cout << endl;
	int gap = aList.size() / 2;
	jobType temp;
	cT = clock();
	while( gap > 0 ) {
		for( int i = 0; i < aList.size() - gap; i++ ) {
			if( aList[i].arrival > aList[i+gap].arrival ) {
				temp.OID = aList[i].OID;
				temp.arrival = aList[i].arrival;
				temp.duration = aList[i].duration;
				temp.timeout = aList[i].timeout;

				aList[i].OID = aList[i+gap].OID;
				aList[i].arrival = aList[i+gap].arrival;
				aList[i].duration = aList[i+gap].duration;
				aList[i].timeout = aList[i+gap].timeout;

				aList[i+gap].OID = temp.OID;
				aList[i+gap].arrival = temp.arrival;
				aList[i+gap].duration = temp.duration;
				aList[i+gap].timeout = temp.timeout;
				i = 0;
			} // if
			else if ( aList[i].arrival == aList[i+gap].arrival ) {
				if ( aList[i].OID > aList[i+gap].OID ) {
					temp.OID = aList[i].OID;
					temp.arrival = aList[i].arrival;
					temp.duration = aList[i].duration;
					temp.timeout = aList[i].timeout;

					aList[i].OID = aList[i+gap].OID;
					aList[i].arrival = aList[i+gap].arrival;
					aList[i].duration = aList[i+gap].duration;
					aList[i].timeout = aList[i+gap].timeout;

					aList[i+gap].OID = temp.OID;
					aList[i+gap].arrival = temp.arrival;
					aList[i+gap].duration = temp.duration;
					aList[i+gap].timeout = temp.timeout;
					i = 0;
				} // if
			} // else if
		} // for
		gap = gap / 2;
	} // while
	cT = clock() - cT;
	float sortT = (float)cT*1000/CLOCKS_PER_SEC;
	strcat( sort, name );
	strcat( sort, ".txt" );
	
	cT = clock();
	putAll(sort);
	cT = clock() - cT;
	float writeT = (float)cT*1000/CLOCKS_PER_SEC;
	cout << "Reading data: " << readT << " clocks " << endl << endl;
	cout << "Sorting data: " << sortT << " clocks " << endl << endl;
	cout << "Writing data: " << writeT << " clocks " << endl << endl;
	return true;
} // getSorted()


class JobQueue{
	
	vector<jT> que;

	
	public:
		int avail = 0;
		int CID;
		
		int size(){  	// get the current queue length
			return que.size();
		}//size

		bool isEmpty(){
			if (que.empty())  return true;

			else    return false;
		} // check whether it is empty
		
		bool isFull(){
			if (que.size()>2){

		  		return true;     //queue�W�L3��
			}
			else    return false;
		} // check whether it is empty


		jT front(){    //�^��head����
			return que[0];
		}//front

		jT back(){     //�^��tail����
			return que[size()-1];
		}//back
		
		jT point(int index){     //�^�ǫ��w����
			return que[index];
		}//back
		
		void push(jT newdata){        //��ȷs�W��̫�
			que.push_back(newdata);
		}//push
		
		void pop(){     //��Ĥ@�ӭȥ�X�h
			que.erase(que.begin());
		}//pop
		
		
		
}; // JobQueue


class AnsList {
	typedef struct aT{	 //�����M��
		int OID;
		int CID = -1;
		int delay = 0;  //���~�ɶ�
		int abort;      //�����ɶ� = �ӭq��U��ɨ�
	} abortType;

	typedef struct toT{ 	//�O�ɲM��
		int OID;
		int CID = -1;
		int delay = 0;  //���~�ɶ�
		int departure;      //�����ɶ� = �ӭq��U��ɨ� + ���~�ɶ� + �s�@�Ӯ�
	} timeoutType;

	vector<abortType> abortJobs;			 // list of aborted jobs with three columns
	vector<timeoutType> timeoutJobs; 		 // list of aborted jobs with three columns
	int failedDelay; 						 // total delay of failed jobs
	float failedRate;						 // percentage of failed jobs

	public:
		AnsList(): failedDelay(0), failedRate(0.0)
		{ abortJobs.clear();
		  timeoutJobs.clear(); } // constructor for initialization
		~AnsList()
		{ abortJobs.clear();
		  timeoutJobs.clear(); } // destructor for initialization
		  
		void showAll(); // declaration: display all on screen
		void showAll2(); // declaration: display all on screen
		void addAbortJob(int, int, int, int, int); // declaration: add one aborted job
		void addTimeoutJob(int, int, int, int, int); // declaration: add one timeout job
		void putAll(char*); // declaration: write all as a file
		void putAll2(char*); // declaration: write all as a file
		
		void countRate(int total) { 
			failedRate /= (float)total;
			failedRate *= 100;
		} // countRate
		
}; //end AnsList

void AnsList::showAll() {
	
	cout << "\t[Abort List]" << endl;
	cout << "\tOID     Delay   Abort" << endl;
	
	for( int i = 0; i < abortJobs.size(); i++ ) {
		cout << "[" << i+1 << "]\t" << abortJobs[i].OID << "\t" << abortJobs[i].delay
			<< "\t" << abortJobs[i].abort << endl;
	} // for
	
	cout << "\t[Timeout List]" << endl;
	cout << "\tOID     Delay   Departure" << endl;
	
	for( int i = 0; i < timeoutJobs.size(); i++ ) {
		cout << "[" << i+1 << "]\t" << timeoutJobs[i].OID << "\t" << timeoutJobs[i].delay
			<< "\t" << timeoutJobs[i].departure << endl;
	} // for
	
	cout << "[Total Delay]" << endl << failedDelay << " min" << endl;
	cout << "[Failure Percentage]" << endl << round(failedRate*100)/100 << " %" << endl;
	
} // showAll

void AnsList::showAll2() {
	cout << "\t[Abort List]" << endl;
	cout << "\tOID	CID	Delay	Abort" << endl;
	
	for( int i = 0; i < abortJobs.size(); i++ ) {
		cout << "[" << i+1 << "]\t" << abortJobs[i].OID << "\t" << abortJobs[i].CID
			<< "\t" << abortJobs[i].delay << "\t" << abortJobs[i].abort << endl;
	} // for
	
	cout << "\t[Timeout List]" << endl;
	cout << "\tOID	CID	Delay	Departure" << endl;
	
	for( int i = 0; i < timeoutJobs.size(); i++ ) {
		cout << "[" << i+1 << "]\t" << timeoutJobs[i].OID << "\t" << timeoutJobs[i].CID
			<< "\t" << timeoutJobs[i].delay << "\t" << timeoutJobs[i].departure << endl;
	} // for
	
	cout << "[Total Delay]" << endl << failedDelay << " min" << endl;
	cout << "[Failure Percentage]" << endl << round(failedRate*100)/100 << " %" << endl;
	
} // showAll2

void AnsList::addAbortJob(int ID, int CID ,int arri, int avail, int to) {
	aT temp;
	temp.OID = ID;
	temp.CID = CID;
	
	if(to == 1) {
		temp.abort = arri;
		temp.delay = 0;
	}//if
	
	else if(to == 2) {
		temp.abort = avail;
		temp.delay = avail - arri;
		failedDelay += temp.delay;
	}//esle if
	
	abortJobs.push_back(temp);
	failedRate++;
	
} // addab 

void AnsList::addTimeoutJob(int ID, int CID, int arri, int dura, int avail) {
	toT temp;
	temp.OID = ID;
	temp.CID = CID;
	temp.delay = avail - dura - arri;	
	temp.departure = avail;
	failedDelay += temp.delay;
	
	timeoutJobs.push_back(temp);
	failedRate++;
} // addto 

void AnsList::putAll( char* name ) {
	ofstream f1;
	f1.open(name);

	if( !f1.is_open() ) {  										// Failed to create file
		cout << "Create File Error" << endl;
		return;
	} //
	
	f1 << "\t[Abort List]" << endl;
	f1 << "\tOID	Delay	Abort" << endl;
	
	for( int i = 0; i < abortJobs.size(); i++ ) {
		f1 << "[" << i+1 << "]\t" << abortJobs[i].OID << "\t" << abortJobs[i].delay
			<< "\t" << abortJobs[i].abort << endl;
	} // for
	
	f1 << "\t[Timeout List]" << endl;
	f1 << "\tOID     Delay   Departure" << endl;
	
	for( int i = 0; i < timeoutJobs.size(); i++ ) {
		f1 << "[" << i+1 << "]\t" << timeoutJobs[i].OID << "\t" << timeoutJobs[i].delay
			<< "\t" << timeoutJobs[i].departure << endl;
	} // for
	
	f1 << "[Total Delay]" << endl << failedDelay << " min" << endl;
	f1 << "[Failure Percentage]" << endl << round(failedRate*100)/100 << " %" << endl;
	strcpy(name, "");
	f1.close();
	
} // putAll


void AnsList::putAll2( char* name ) {
	ofstream f1;
	f1.open(name);

	if( !f1.is_open() ) {  										// Failed to create file
		cout << "Create File Error" << endl;
		return;
	} //
	
	f1 << "\t[Abort List]" << endl;
	f1 << "\tOID	CID	Delay	Abort" << endl;
	for( int i = 0; i < abortJobs.size(); i++ ) {
		f1 << "[" << i+1 << "]\t" << abortJobs[i].OID << "\t" << abortJobs[i].CID
			<< "\t" << abortJobs[i].delay << "\t" << abortJobs[i].abort << endl;
	} // for
	
	f1 << "\t[Timeout List]" << endl;
	f1 << "\tOID	CID	Delay	Departure" << endl;
	for( int i = 0; i < timeoutJobs.size(); i++ ) {
		f1 << "[" << i+1 << "]\t" << timeoutJobs[i].OID << "\t" << timeoutJobs[i].CID
			<< "\t" << timeoutJobs[i].delay << "\t" << timeoutJobs[i].departure << endl;
	} // for
	
	f1 << "[Total Delay]" << endl << failedDelay << " min" << endl;
	f1 << "[Failure Percentage]" << endl << round(failedRate*100)/100 << " %" << endl;
	strcpy(name, "");
	f1.close();
	
} // putAll


class Simulation {
	JobList jobs; 				// a list of jobs
	AnsList answers; 			// a set of answers
	vector<JobQueue> allQ;
	JobQueue tempQ;
	int qNum;
	int time;


	void delQ(); // declaration: delete an old job from a queue
	void delQ2();
	void updateQ(JobQueue&, jobType); // declaration: update each queue
	void TwoupdateQ();
	void check1(jobType);
	void check2(jobType);
	void finishQ(JobQueue);

	public:
		Simulation(JobList aList, int N): jobs(aList), qNum(N){
			for ( int i = 0; i < qNum; i++ ) {
				jobType job = jobs.nextJob();
				tempQ.CID = i + 1;
	   			tempQ.push(job);
				tempQ.avail = job.arrival + job.duration;
				tempQ.pop();
				allQ.push_back(tempQ);
			} // for
		} //copy constructor
		
		void SQF(int); // declaration: shortest queue first
}; //end Simulation


void Simulation::updateQ( JobQueue &Q, jobType J ){
	jobType job = J;
	if ( Q.avail <= job.arrival ) Q.avail = job.arrival; 
	
	if ( Q.isEmpty() || job.OID != Q.front().OID   ) {
		Q.push(job);
	} // if
	
	if(job.timeout <= Q.avail) {
		answers.addAbortJob(job.OID, Q.CID, job.arrival, Q.avail, 2);
	} // if
	
	else {
		Q.avail = job.duration + Q.avail;
		if(job.timeout < Q.avail) {
			answers.addTimeoutJob(job.OID, Q.CID, job.arrival, job.duration, Q.avail);							
		} // if
	}//else	
	
	Q.pop();
	
}//update

void Simulation::SQF(int command){
	jobType job, tmpj;
	char one[300] = {'o','n','e'}, two[300] = {'t','w','o'}, more[300] = {'m','o','r','e'};
	jobType t;
	JobQueue nowQ;
	JobQueue tempQ, minQ;
	int qtotal = 0;
	int aCID[20];
	vector<JobQueue> avaQ;
	
	while(!jobs.isEmpty()){
		int min = -1;
		job = jobs.nextJob();
		

		for(int i = 0; i < qNum; i++){     //���X�Ӽp�v���m

			while ( allQ[i].avail <= job.arrival && !allQ[i].isEmpty() ){    //�p�v���m
				updateQ(allQ[i], allQ[i].front());
			} // while
			
			if (allQ[i].avail <= job.arrival && allQ[i].isEmpty() ) {
				min = i;
				i = qNum;
			} // if
			
		}//for
	
		if( min == -1 ) {  //�C�ӳ����O���m
			min = 0;
			for(int i = 1; i < qNum; i++){
				if(allQ[min].size() > allQ[i].size()){
					min = i;
				}//if
			}//for
			
			
		}//if
		
		if(allQ[min].isFull()){
			answers.addAbortJob(job.OID, 0, job.arrival, job.duration, 1);
		} // if
		
		else
			allQ[min].push(job);
		


	}//while
	
	for(int i = 0; i < qNum; i++){
	 
		while ( !allQ[i].isEmpty() ){    //�p�v���m
			updateQ(allQ[i], allQ[i].front());
		} // while
		
	}//for

	

	
	if( command == 1) {
		strcat(one,jobs.getFileID());
		strcat(one,".txt");
		answers.countRate(jobs.getTotal());
		answers.putAll(one);
		answers.showAll();
	}

	else if(command == 2){
		strcat(two,jobs.getFileID());
		strcat(two,".txt");
		answers.countRate(jobs.getTotal());
		answers.putAll2(two);
		answers.showAll2();
	}
	
	else {
		strcat(more,jobs.getFileID());
		strcat(more,".txt");
		answers.countRate(jobs.getTotal());
		answers.putAll2(more);
		answers.showAll2();
		
	}
	
	
}//sqf

int main(void){
	
	int command = 0, qTotal; // user command
	char name[300];
	char sort[300];
	int index = 0;


	
	do{
		JobList orders; // a list of orders
		
		cout << endl << "**** Simulate FIFO Queues by SQF *****";
		cout << endl << "* 0. Quit *";
		cout << endl << "* 1. Sort a file *";
		cout << endl << "* 2. Simulate one FIFO queue *";
		cout << endl << "* 3. Simulate two queues by SQF *";
		cout << endl << "* 4. Simulate multiple queues by SQF *";
		cout << endl << "*******************************";
		cout << endl << "Input a command(0, 1, 2, 3, 4): ";
		cin >> command; // get the command
		
		switch (command){
			
			case 0: break;

			case 1:

			    cout << endl << "Input a file number (e.g., 401, 402, 403, ...): "; 	// input file name
			    cin >> name;
			    orders.inputID(name);
				if (orders.getSorted(name)){ // call: read all from a file and sort them
				 	orders.showAll(); // call: output all on screen
				} //end if

				break;

			case 2:
				strcpy(sort, "sort");
				cout << endl << "Input a file number (e.g., 401, 402, 403, ...): "; 	// input file name
			    cin >> name;
			    orders.inputID(name);
			   	if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( sort, name );
					strcat( sort, ".txt" );
				} // if
				else 	strcpy( sort, name );						// if input full file name
			    
				if (orders.readFile(sort)){	   	 // call: read all from a sorted file
					Simulation sim(orders,1);	 	 // create a simulation of one queue
					orders.showAll();				 // call: output all on screen
					cout << endl;
					sim.SQF(1); 						 // call: simulate FIFO queues by SQF
				} //end if
				
				break;

			case 3: 
				strcpy(sort, "sort");
				cout << endl << "Input a file number (e.g., 401, 402, 403, ...): "; 	// input file name
			    cin >> name;
			    orders.inputID(name);
			   	if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( sort, name );
					strcat( sort, ".txt" );
				} // if
				else 	strcpy( sort, name );						// if input full file name
			    
				if (orders.readFile(sort)){	   	 // call: read all from a sorted file
					Simulation sim(orders,2);	 	 // create a simulation of one queue
					orders.showAll();				 // call: output all on screen
					cout << endl;
					sim.SQF(2); 						 // call: simulate FIFO queues by SQF
				} //end if
				
				break;
				
			case 4:
				strcpy(sort, "sort");
				cout << endl << "Input a file number (e.g., 401, 402, 403, ...): "; 	// input file name
				cin >> name;
				cout << endl << "Input the number of queues: ";
			    cin >> qTotal;
			    
			    orders.inputID(name);
			   	if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( sort, name );
					strcat( sort, ".txt" );
				} // if
				else 	strcpy( sort, name );						// if input full file name

				if (orders.readFile(sort)){	   	 // call: read all from a sorted file
					Simulation sim(orders,qTotal);	 	 // create a simulation of one queue
					orders.showAll();				 // call: output all on screen
					cout << endl;
					sim.SQF(3); 						 // call: simulate FIFO queues by SQF
				} //end if

				break;
				
			default: cout << endl << "Command does not exist!" << endl;
			
		} // end switch
		
	} while (command != 0); // '0': stop the program
	
	return 0;
	
} // end of main
