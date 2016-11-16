#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <iomanip>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <cctype>
#include <cstdlib>
using namespace std;

int flag = 0;//flag = 0 means a single command is executed successfully.
int exit_flag = 0;//This is for command "exit" appeared in a composition, exit_flag = 1 means it's time to exit
class Executor {
public:
	void execute(char *argv[]) {

		if (strcmp(argv[0], "test") == 0 || strcmp(argv[0], "[") == 0) {//test command starts with "test" and "["
			struct stat sb;
			if (*argv[1] == '-') {//test command with argument "-e" "-f" or "-d"
				if (strcmp(argv[1], "-f") == 0) {//test command with "-f"
				
					if (stat(argv[2], &sb) == -1)
					{
//						perror("stat");
					}
					if (S_ISREG(sb.st_mode))//the argument is a file
					{
						cout << "(True)" << endl;
					}
					else {
						cout << "(False)" << endl;
					}
					//start with "[" but the number of arguments is more than 1, give an error information
					// or start with "test" but the number of arguments is more than 1, give an error information
					if ((strcmp(argv[0], "[") == 0 && argv[3] != NULL && strcmp(argv[3], "]") != 0) || (strcmp(argv[0], "test") == 0 && argv[3] != NULL)) {
						cout << "syntax error near unexpected token '" << argv[3] << "'" << endl;
					}
					//start with "[" but lack "]"
					if (strcmp(argv[0], "[") == 0 && argv[3] == NULL) {
						cout << "There should be a ']' after token " << argv[2] << endl;
					}
				}
				else if (strcmp(argv[1], "-d") == 0) {//test command with "-d"
					if (stat(argv[2], &sb) == -1)
					{
	//					perror("stat");
					}
					if (S_ISDIR(sb.st_mode))// the argument is a directory
					{
						cout << "(True)" << endl;
					}
					else {
						cout << "(False)" << endl;
					}	
					//start with "[" but the number of arguments is more than 1, give an error information
					// or start with "test" but the number of arguments is more than 1, give an error information
					if ((strcmp(argv[0], "[") == 0 && argv[3] != NULL && strcmp(argv[3], "]") != 0) || (strcmp(argv[0], "test") == 0 && argv[3] != NULL)) {
						cout << "syntax error near unexpected token '" << argv[3] << "'" << endl;
					}
					//start with "[" but lack "]"
					if (strcmp(argv[0], "[") == 0 && argv[3] == NULL) {
						cout << "There should be a ']' after token " << argv[2] << endl;
					}
				}
				else if (strcmp(argv[1], "-e") == 0) {//test command with "-e"
					if (stat(argv[2], &sb) == -1)
					{
	//					perror("stat");
					}
					if (S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode))//the argument exsists
					{
						cout << "(True)" << endl;
					}
					else {
						cout << "(False)" << endl;
					}
					//start with "[" but the number of arguments is more than 1, give an error information
					// or start with "test" but the number of arguments is more than 1, give an error information
					if ((strcmp(argv[0], "[") == 0 && argv[3] != NULL && strcmp(argv[3], "]") != 0) || (strcmp(argv[0], "test") == 0 && argv[3] != NULL)) {
						cout << "syntax error near unexpected token '" << argv[3] << "'" << endl;
					}
					//start with "[" but lack "]"
					if (strcmp(argv[0], "[") == 0 && argv[3] == NULL) {
						cout << "There should be a ']' after token " << argv[2] << endl;
					}
				}
				else {//the argument with "-" is not one of "-e" "-d" and "-f", give an error information
					cout << "\"" << argv[1] << "\"" << "can not be identified" << endl;
				}
			}
			else {//test command without the argument "-", use default "-e"
				if (stat(argv[1], &sb) == -1)
				{
		//			perror("stat");
				}
				if (S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode))//the argument exsists
				{
					cout << "(True)" << endl;
				}
				else {
					cout << "(False)" << endl;
				}
				//start with "[" but the number of arguments is more than 1, give an error information
				// or start with "test" but the number of arguments is more than 1, give an error information
				if ((strcmp(argv[0], "[") == 0 && argv[2] != NULL && strcmp(argv[2], "]") != 0) || (strcmp(argv[0], "test") == 0 && argv[2] != NULL)) {
					cout << "syntax error near unexpected token '" << argv[2] << "'" << endl;
				}
				//start with "[" but lack "]"
				if (strcmp(argv[0], "[") == 0 && argv[2] == NULL) {
					cout << "There should be a ']' after token " << argv[1] << endl;
				}
			}
		}
		else {
			int child_ret;
			pid_t pid;
			pid = fork();//create a child
			if (pid == 0) {//child
				execvp(argv[0], argv);
				cout << "command not found" << endl;
				exit(-1);
			}
			else if (pid > 0) {//parent
				wait(&child_ret);
				flag = WEXITSTATUS(child_ret);//get the value of what the child returned, if flag!=0,
			}                                 // then the command has not been executed successfully
			else {
				cout << "fork error" << endl;
			}
		}
	}
};



