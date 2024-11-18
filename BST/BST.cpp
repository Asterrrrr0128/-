/*
BST操作
*/
#include <iostream>
#include <fstream> 
#include <string.h>
#include <cstdlib> 
#include <queue>
#include <iomanip> 
#include <vector>
#include <stdlib.h>
#include <cstdlib>
using namespace std;
//************************************************************/
// Header file for Binary Search Tree by YH
//************************************************************/
// template <typename T>
class BSTree { 
	typedef struct BST { 
		//T key; // search key
		vector<int> idx; // data index
		struct BST *lchild; // left child
		struct BST *rchild; // right child
	} nodeType; // node structure of BST
	
	nodeType *root; // root node of BST
	
	int visit; // number of visited nodes
	
	
	void TreeBalance( BST *, BST *);
	void insertNode(int); // insert a node by recursion
	int getH(nodeType *); // calculate the tree height by recursion
	void matchLB(int, nodeType *, int&); // get all matches over the lower bound by recursion
	void clearBST( ); // clean up the entire tree by recursion

	public:
		vector<int> list;
		
		BSTree(): root(NULL), visit(0) { } // constructor of an empty tree
		
		bool isEmpty() {
			if( root == NULL ) return 1;
			
			return 0;
		} // examine whether it is empty or not
		void TreeB();
		
		void TreeBalance();
		void store(BST*, vector<int>& );
		void buildbal(BST* &, vector<int> &, int , int );
		
		void add(int); // insert a node into BST on any type
		void showH(); // show the tree height
		int findLB(const char* ); // search by a lower bound
		int delMax(); // delete a record with the maximal key
		void level(vector<int>&);
		void clearUp() {
			delete root;
			root = NULL;
			visit = 0;

		}//clearBST(root); visit = 0; } // cut off the entire tree
		~BSTree() { clearUp(); } // destructor
}; //end BSTree



void BSTree::store(BST* temp, vector<int> &nodes){
    // Base case
    if (temp == NULL)
        return;

    // Store nodes in Inorder (which is sorted
    // order for BST)
    store(temp->lchild, nodes);
    nodes.push_back(temp->idx[0]);
    store(temp->rchild, nodes);
}

void BSTree::buildbal(BST* &temp, vector<int> &nodes, int start, int end){
    // base case
	
    if (start > end)    return;
    
	
    int mid = (start + end)/2;
    
	temp = new BST;
	temp->lchild = NULL;
	temp->rchild = NULL;
    temp->idx.push_back(nodes[mid]);

	//cout << temp->idx[0] <<endl;
	
    buildbal(temp->lchild, nodes, start, mid-1);
    buildbal(temp->rchild , nodes, mid+1, end);
    

}



void BSTree::TreeBalance(){
	
	
	BST * temp = root, * temp2;
    vector<int> nodes;
    store(temp, nodes);
	delete root;
	root = NULL;
	root = new BST;
	root->lchild = NULL;
	root->rchild = NULL;
	
    int n = nodes.size();
    

    buildbal(temp2, nodes, 0, n-1);
    
	root = temp2;
    
}


int BSTree::delMax() {
	if (isEmpty()) return -1;
	BST *temp = root;
	BST *temp2 = temp;
	while ( temp->rchild != NULL ) {
		temp2 = temp;
		temp = temp->rchild;
	} // while
	int del = temp->idx[0];
	if ( temp->idx.size() == 1) {
		if ( temp == root ) {
			root = temp->lchild;
			temp->lchild = NULL;
			delete temp;			
		} // if
		else {
			temp2->rchild = temp->lchild;
			temp->lchild = NULL;
			delete temp;			
		} // else 
		
	} // if
	else temp->idx.pop_back();
	return del;
} // delMax()


