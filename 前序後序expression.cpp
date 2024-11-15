// 11027217胡佑禎 11027243陳冠妤 
#include <string>			// string class
#include <cstdlib>			// system, atoi
#include <iostream>			// cin, cout
#include <vector>			// vector
using namespace std;

class Istack { 									// the integer stack
	public:
		vector<int> sta;

    	bool empty() { 							// check if the stack is empty 
    		if ( sta.empty() )    return true;
    		else 	return false;
    	} // empty
		
		void push( int str ) { 					// push the stack
			sta.push_back(str);
		} // push

    	void pop() { 							// pop the stack
    		sta.pop_back();
		} // pop

    	int top() { 							// get the top of stack
			int pos = sta.size() - 1;
			return sta[pos];
		} // top

}; // Istack 

class Cstack { 									// the char stack
	public:
		vector<char> sta;

    	bool empty() { 							// check if stack is empty
    		if( sta.empty() )    return true;
    		else 	return false;
    	} // empty

		void push( char str ) { 				// push the stack
			sta.push_back(str);
		} // push

    	void pop() { 							// pop the stack
    		sta.pop_back();
		} // pop

    	char top() { 							// get the top of stack
			int pos = sta.size() - 1;
			return sta[pos];
		} // top
}; // Cstack

class Sstack { 									// the char stack
	public:
		vector<string> sta;

    	bool empty() { 							// check if stack is empty
    		if( sta.empty() )    return true;
    		else 	return false;
    	} // empty

		void push( string str ) { 				// push the stack
			sta.push_back(str);
		} // push

    	void pop() { 							// pop the stack
    		sta.pop_back();
		} // pop

    	string top() { 							// get the top of stack
			int pos = sta.size() - 1;
			return sta[pos];
		} // top

}; // sstack

struct Post { 									// pointer to store postfix
    string num;
    Post *next;
}; // post

typedef Post * PostPtr;


bool isNumber( const char ch ) {				// check whether the character is a number
 
	if( ch <= '9' && ch >= '0' ) return true;
	return false;
 
} // isNumber

bool isOp( const char ch ) {					// check whether the character is an operator

	if( ch == '+' || ch == '-' || ch == '*' || ch == '/' )    return true;
	return false;
	
} // isOp

bool isPar( const char ch ) {					// check whether the character is a parenthesis

	if ( ch == '(' || ch == ')' )  return true;
	return false;
 
} // isPar

bool isBalanced( string input ) {				// check the validity of balanced parentheses

	Cstack sta;

    for ( int i = 0; input[i] != '\0'; i++ ) {
    	if ( input[i] == '(' )     sta.push(input[i]);
    	else if ( input[i] == ')' ) {
    		if ( sta.empty() )   return false;
    		else	sta.pop();
		} // else if
	} // for

	if( !sta.empty() )    return false;
	return true;
	
} // isBalanced

void DeletePtr( PostPtr &aim ) { 					// delete the whole list
	PostPtr temp = aim;
	while( aim != NULL ) {
		aim = aim->next;
		delete temp;
		temp = aim;
	} // while
	temp = NULL;
} // DeletePtr

void Print( PostPtr aim ) { 						// print the whole list
	
	PostPtr temp = aim;
	while( temp != NULL) {
		cout << temp->num;
		temp = temp->next;
		
		if( temp != NULL ) cout << ", ";
	} // while
	cout << endl;
} // Print

bool isInfix( string input, int &type ) { 			// check whether it is an infix expression

	if(isOp(input[0])){
		type = 1;
		return false;
	}//if
	
	for( int i = 0; input[i] != '\0'; i++ ) {		// type=1:operator error，type=2:operand，type=3:()
		if ( input[i] == '(' ) {
			i++;
			if ( input[i] == ')' ) {
				type = 3;
				return false; 		// when the case is ()
			} // if
			if ( !( isNumber(input[i]) || input[i] == ' ' || input[i] =='(' ) && input[i] != '\0' ) {
				type = 1;
				return false; 		// operator error
			} // if
			else 	i--;
		} // if
		
		else if ( isNumber(input[i]) ) {
			i++;
			if( !( isNumber(input[i]) || isOp(input[i]) || input[i] == ' ' 
				|| input[i] == ')' ) && input[i] != '\0' ) {
				type = 2;
				return false; 		// operand
			} // if
			else 	i--;
		} // else if
		
		else if ( isOp(input[i]) ) {
			i++;
			if( !( isNumber(input[i]) || input[i] == ' ' || input[i] == '(') ) {
				type = 1; 		
				return false;		// operator error
			} // if
			else 	i--;
		} // else if
		
		else if ( input[i] == ')' ) {
			i++;
			if( !( isOp(input[i]) || input[i] == ' ' || input[i] == ')' ) && input[i] != '\0' ) {
				type = 2;
				return false; 		// operand
			} // if
			else 	i--;
		} // else if
	} // for
	
	return true;
} // isInfix

