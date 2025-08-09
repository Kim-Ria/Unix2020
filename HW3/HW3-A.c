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

int main() {
	int fd, in;
	pid_t pid, n;
	static struct sigaction act;

	// "data1" 파일에서 B 프로그램의 process id 읽는다.
	fd = open("data1", O_RDWR | O_CREAT, 0600);
	// B 먼저 open된 후 A에도 TRUNC 옵션 있으면 싹 지워짐 > 문제

	do { // B 프로그램의 pid 알아낼 때까지 대기해야한다.
		n = read(fd, &pid, sizeof(pid_t));
		sleep(2); // 2~3초씩 대기하고 읽어야 낭비 덜하다
	} while (n <= 0);

	// 표준 입력으로 정수 하나 입력받아, data1 파일에 쓴다.
	scanf("%d", &in);
	while (in > 0) {
		write(fd, &in, sizeof(int)); // data1 파일에 쓴다.
		printf("writes ..... %d\n", in);

		kill(pid, SIGUSR1); // B 프로그램에게 SIGUSR1 signal 보내기

		scanf("%d", &in);
	}

	printf("bye...\n");
	kill(pid, SIGUSR1); // 음수: B 프로그램에게 SIGUSR1 signal 보낸다.
	exit(0);
}