int BSTree::getH( nodeType *temp ) {

	
	if( temp->lchild == NULL && temp->rchild == NULL ) return 1;
	
	else if ( temp->lchild == NULL )	 return  getH( temp->rchild )+1;
	
	else if ( temp->rchild == NULL ) 	 return  getH( temp->lchild )+1;
	
	else {
		int left = getH( temp->lchild )+1;
		int right = getH( temp->rchild )+1;
		
		if( left > right ) 	return left;
		
		else	 return	 right;
	} // else
	
} // getH

void BSTree::showH() {

	if (!isEmpty()) {
		BST *temp = root;
		int H = getH(temp);
		cout << "HP tree height = " << H;
	} // if
	else cout << "HP tree height = 0";

} // showH


void BSTree::level(vector<int> &vec){

	nodeType *temp = root;
	queue< pair<nodeType *,int> > que;
	nodeType *temp2;
	int data, le;
	
	que.push(make_pair(temp, 1));

	
	while(!que.empty()){
		
		temp2 = que.front().first;
	    data = temp2->idx[0];
		le = que.front().second;
		vec.push_back(le);
		vec.push_back(data);
		que.pop();
			
	  	
		 
		 
	  	if(temp2->lchild != NULL)
		 	que.push(make_pair(temp2->lchild, le + 1));


		if(temp2->rchild != NULL)
			que.push(make_pair(temp2->rchild, le + 1));
			
	}
	

}//leveel

void BSTree::add( int num ) {
	if (isEmpty()) {
		root = new BST;
		root->lchild = NULL;
		root->rchild = NULL;
		root->idx.push_back(num);
	} // if
	
	else {
		insertNode(num);
	} // else
} // add

void BSTree::insertNode(int num) {
	BST *temp = root;
	BST *temp2 = temp;
	
	while ( temp != NULL) {
		temp2 = temp;
		if( num > temp->idx[0] ) temp = temp->rchild;
		else if( num < temp->idx[0] ) temp = temp->lchild;
		
		else break;
	} // while
	
	if ( num > temp2->idx[0] ) {
		temp2->rchild = new BST;
		temp2->rchild->idx.push_back(num);
		temp2->rchild->rchild = NULL;
		temp2->rchild->lchild = NULL;
	} // if
	
	else if(num < temp2->idx[0] ){
		temp2->lchild = new BST;
		temp2->lchild->idx.push_back(num);
		temp2->lchild->rchild = NULL;
		temp2->lchild->lchild = NULL;
	} // else
	
	else {
		temp2->idx.push_back(num);
		
	}//else
	
} // insertNode

int BSTree::findLB( const char* key) {
	int bound = 0, count = 0;
	nodeType *temp = root, *temp2 = root;
	sscanf(key, "%d", &bound);

	matchLB(bound, temp, count);

	return count;
}

void BSTree::matchLB(int bound, nodeType *temp, int &count){
	int index = 0, i = 0;

	
	if(temp == NULL){
		return;
	}//if
	
	else {
		count ++;
	 	if( temp->idx[0] >= bound){
	 		
			for(i = 0; i < list.size() ; i++)
				if(list[i] == temp->idx[0]) 	break;
		
			if(i == list.size())    list.push_back(temp->idx[0]);
			
			matchLB(bound, temp->rchild, count);
			
			
			if(temp->idx[0] != bound){
				matchLB(bound, temp->lchild, count);
			}		

	 	}//if



		else {

			matchLB(bound, temp->rchild, count);
			
		}


		
		return;
	}//else
	

	
}

class pokemonList { 
	typedef struct pT { 
		string rawR; // raw data of one record
		int no; // pokemon number
		string name; // pokemon name
		string tp1; // 1st type
		int tot; // total sum
		int hp; // health point
		int atk; // attack
		int def; // defense
	} pokemonType;
	
	vector<pokemonType> pSet; // set of data records
	string fileNO; // number to form a file name
	BSTree aBST; // a binary search tree

	void show( ); // display one record on screen
	int loc( ) const; // locate the entry

