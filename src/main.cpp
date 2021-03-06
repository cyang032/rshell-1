#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <cstring>
#include <vector>
using namespace std;

// execvp funtion
bool execute(char* args[],int contype){

    int pid=fork();
    int status;
    if(pid==-1){
		perror("error in fork");
        exit(1);
	}
	else if (pid ==0) { //in child process
		status=execvp(args[0],args);

        if(status!=0){
            perror("error in execvp");
        }

        exit(1);
	}
    // in parent
	else if (pid>0) {
        int result=waitpid(pid,&status,0);
            if(result==-1){
            perror("error in wait");
        }
        if( status==0){ // SUCCESSS
            if(contype==0)       // EOL
                return false;
             if(contype==1)      // OR
                return false;
             if(contype==2)      // SEMI
                return true;
              if(contype==3)   // AND
                return true;

       }
       else if(status>0) { //FAILURE
              if(contype==0)   //EOL
                return false;
              if(contype==1)   //OR
                return true;
              if(contype==2)   //SEMI
                return true;
              if(contype==3)    //AND
                return false;
            }
       }
return true;
}


char exitS[]="exit";
char exitB[]="Exit";

int main(){

    string user;
    char host[3200];
    user= getlogin();
    gethostname(host,3200);
	string str="";
	char cmd[320000];
    char connectorparsed[32000];
	int tokenindex=0;
	char* savedTokens[32000];
    vector<char> myvector;
    char* args[3333];
    bool run=true;
    string connector="";
    char andd[]={'&','&','\0'};
    char orr[]={'|','|','\0'};
    char semi[]={';','\0'};


    while(1){
    run=true;
    myvector.clear();
    tokenindex = 0;
   	cout<<user<<"@"<<host<< "$ ";
	getline(cin,str);		// get input at str
	strcpy(cmd,str.c_str());	// get c string of str
   	strcpy(connectorparsed,str.c_str());
    for(int i=0;cmd[i]!='\0';i++){
        if( cmd[i]=='#' ){
            myvector.push_back('\0');
        break;
        }
        else if( cmd[i]==';' ){
            myvector.push_back(' ');
            myvector.push_back(cmd[i]);
            myvector.push_back(' ');
        }
        else if(( cmd[i]=='&'&& cmd[i+1]=='&')||( cmd[i]=='|'&& cmd[i+1]=='|')){
            myvector.push_back(' ');
            myvector.push_back(cmd[i]);
            myvector.push_back(cmd[i]);
            myvector.push_back(' ');
            i++;
        }
        else{
            myvector.push_back(cmd[i]);
         }
    }
    for(size_t i=0;i<myvector.size();i++){
        connectorparsed[i]=myvector.at(i);
    }
    char* tokens= strtok(connectorparsed," ");

    while(tokens!= NULL){

        savedTokens[tokenindex]= strdup(tokens);
        tokenindex++;
		tokens=strtok(NULL," ");
		}                   // done tokenizing

    savedTokens[tokenindex]=NULL;

	if(strcmp(exitS,savedTokens[0])==0||strcmp(exitB,savedTokens[0])==0){
	return 0;
    }

    int contype=-1;
    for(int i=0, j=0;run;i++,j++){
       // cout<< savedTokens[i]<<endl;

        if(savedTokens[i]==NULL){
         args[j]=NULL;
         run=execute(args,contype);
          break;

        }
        else if(strcmp(savedTokens[i],andd)==0){
           // cout<<" ANDD"<< endl;
            contype=3;
         }
        else if(strcmp(savedTokens[i],orr)==0){
          // cout<< "ORR" << endl;
            contype=1;
         }
        else if(savedTokens[i][0] == semi[0] && savedTokens[i][1] == '\0'){
          // cout << "SEMI"<< endl;
            contype=2;
           // cout << "semi"<< endl;
         }
        else{
            args[j]=savedTokens[i];
          // cout << "arg: "<<savedTokens[i]<<endl;
          // cout<< "andd: " << andd << endl;
            continue;
        }
        run=execute(args,contype);
        j=-1;

    }
}
}



