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

int main(void) {
	char fa[5] = "fifo", f[6][3] = { "f1", "f2", "f3", "f4", "f5", "f6" };
	int i, in, fda, fd[6], n;
	fd_set set, master;
	struct timeval t;

	// fifo 만들기
	mkfifo(fa, 0600); // id 통신용
	for (i = 0; i < 6; i++) // client 통신용
		mkfifo(f[i], 0600);

	fda = open(fa, O_RDWR); // 항상 유지 되어야 하면 RDWR
	for (i = 0; i < 6; i++)
		fd[i] = open(f[i], O_RDWR); // 통신 없을 때에도 fifo 유지되도록 RDWR

	for (i = 0; i < 3; i++) // 최대 3명 통신
		write(fda, &i, sizeof(int));

	FD_ZERO(&master);
	for (i = 0; i < 3; i++)
		FD_SET(fd[i], &master); // 읽는 부분 0~2만 관심있어 / write는  체크 안해도 돼

	   // select
	t.tv_sec = 60;
	while (set = master, select(fd[2] + 1, &set, NULL, NULL, &t) > 0) { // 0인 경우 data 없이 time out 됐다 의미
		for (i = 0; i < 3; i++) {
			if (FD_ISSET(fd[i], &set)) {
				if ((n = read(fd[i], &in, sizeof(int))) > 0) { // data 왔으면
					in += 8; // client 정수 +8
					write(fd[i + 3], &in, sizeof(int)); // 해당 client에게 다시 보낸다.
				}
			}
		}
		t.tv_sec = 60;
	}

	exit(0);
}