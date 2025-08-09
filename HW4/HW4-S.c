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

	// fifo �����
	mkfifo(fa, 0600); // id ��ſ�
	for (i = 0; i < 6; i++) // client ��ſ�
		mkfifo(f[i], 0600);

	fda = open(fa, O_RDWR); // �׻� ���� �Ǿ�� �ϸ� RDWR
	for (i = 0; i < 6; i++)
		fd[i] = open(f[i], O_RDWR); // ��� ���� ������ fifo �����ǵ��� RDWR

	for (i = 0; i < 3; i++) // �ִ� 3�� ���
		write(fda, &i, sizeof(int));

	FD_ZERO(&master);
	for (i = 0; i < 3; i++)
		FD_SET(fd[i], &master); // �д� �κ� 0~2�� �����־� / write��  üũ ���ص� ��

	   // select
	t.tv_sec = 60;
	while (set = master, select(fd[2] + 1, &set, NULL, NULL, &t) > 0) { // 0�� ��� data ���� time out �ƴ� �ǹ�
		for (i = 0; i < 3; i++) {
			if (FD_ISSET(fd[i], &set)) {
				if ((n = read(fd[i], &in, sizeof(int))) > 0) { // data ������
					in += 8; // client ���� +8
					write(fd[i + 3], &in, sizeof(int)); // �ش� client���� �ٽ� ������.
				}
			}
		}
		t.tv_sec = 60;
	}

	exit(0);
}