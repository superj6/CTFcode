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
#define f first
#define s second

pid_t pid;
int inpipefd[2];
int outpipefd[2];
char buf[256];
int status;

void runcmd(string s = "", string arg0 = "", string arg1 = "", string arg2 = ""){
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

void delay(int milliseconds){
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

string cmdin(){
	delay(50);
	read(inpipefd[0], buf, 256);
	return string(buf);
}

int cmdint(){
	int ret = 0;
	while(1){
		read(inpipefd[0], buf, 1);
		if(buf[0] == ' ' || buf[0] == '\n') break;
		ret = 10 * ret + buf[0] - '0';
	}
	return ret;
}

void cmdout(string s){
	s += '\n';
	write(outpipefd[1], s.c_str(), (int)s.size());
}

void cmdout(int x){
	cmdout(to_string(x));
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	
	return 0;
}
