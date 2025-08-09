#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

void cat_m(char **res) {
	char buf[512]; // buffer 크기는 512 bytes 입니다. 이 부분은 수정하지 마세요.
	int fd, n;

	//코드작성
	fd = open(res[1], O_RDONLY);

	if (fd == -1) return; // 실패 > return

	while ((n = read(fd, buf, 512)) > 0) {
		write(1, buf, n); // 표준 출력 write 사용
	}
}

void cd_m(char **res) {
	// 코드작성
	chdir(res[1]); // 해당 이름 directory로 이동
}

void cp_m(char **res) {
	char buf[512]; // buffer 크기는 512 bytes 입니다. 이 부분은 수정하지 마세요.
	int fd1, fd2, n;

	// 코드작성
	fd1 = open(res[1], O_RDONLY);

	if (fd1 == -1) return; // 실패 > return

	fd2 = open(res[2], O_WRONLY | O_CREAT | O_TRUNC, 0600); // 존재하면 지우고 열기

	while ((n = read(fd1, buf, 512)) > 0) {
		write(fd2, buf, n); // fd1 내용 fd2에 복사
	}
}

void mkdir_m(char **res) {
	// 코드작성
	mkdir(res[1], 0700); // 해당 이름 subdir 만들기
}

void ls_m(char **res) {
	// 코드작성
	DIR *dp;
	struct dirent *d;
	struct stat buf;
	int n;
	char time[512];

	dp = opendir(".");

	while (d = readdir(dp)) {
		lstat(d->d_name, &buf); // 상세 정보

		if (d->d_name[0] == '.') continue; // .으로 시작하는 정보 출력 X

		if (S_ISREG(buf.st_mode)) printf("F "); // File
		else printf("D "); // Dir

		printf("0%o %d ", buf.st_mode & 0777, buf.st_nlink); // permission8진수, link수
		printf("%d %d %ld ", buf.st_uid, buf.st_gid, buf.st_size);

		strcpy(time, ctime(&buf.st_mtime));
		n = strlen(time);
		time[n - 1] = '\0'; // 줄바꿈 문자 대신 null 문자

		printf("%s %s\n", time, d->d_name); // 마지막 update날짜, file이름
	}
}

int main(void) { // main 함수는 수정하지 말고, 사용 하세요.
	char name[100], in[50], *res[20] = { 0 };
	int i;

	while (1) {
		// current working directory 이름 출력
		getcwd(name, 99); // cwd 이름 가져오기
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