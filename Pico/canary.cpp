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

int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	pid_t pid = 0;
	int inpipefd[2];
	int outpipefd[2];
	char buf[256];
	char msg[256];
	int status;
	
	pipe(inpipefd);
	pipe(outpipefd);
	pid = fork();
	if(pid == 0){
		dup2(outpipefd[0], STDIN_FILENO);
		dup2(inpipefd[1], STDOUT_FILENO);
		dup2(inpipefd[1], STDOUT_FILENO);
		
		prctl(PR_SET_PDEATHSIG, SIGTERM);
		
		execl("/problems/canary_3_257a2a2061c96a7fb8326dbbc04d0328/vuln", "vuln", (char*)NULL);
		
		exit(1);
	}
	
	close(outpipefd[0]);
	close(inpipefd[1]);
	
	read(inpipefd[0], buf, 256);
	printf("%s\n", buf);
	write(outpipefd[1], "1\n", 2);
	read(inpipefd[0], msg, 256);
	printf("%s\n", msg);
	write(outpipefd[1], buf, strlen(buf));
	read(inpipefd[0], msg, 256);
	printf("%s\n", msg);
	
	kill(pid, SIGKILL);
	waitpid(pid, &status, 0);

	return 0;
}
