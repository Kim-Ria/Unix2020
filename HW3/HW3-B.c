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

	// "data1" ���Ͽ� B���α׷��� process id�� ����.
	fd = open("data1", O_RDWR | O_CREAT | O_TRUNC, 0600);

	pid = getpid();
	write(fd, &pid, sizeof(pid_t));

	// SIGUSR1 signal�� �� ������ ����Ѵ�.
	pause();

	// SIGUSR1 signal ������,
	while ((n = read(fd, &in, sizeof(int))) > 0) { // data1 ���Ͽ��� ���� �ϳ� �д´�.
		printf("reads... %d\n", in);
		pause();
	}

	printf("bye...\n"); // A ���α׷��� �����ϸ�, �����Ѵ�. (data1 ���� �� ����)
	exit(0);

}

void catchusr1(int signo) {
}