	public:
		pokemonList(): fileNO("") { } // constructor: do nothing
		void sort(vector<int> &);
		void balance();
		bool isEmpty(); // check if there is nothing
		bool readF(const char*); // read records from a file
		void showAll(); // display all the records on screen
		void buildBST(); // build BST
		void printl();
		void tSearch(const char*); // threshold search
		void maxDel(); // delete the maximum of a column
		void clearUp() // erase the object content
		{ pSet.clear(); fileNO.clear(); aBST.clearUp(); } // end clearUp
		~pokemonList() { clearUp(); } // destructor: destroy the object
}; // class pokemonList

void pokemonList::balance(){
	aBST.TreeBalance();
}

void pokemonList::printl(){
	vector<int> vec;
	int lev = 0, index = 0;
	aBST.level(vec);
	
	for(int i = 0; i < vec.size(); i++){
		
		if(lev != vec[i]){

			cout << endl << "Level " << vec[i] << ": ";
		}	
  
		
		lev = vec[i];
		i++;
		
		for(int n = 0; n < pSet.size(); n++){
			
			if(pSet[n].hp == vec[i]) {
				if(index == 0 ) cout << "(" << vec[i] ;
				
				cout <<  ", " << pSet[n].no ;
				index = 1;
			}//if
			
		}//for
		
		cout << ")" ;
		index = 0;
		
	}//for
		
	
		

}

void pokemonList::maxDel(){
	int aim = aBST.delMax();
	if ( aim != -1 ) {
		cout << "#\tName\tType 1\tType 2\tTotal\tHP\tAttack\tDefense\tSp. Atk\tSp. Def\tSpeed\tGeneration\tLegendary" << endl;
		for(int n = 0; n < pSet.size(); n++){
			if(pSet[n].hp == aim) {
				cout << pSet[n].rawR << endl;
				pSet.erase(pSet.begin()+n);
				n = pSet.size();
			}//if
		}//for		
		aBST.showH();		
	} // if
	else	cout << "----- Execute Mission 1 first! -----" << endl;

} // maxDel()


void pokemonList::buildBST() {
	aBST.clearUp();
	for( int i = 0; i < pSet.size(); i++ ) {
		aBST.add(pSet[i].hp);
	} // for
	

	aBST.showH();
} // build

void pokemonList::showAll() {
	cout << "\t#\tName\tType 1\tHP\tAttack\tDefense" <<endl;
	for( int i = 0; i < pSet.size(); i++ ) {
		cout << "[" << i+1 << "]\t" << pSet[i].no << "\t" << pSet[i].name << "\t" << pSet[i].tp1 
			<< "\t" << pSet[i].hp << "\t" << pSet[i].atk << "\t" << pSet[i].def << endl;
	} // for
} // showAll

bool pokemonList::readF( const char* path ) { 	// read the file into the vector 
	pSet.clear();
	pT temp;
	char ch;
	
	ifstream f1(path);	
	if( !f1.is_open() ) { 										// if the file name doesn't exist
		cout << endl << "Failed to open file." << endl;
		return false;
	} // if

	string tmp;
	getline( f1, tmp );

	for( int i = 0; getline( f1, tmp ); i++ ) { 				// save a line of data into rawR
		temp.rawR = tmp;
		pSet.push_back(temp);
	} // for
	
	f1.close();
	f1.open(path);
	
	getline( f1, tmp );

	for( int i = 0, size = 0; f1 >> temp.no >> temp.name ; i++ ) {
		ch = '\0';
	    pSet[i].no = temp.no;
	    
	    size = temp.name.size();
	    
	    pSet[i].name = temp.name;
	    
	    f1.get(ch);
	    
		if(ch != '\t') {
			f1 >> tmp;
			pSet[i].name = pSet[i].name + tmp;
		}
	    
	    
		f1 >> temp.tp1;
		
	    pSet[i].tp1 = temp.tp1;	
	    f1 >> tmp;
	    
	    
	    if(!(tmp[0] >= '0' && tmp[0] <= '9'))  f1 >> tmp;


	   	pSet[i].tot = atoi(tmp.c_str());

		
		f1 >> temp.hp >> temp.atk >> temp.def >> tmp >> tmp >> tmp >> tmp >> tmp;
	    
	    		
	    pSet[i].hp = temp.hp;
	    pSet[i].atk = temp.atk;
	    pSet[i].def = temp.def;
	    
	} // for
	
	f1.close();

	return true;

} // readfile()


