#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<ftw.h>
#include<time.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>

void catchusr1(int);

int main() {
	int fd, in, n, num, cnt;
	pid_t pid;
	static struct sigaction act;

	act.sa_handler = catchusr1;
	sigaction(SIGUSR1, &act, NULL);

	// "data1" 파일에 B프로그램의 process id를 쓴다.
	fd = open("data1", O_RDWR | O_CREAT | O_TRUNC, 0600);

	pid = getpid();
	write(fd, &pid, sizeof(pid_t));

	// SIGUSR1 signal이 올 때까지 대기한다.
	pause();

	// SIGUSR1 signal 받으면,
	while ((n = read(fd, &in, sizeof(int))) > 0) { // data1 파일에서 정수 하나 읽는다.
		printf("reads... %d\n", in);
		pause();
	}

	printf("bye...\n"); // A 프로그램이 종료하면, 종료한다. (data1 읽을 것 없음)
	exit(0);

}

void catchusr1(int signo) {
}