class Base {
public:
	virtual void execute() = 0;
};

class Command :public Base {
public:
	char *command[20];//store several argument of a single command
	Executor* exe;
public:
	Command(char *cmd[]) {
		for (int i = 0; i < 20; i++) {
			command[i] = new char[20];
			command[i] = cmd[i];
		}
	}
	void execute() {
		if (strcmp(command[0], "exit") == 0) {//if current command is "exit", give exit flag value 1
			exit_flag = 1;
		}
		else {
			exe->execute(command);//if current command is not "exit", execute the command
		}
	}
};

class And :public Base {
protected:
	Base* left;
	Base* right;
public:
	And(Base* lef, Base* righ) {
		left = lef;
		right = righ;
	}
	void execute() {
		flag = 0;
		exit_flag = 0;
		left->execute();
		if (exit_flag != 1) {//the left node is not "exit"
			if (flag == 0) {//the left node has been executed successfully
				flag = 0;
				exit_flag = 0;
				right->execute();
			}
		}
	}
};
class Or :public Base {
protected:
	Base* left;
	Base* right;
public:
	Or(Base* lef, Base* righ) {
		left = lef;
		right = righ;
	}
	void execute() {
		flag = 0;
		exit_flag = 0;
		left->execute();
		if (exit_flag != 1) {//the left node is not "exit"
			if (flag != 0) {//the left node has not been executed successfully
				flag = 0;
				exit_flag = 0;
				right->execute();
			}
		}
	}
};

class Parenthesis :public Base {//the composition of "()"
protected:
	vector<Base*> commd;
public:
	void addNode(Base* Node) {
		commd.push_back(Node);
	}
	void removeNode(Base* Node) {
		for (int i = 0; i < (int)commd.size(); i++) {
			if (commd.at(i) == Node) {
				commd.erase(commd.begin() + i);
			}
		}
	}

	void execute() {
		for (int i = (int)commd.size() - 1; i >= 0; i--) {
			flag = 0;
			exit_flag = 0;
			commd.at(i)->execute();
			if (exit_flag == 1) {//if current command is exit
				break;
			}
		}
	}

};

