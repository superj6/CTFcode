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

pid_t pid = 0;
int inpipefd[2];
int outpipefd[2];
char buf[256];
char msg[256];
int status;

void runcmd(string s, string arg = ""){
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

int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	return 0;
}