void toPostfix( string &ori, PostPtr &postfix ) { 		// transform infix into postfix
	bool reset = true;									// true:previous one is operator
	int Par = 0; 										// false:still inputing numbers (ex:22)
	PostPtr now = postfix; 							
	Cstack ope;
	for( int i = 0; ori[i] != '\0'; i++ ) { 			// traverse each char
		if ( isNumber(ori[i]) ) {						// if the char is number
			if ( postfix == NULL ) {					// if it is first char
				postfix = new Post;
				postfix->num = ori[i];
				postfix->next = NULL;
				now = postfix;
				reset = false;
			} // if
			else if ( reset ) { 						// if the previous one is operator
				now->next = new Post;
				now->next->num = ori[i];
				now->next->next = NULL;
				now = now->next;
				reset = false;
			} // else if
			else now->num += ori[i]; 					// if the previous one is number
		} // if
		else if ( isOp(ori[i]) || isPar(ori[i]) ) { 	// if the char is operator or parenthesis
			reset = true;
			if ( ori[i] == ')' ) { 						// if encounter )
				while( ope.top() != '(' ) {
					now->next = new Post;
					now->next->num = ope.top();
					now->next->next = NULL;
					now = now->next;
					ope.pop();			
				} // while
				ope.pop();
			} // if
			
			else if ( ope.empty() || ori[i] == '(' || ope.top() == '(' ) // encounter ( or the stack is empty
				ope.push(ori[i]); 
				
			else if ( ( ori[i] == '*' || ori[i] == '/' )
				&& ( ope.top() == '+' || ope.top() == '-' ) ) { 		 // */ first to be calculated 
				ope.push(ori[i]);
			} // else if
			
			else if ( ( ori[i] == '+' || ori[i] == '-' )
				&& ( ope.top() == '*' || ope.top() == '/' ) ) { 		// encounter +- and there is */ in stack
				while( !ope.empty() ) {									// store the whole stack into list
					if ( !isPar(ope.top()) ) { 							// whithout parenthesis
						now->next = new Post;
						now->next->num = ope.top();
						now->next->next = NULL;
						now = now->next;
						ope.pop();			
					} // if
					else ope.pop(); 									// pop the (
				} // while
				ope.push(ori[i]);
			} // else if	
			else { 														// other possibilities
				now->next = new Post;
				now->next->num = ope.top();
				now->next->next = NULL;
				now = now->next;
				ope.pop();
				ope.push(ori[i]);
			} // else
		} // else if
	} // for
	
	while( !ope.empty() ) { 				// store all the operators left in the stack
		if ( !isPar(ope.top()) ) {
			now->next = new Post;
			now->next->num = ope.top();
			now->next->next = NULL;
			now = now->next;
			ope.pop();			
		} // if
		else ope.pop();
	} // while
	
	reset = true;
} // toPostfix()

void postfixEval( string input ) {    					// evaluate the value of postfix
	PostPtr postfix = NULL, temp = NULL, temp2 = NULL;
 	toPostfix( input, postfix );						// transform infix into postfix
 	temp2 = postfix;
 	cout << "Postfix expression: ";
 	Print( temp2 );										// print the postfix
	int index = 0;
	temp = postfix;
	int num = 0, total = 0;
	Istack data;
	
	while( temp != NULL ) {
		if ( isNumber( ( temp->num)[0] ) ) { 			// change the numbers from string to integer
			num = atoi( temp->num.c_str() );
			data.push(num);								// store the integers into stack
		} // if

		if ( isOp ( temp->num[0] ) ) { 					// if encounter the operator
			num = data.top();
			data.pop();
			total = data.top();
			data.pop(); 								// obtain two toppest numbers in stack
														// calculating
			if ( temp->num == "+" )   total = total + num;
			
			else if ( temp->num == "-" )  total = total - num;
			
			else if ( temp->num == "*" )  total = total * num;
			
			else if ( temp->num == "/" ) {
				if ( num != 0 )	total = total / num; 	
				else {									// divide 0
					index = 1;
					break;
				} // else
			} // else if
			 
			data.push(total); 							// record result
		} // if
		temp = temp->next ;
	} // while

	if( !data.empty() )   total = data.top(); 			// ans
	
	if( index == 0 )	cout << "Answer = " << total << endl;

	else    cout << "### Error: Divided by ZERO! ###" 
		<< endl << "### It cannot be successfully evaluated! ###" << endl;
	
} // postfixEval()


