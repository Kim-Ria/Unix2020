#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

void cat_m(char **res) {
	char buf[512]; // buffer ũ��� 512 bytes �Դϴ�. �� �κ��� �������� ������.
	int fd, n;

	//�ڵ��ۼ�
	fd = open(res[1], O_RDONLY);

	if (fd == -1) return; // ���� > return

	while ((n = read(fd, buf, 512)) > 0) {
		write(1, buf, n); // ǥ�� ��� write ���
	}
}

void cd_m(char **res) {
	// �ڵ��ۼ�
	chdir(res[1]); // �ش� �̸� directory�� �̵�
}

void cp_m(char **res) {
	char buf[512]; // buffer ũ��� 512 bytes �Դϴ�. �� �κ��� �������� ������.
	int fd1, fd2, n;

	// �ڵ��ۼ�
	fd1 = open(res[1], O_RDONLY);

	if (fd1 == -1) return; // ���� > return

	fd2 = open(res[2], O_WRONLY | O_CREAT | O_TRUNC, 0600); // �����ϸ� ����� ����

	while ((n = read(fd1, buf, 512)) > 0) {
		write(fd2, buf, n); // fd1 ���� fd2�� ����
	}
}

void mkdir_m(char **res) {
	// �ڵ��ۼ�
	mkdir(res[1], 0700); // �ش� �̸� subdir �����
}

void ls_m(char **res) {
	// �ڵ��ۼ�
	DIR *dp;
	struct dirent *d;
	struct stat buf;
	int n;
	char time[512];

	dp = opendir(".");

	while (d = readdir(dp)) {
		lstat(d->d_name, &buf); // �� ����

		if (d->d_name[0] == '.') continue; // .���� �����ϴ� ���� ��� X

		if (S_ISREG(buf.st_mode)) printf("F "); // File
		else printf("D "); // Dir

		printf("0%o %d ", buf.st_mode & 0777, buf.st_nlink); // permission8����, link��
		printf("%d %d %ld ", buf.st_uid, buf.st_gid, buf.st_size);

		strcpy(time, ctime(&buf.st_mtime));
		n = strlen(time);
		time[n - 1] = '\0'; // �ٹٲ� ���� ��� null ����

		printf("%s %s\n", time, d->d_name); // ������ update��¥, file�̸�
	}
}

int main(void) { // main �Լ��� �������� ����, ��� �ϼ���.
	char name[100], in[50], *res[20] = { 0 };
	int i;

	while (1) {
		// current working directory �̸� ���
		getcwd(name, 99); // cwd �̸� ��������
		printf("%s> ", name);
		gets(in);
		i = 0;
		res[i] = strtok(in, " ");

		while (res[i]) {
			i++;
			res[i] = strtok(NULL, " ");
		}

		if (!strcmp(res[0], "cat")) {
			cat_m(res);
		}
		else if (!strcmp(res[0], "cd")) {
			cd_m(res);
		}
		else if (!strcmp(res[0], "cp")) {
			cp_m(res);
		}
		else if (!strcmp(res[0], "mkdir")) {
			mkdir_m(res);
		}
		else if (!strcmp(res[0], "ls")) {
			ls_m(res);
		}
		else
			exit(0);

	}
}