class Semicolon :public Base {
protected:
	vector<Base*> commd;
public:
	void addNode(Base* Node) {
		commd.push_back(Node);
	}
	void removeNode(Base* Node) {
		for (int i = 0; i < (int)commd.size(); i++) {
			if (commd.at(i) == Node) {
				commd.erase(commd.begin() + i);
			}
		}
	}
	void execute() {
		for (int i = (int)commd.size() - 1; i >= 0; i--) {
			flag = 0;
			exit_flag = 0;
			commd.at(i)->execute();
			if (exit_flag == 1) {//if current command is exit
				break;
			}
		}
	}
};
class Parser {
protected:
	stack<Base*> Cmdstack;//store single commands and combination commands with "&&" and "||"

public:
	Semicolon* parse(char inputBuffer[]) {
		char *argv[30][20];//store every single command after parse
		string argv1[30][20];
		string temp = "";//store argument of every command temporarily during the process of parse

		Semicolon* root = new Semicolon();//root node
		queue<string>Argumentqueue;//store arguments of every single command
		stack<string>Symbolstack;//store symbols like ';', '&&' and '||'
		int cursor = 0;//number of single commands
		char *p, *q;//*p is the pointer of current character, *q is the next character of *p
		int i = 0;
		for (p = inputBuffer; ; p++) {
			q = p + 1;
			if (*p == ' ') {
				continue;//skip space
			}
			temp = temp + *p;
			if ((*q == ' ' || *q == ';' || *q == '\0' || *q == '#' || *q == '&' || *q == '|' || *q == ']' || *q == ')') && ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '\"' || *p == '[' || *p == ']')) {
				//if the next charater is one of these, create a argument item in queue
				Argumentqueue.push(temp);//push an argument into queue
				temp = "";
			}
			if (*p == '[' && *q != ' ') {
				if (temp == "[") {
					Argumentqueue.push("[");//push "[" into queue
					temp = "";
				}		
			}

			if (*p == '\0' || *p == '#') {//this is the end of the whole input, or just meet with '#'
				i = 0;
				if (!Argumentqueue.empty()) {
					while (!Argumentqueue.empty()) {
						argv1[cursor][i] = Argumentqueue.front();//put all arguments into one command space
						i++;
						Argumentqueue.pop();
					}
					for (int m = 0; m < i; m++) {
						argv[cursor][m] = (char*)argv1[cursor][m].data();//change type from "string" to "char*"
					}
					argv[cursor][i] = NULL;
					Base* singlecmd = new Command(argv[cursor]);//create a single command class 
					Cmdstack.push(singlecmd);//
				}
				

				if (Symbolstack.empty())//there is no symbol left, means that we have already combined all single commands with "&&" or "||"
				{
					while (!Cmdstack.empty()) {
						root->addNode(Cmdstack.top());//every node is a single command or composition command with "&&" or "||"
						Cmdstack.pop();
					}
					root->execute();
					return root;
				}
				else {
					string t = Symbolstack.top();//there are symbols left
					if (t != "&&" && t != "||") {//the symbol on the top of the stack is ';'

						while (!Cmdstack.empty()) {
							root->addNode(Cmdstack.top());
							Cmdstack.pop();
						}
						root->execute();
						return root;
					}
					if (t == "&&") {//the symbol on the top of the stack is "&&"
						Symbolstack.pop();
						Base* right = Cmdstack.top();
						Cmdstack.pop();
						Base* left = Cmdstack.top();
						Cmdstack.pop();
						Base* andcmd = new And(left, right);//composition of "&&"
						Cmdstack.push(andcmd);//add this composition into stack

						while (!Cmdstack.empty()) {
							root->addNode(Cmdstack.top());
							Cmdstack.pop();
						}
						root->execute();
						return root;
					}
					if (t == "||") {//the symbol on the top of the stack is "||"
						Symbolstack.pop();
						Base* right = Cmdstack.top();
						Cmdstack.pop();
						Base* left = Cmdstack.top();
						Cmdstack.pop();
						Base* orcmd = new Or(left, right);//composition of "||"
						Cmdstack.push(orcmd);//add this composition into stack

						while (!Cmdstack.empty()) {
							root->addNode(Cmdstack.top());
							Cmdstack.pop();
						}
						root->execute();
						return root;
					}
				}
			}
			if (*p == '(') {//if current character is '('
				Symbolstack.push("(");
				temp = "";
			}
			if (*p == ')') {
				Parenthesis* paren = new Parenthesis();
				i = 0;

				if (!Argumentqueue.empty()) {
					while (!Argumentqueue.empty()) {
						argv1[cursor][i] = Argumentqueue.front();//put all arguments into one command space
						i++;
						Argumentqueue.pop();
					}
					for (int m = 0; m < i; m++) {
						argv[cursor][m] = (char*)argv1[cursor][m].data();//change type from string to char*
					}
					argv[cursor][i] = NULL;
					Base* singlecmd = new Command(argv[cursor]);//create a single command
					Cmdstack.push(singlecmd);//add this command into stack
				}
				

				if (Symbolstack.empty()) {
					//there is no "(" but has ")"
					cout << "syntax error near unexpected token ')'" << endl;
				}
				else {
					//there are symbols in stack now
					while (!Symbolstack.empty()) {
						string t = Symbolstack.top();
						if (t == ";") {//the symbol on top of the stack is ';'
							Symbolstack.pop();
							paren->addNode(Cmdstack.top());
							Cmdstack.pop();
						}
						if (t == "&&") {//the symbol on top of the stack is '&&'
										//then create a "and" composition first
							Symbolstack.pop();
							Base* right = Cmdstack.top();
							Cmdstack.pop();
							Base* left = Cmdstack.top();
							Cmdstack.pop();
							Base* andcmd = new And(left, right);
							Cmdstack.push(andcmd);
						}
						if (t == "||") {//the symbol on top of the stack is '||'
										//then create a "or" composition first
							Symbolstack.pop();
							Base* right = Cmdstack.top();
							Cmdstack.pop();
							Base* left = Cmdstack.top();
							Cmdstack.pop();
							Base* orcmd = new Or(left, right);
							Cmdstack.push(orcmd);
						}
						if (t == "(") {//complete the "()" structure
							Symbolstack.pop();
							paren->addNode(Cmdstack.top());
							Cmdstack.pop();
							Cmdstack.push(paren);
							break;
						}
					}
				}
				temp = "";//move on to next command
				cursor++;
				continue;
			}
			if (*p == ';') {//if current character is ';'

				i = 0;
				if (!Argumentqueue.empty()) {
					while (!Argumentqueue.empty()) {
						argv1[cursor][i] = Argumentqueue.front();//put all arguments into one command space
						i++;
						Argumentqueue.pop();
					}
					for (int m = 0; m < i; m++) {
						argv[cursor][m] = (char*)argv1[cursor][m].data();//change type from string to char*
					}
					argv[cursor][i] = NULL;
					Base* singlecmd = new Command(argv[cursor]);//create a single command
					Cmdstack.push(singlecmd);//add this command into stack
				}
				

				if (Symbolstack.empty()) {
					//there is no symbol in stack now
					Symbolstack.push(";");//put current symbol into symbol stack
				}
				else {
					//there are symbols in stack now
					string t = Symbolstack.top();
					if (t != "&&" && t != "||") {//the symbol on top of the stack is ';'
						Symbolstack.push(";");
					}
					if (t == "&&") {//the symbol on top of the stack is '&&'
						//then create a "and" composition first
						Symbolstack.pop();
						Base* right = Cmdstack.top();
						Cmdstack.pop();
						Base* left = Cmdstack.top();
						Cmdstack.pop();
						Base* andcmd = new And(left, right);
						Cmdstack.push(andcmd);
						Symbolstack.push(";");
					}
					if (t == "||") {//the symbol on top of the stack is '||'
						//then create a "or" composition first
						Symbolstack.pop();
						Base* right = Cmdstack.top();
						Cmdstack.pop();
						Base* left = Cmdstack.top();
						Cmdstack.pop();
						Base* orcmd = new Or(left, right);
						Cmdstack.push(orcmd);
						Symbolstack.push(";");
					}
				}
				temp = "";//move on to next command
				cursor++;
				continue;
			}
			if (*p == '&') {
				if (*q == '&') {
					//we meet with '&&'
					i = 0;
					if (!Argumentqueue.empty()) {
						while (!Argumentqueue.empty()) {
							argv1[cursor][i] = Argumentqueue.front();//put all arguments into one command space
							i++;
							Argumentqueue.pop();
						}
						for (int m = 0; m < i; m++) {
							argv[cursor][m] = (char*)argv1[cursor][m].data();// change type from string to char*
						}
						argv[cursor][i] = NULL;
						Base* singlecmd = new Command(argv[cursor]);//create a single command
						Cmdstack.push(singlecmd);
					}
					

					if (!Symbolstack.empty()) {//there are symbols in stack
						string t = Symbolstack.top();
						if (t == "&&") {//the symbol on top is '&&'
							Symbolstack.pop();
							Base* right = Cmdstack.top();
							Cmdstack.pop();
							Base* left = Cmdstack.top();
							Cmdstack.pop();
							Base* andcmd = new And(left, right);//create a "and" composition
							Cmdstack.push(andcmd);
						}
						if (t == "||") {//the symbol on top is '||'
							Symbolstack.pop();
							Base* right = Cmdstack.top();
							Cmdstack.pop();
							Base* left = Cmdstack.top();
							Cmdstack.pop();
							Base* orcmd = new Or(left, right);//create a "or" composition
							Cmdstack.push(orcmd);
						}
					}
					Symbolstack.push("&&");//push '&&' into stack
					temp = "";//move on to the next command
					cursor++;
					continue;
				}
				temp = "";
			}

			if (*p == '|') {
				if (*q == '|') {
					//we meet with '||'
					i = 0;
					if (!Argumentqueue.empty()) {
						while (!Argumentqueue.empty()) {
							argv1[cursor][i] = Argumentqueue.front();// put all arguments into one command
							i++;
							Argumentqueue.pop();
						}
						for (int m = 0; m < i; m++) {
							argv[cursor][m] = (char*)argv1[cursor][m].data();//change type from string to char*
						}
						argv[cursor][i] = NULL;
						Base* singlecmd = new Command(argv[cursor]);//create a single command
						Cmdstack.push(singlecmd);//add it into stack
					}
					

					if (!Symbolstack.empty()) {//there are symbols in stack
						string t = Symbolstack.top();
						if (t == "&&") {//the symbol on top is '&&'
							Symbolstack.pop();
							Base* right = Cmdstack.top();
							Cmdstack.pop();
							Base* left = Cmdstack.top();
							Cmdstack.pop();
							Base* andcmd = new And(left, right);//create a "and" composition
							Cmdstack.push(andcmd);
						}
						if (t == "||") {//the symbol on top is '||'
							Symbolstack.pop();
							Base* right = Cmdstack.top();
							Cmdstack.pop();
							Base* left = Cmdstack.top();
							Cmdstack.pop();
							Base* orcmd = new Or(left, right);//create a "or" composition
							Cmdstack.push(orcmd);
						}
					}
					Symbolstack.push("||");//push '||' into stack
					temp = "";
					cursor++;
				}
				temp = "";
			}
		}
	}
};
int main()
{
	char inputBuffer[1000];
	while (1) {
		int i = 0;

		for (int j = 0; j < 1000; j++) {
			inputBuffer[j] = '\0';
		}//initialize inputBuffer
		cout << "$" << " ";
		cin.getline(inputBuffer, 500);//get input

		i = 0;
		string exit = "";
		while (inputBuffer[i] != '\0') {
			exit = exit + inputBuffer[i];
			i++;
		}
		if (exit == "exit") {//user input "exit"
			break;
		}

		Parser* parsecommd = new Parser();
		parsecommd->parse(inputBuffer);
		if (exit_flag == 1) {//there is a "exit" in a combination of commands
			break;
		}
	}
	return 0;
}