void toPrefix( string &ori, PostPtr &prefix ) { 		// transform infix into postfix
	bool reset = true;									// true:previous one is operator
	int Par = 0; 										// false:still inputing numbers (ex:22)
	PostPtr now = prefix;
	Cstack ope, change, buffer;
	
	for(int i = 0; ori[i] != '\0'; i++){      //把字串倒過來存在stack
	
		if(isNumber(ori[i])){
			while(isNumber(ori[i]) && ori[i] != '\0'){
				buffer.push(ori[i]);
				i++;
			}//while
			
			i--;
			
			while(!buffer.empty()){
				change.push(buffer.top());
				buffer.pop();
			}//while
			
		}//if
		
		else			change.push(ori[i]);
		
	}//for
	
	ori.clear();//清空原字串
	for(int i = 0; !change.empty(); i++){
		ori[i] = change.top();
		change.pop();
	}//while()

	
	for( int i = 0; ori[i] != '\0'; i++ ) { 			// traverse each char
		if ( isNumber(ori[i]) ) {						// if the char is number
			if ( prefix == NULL ) {						// if it is first char
				prefix = new Post;
				prefix->num = ori[i];
				prefix->next = NULL;
				now = prefix;
				reset = false;
			} // if
			else if ( reset ) { 						// if the previous one is operator
				prefix = NULL;
				prefix = new Post;
				prefix->num = ori[i];
				prefix->next = now;
				now = prefix;
				reset = false;
			} // else if
			else now->num += ori[i]; 					// if the previous one is number
		} // if
		else if ( isOp(ori[i]) || isPar(ori[i]) ) { 	// if the char is operator or parenthesis
			reset = true;
			if ( ori[i] == '(' ) { 						// if encounter )
				while( ope.top() != ')' ) {
					prefix = NULL;
					prefix = new Post;
					prefix->num = ope.top();
					prefix->next = now;
					now = prefix;
					ope.pop();
				} // while
				ope.pop();
			} // if

			else if ( ope.empty() || ori[i] == ')' || ope.top() == ')' ) // encounter ( or the stack is empty
				ope.push(ori[i]);

			else if ( ( ori[i] == '*' || ori[i] == '/' )
				&& ( ope.top() == '+' || ope.top() == '-' ) ) { 		 // */ first to be calculated
				ope.push(ori[i]);
			} // else if

			else if ( ( ori[i] == '+' || ori[i] == '-' )
				&& ( ope.top() == '*' || ope.top() == '/' ) ) { 		// encounter +- and there is */ in stack
				// while( !ope.empty() ) {									// store the whole stack into list
					if ( !isPar(ope.top()) ) { 							// whithout parenthesis
						prefix = NULL;
						prefix = new Post;
						prefix->num = ope.top();
						prefix->next = now;
						now = prefix;
						ope.pop();
					} // if
					else ope.pop(); 									// pop the (
				// } // while
				ope.push(ori[i]);
			} // else if
			else { 														// other possibilities
				ope.push(ori[i]);
			} // else
		} // else if

	} // for

	while( !ope.empty() ) { 				// store all the operators left in the stack
		if ( !isPar(ope.top()) ) {
			prefix = NULL;
			prefix = new Post;
			prefix->num = ope.top();
			prefix->next = now;
			now = prefix;
			ope.pop();
		} // if
		else ope.pop();
	} // while

	reset = true;
} // toprefix()

