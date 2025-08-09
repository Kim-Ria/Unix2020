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

	// "data1" ���Ͽ��� B ���α׷��� process id �д´�.
	fd = open("data1", O_RDWR | O_CREAT, 0600);
	// B ���� open�� �� A���� TRUNC �ɼ� ������ �� ������ > ����

	do { // B ���α׷��� pid �˾Ƴ� ������ ����ؾ��Ѵ�.
		n = read(fd, &pid, sizeof(pid_t));
		sleep(2); // 2~3�ʾ� ����ϰ� �о�� ���� ���ϴ�
	} while (n <= 0);

	// ǥ�� �Է����� ���� �ϳ� �Է¹޾�, data1 ���Ͽ� ����.
	scanf("%d", &in);
	while (in > 0) {
		write(fd, &in, sizeof(int)); // data1 ���Ͽ� ����.
		printf("writes ..... %d\n", in);

		kill(pid, SIGUSR1); // B ���α׷����� SIGUSR1 signal ������

		scanf("%d", &in);
	}

	printf("bye...\n");
	kill(pid, SIGUSR1); // ����: B ���α׷����� SIGUSR1 signal ������.
	exit(0);
}