void pokemonList::sort(vector<int> &list) {
	int index ;
	for (int i = list.size() - 1; i > 0; i--) {
		for (int j = 0; j < i; j++)
			if (list[j + 1] > list[j]) {
				index = list[j] ;
				list[j] = list[j+1] ;
				list[j+1] = index ;
			}
	}

}

void pokemonList::tSearch( const char* key ){
	int count = 0;
	
	if(!aBST.isEmpty()){
		count = aBST.findLB(key);
		
		sort(aBST.list);
		
		cout << "\t#\tNAME\tTYPE 1\tTOTAL\tHP\tATTACK\tDEFENSE" <<endl;
		for(int i = 0, j = 1; i < aBST.list.size(); i++){

			for(int n = 0; n < pSet.size(); n++){
				if(pSet[n].hp == aBST.list[i]) {
					cout << "[" << j << "]\t" << pSet[n].no << "\t" << pSet[n].name << "\t" << pSet[n].tp1
						<< "\t" << pSet[n].tot << "\t"<< pSet[n].hp << "\t" << pSet[n].atk << "\t" << pSet[n].def << endl;
						
					j++;
				}//if
			}//for

			
		}//for
		
		cout << "Number of visited nodes = " << count << endl;

	}//if
	
	else    return;

}



int main(void){

	int command = 0; // user command
	pokemonList list;
	bool isEXE = false;
	
	do{
		
		char key[300];
	    char name[300];
	    char input[300] = {'i','n','p','u','t'}, sel[300] = {'s','e','l','e','c','t','_','s','o','r','t'};

		cout << endl << "*** Binary Search Tree on Pokemon ***";
		cout << endl << "* 0. Quit *";
		cout << endl << "* 1. Read one file to build BST     *";
		cout << endl << "* 2. Threshold search on one column *";
		cout << endl << "* 3. Delete the max on one column   *";
		cout << endl << "* 4. Convert into balanced BST      *";
		cout << endl << "*******************************";
		cout << endl << "Input a command(0, 1, 2, 3, 4): ";
		cin >> command; // get the command

		switch (command){

			case 0: break;

			case 1:
				cout << "Input a file number [0: quit]: ";
				cin >> name;
				if ( name[0] >= '0' && name[0] <= '9' ) { 			// if input only numbers
					strcat( input, name );
					strcat( input, ".txt" );
				} // if
				else 	strcpy( input, name );						// if input full file name
				
				if(list.readF(input)) {
					list.showAll();
					list.buildBST();
				} // if
				
				isEXE = true;

				break;

			case 2:
				
				if(isEXE){
					cout << "Threshold (a positive integer): ";
					
					
					do{
						cin >> key;
						
						if(key[0] >= '0' && key[0] <='9')   list.tSearch(key);
						
						else{
							cout << "### It is NOT a positive integer. ###" << endl;
							cout << "Try again: " ;
						}
						
					}while(!(key[0] >= '0' && key[0] <='9'));
	               
					
				}//if
				
				
				else		cout << "----- Execute Mission 1 first! -----" << endl;

			
				break;

			case 3:

				if(isEXE){
					list.maxDel();
				}//if

				else		cout << "----- Execute Mission 1 first! -----" << endl;

				break;

			default: cout << endl << "Command does not exist!" << endl;
			
			case 4:
				
				if(isEXE){
					list.balance();
					list.printl();
				}//if

				else		cout << "----- Execute Mission 1 first! -----" << endl;

				break;
				

		} // end switch

	} while (command != 0); // '0': stop the program

	return 0;
} // end of main