void prefixEval( string input ) {    					// evaluate the value of postfix
	PostPtr prefix = NULL, temp = NULL, temp1 = NULL, temp2 = NULL, temp3 = NULL;

 	toPrefix( input, prefix );						// transform infix into postfix

 	temp2 = prefix;
	cout << "Prefix expression:";
 	Print( temp2 );									// print the postfix

	int index = 0;
	temp3 = prefix;
	int num = 0, total = 0;

	Istack data;
	Sstack newlist;
	
	if(temp3 != NULL && temp3->next == NULL){
		total = atoi( temp3->num.c_str() );
		cout << "Answer = " << total << endl;
		return;
	}//if
	
	while(temp3 != NULL){
		newlist.push(temp3->num);
		temp3 = temp3->next;
	}//while()


	temp = new Post;
	temp1 = temp ;
	while(!newlist.empty()){
		temp1->num = newlist.top();
		newlist.pop();

		if(!newlist.empty()) {
			temp1->next = new Post;
			temp1 = temp1->next;
		}//if

	}//while()



	while( temp != NULL ) {
		if ( isNumber( ( temp->num)[0] ) ) { 			// change the numbers from string to integer
			num = atoi( temp->num.c_str() );
			data.push(num);								// store the integers into stack
		} // if

		if ( isOp ( temp->num[0] ) ) { 					// if encounter the operator
			total = data.top();
			data.pop();
			num = data.top();
			data.pop(); 								// obtain two toppest numbers in stack
														// calculating
			if ( temp->num == "+" )   total = total + num;

			else if ( temp->num == "-" )  total = total - num;

			else if ( temp->num == "*" )  total = total * num;

			else if ( temp->num == "/" ) {
				if ( num != 0 )	total = total / num;
				else {									// divide 0
					index = 1;
					break;
				} // else
			} // else if

			data.push(total); 							// record result
		} // if
		
		temp = temp->next ;
		
	} // while

	if( !data.empty() )   total = data.top(); 			// ans

	if( index == 0 )	cout << "Answer = " << total << endl;

	else    cout << "### Error: Divided by ZERO! ###"
		<< endl << "### It cannot be successfully evaluated! ###" << endl;

} // prefixEval()

bool isError( string input ) { 							// check if there is error
	int errtype = 0;

	if ( input[1] == '\0' && isOp(input[0]) ) { 		// one extra operator
		cout << "Error 3: there is one extra operator." << endl;
		return false;
	} // if
	
	for( int i = 0; input[i] != '\0'; i++ ) {			// not a legitimate character
		if ( !isNumber(input[i]) && !isPar(input[i]) && !isOp(input[i]) && input[i] != ' ' ) {
			cout << "Error 1: "<< input[i] << " is not a legitimate character." << endl;
			return false;
		} // if
	} // for

	if( !isBalanced(input) ) {							// extra open parenthesis
		cout << "Error 2: there is one extra open parenthesis." << endl;
		return false;
	} // if

	if( !isInfix(input, errtype) ) {
		if(errtype == 1)    cout << "Error 3: there is one extra operator." << endl;

		if(errtype == 2)    cout << "Error 3: there is one extra operand." << endl;
		
		if(errtype == 3)    cout << "Error 3: it is not infix in the parentheses." << endl;
		return false;
	} // if

	return true;
} // isError


int main(void) {
	int command;
	PostPtr postfix = NULL, prefix = NULL;
	do {
		string infixS, postfixS;
		
		cout << endl << "* Arithmetic Expression Evaluator *";
		cout << endl << "* 0. QUIT *";
		cout << endl << "* 1. Infix2postfix Evaluation *";
		cout << endl << "* 2. Infix2prefix Evaluation  *";
		cout << endl << "***********************************";
		cout << endl << "Input a choice(0, 1, 2): ";
		cin >> command;      // get a command
		
		switch(command) {
			
			case 0:										// '0': stop the program
				
				break;
				
			case 1: 									// '1':Infix to postfix evaluation
				cout << endl << "Input an infix expression: ";
				cin.get();
				getline(cin, infixS);
				
				if( isError(infixS) ) { 				// Input legitimate infix expression
					cout << "It is a legitimate infix expression." << endl;
					postfixEval(infixS);
					DeletePtr(postfix);
				} // if
				
				break;
				

			case 2:
				cout << endl << "Input an infix expression: ";
				cin.get();
				getline(cin, infixS);

				if( isError(infixS) ) { 				// Input legitimate infix expression
					cout << "It is a legitimate infix expression." << endl;
					prefixEval(infixS);
					DeletePtr(prefix);
				} // if

				break;
				
				
			default: cout << endl << "Command does not exist!" << endl;
			
		} // end switch
		
	} while ( command != 0 );	// '0': stop the program
	
	system("pause");  			// pause the display
	return 0;
 
} // end main
