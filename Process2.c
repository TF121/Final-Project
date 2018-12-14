// Process2.c

#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <time.h>

int main() 
{ 
	int fd;
	// 파일 경로
	char * FIFO_PIPE = "/home/s201421266/181209/FIFO_PIPE"; 

	// 실행한 시간 출력
	struct tm *date;
	const time_t t = time(NULL);
	date = localtime(&t);

	printf("\n[%d/%d/%d %d:%d:%d]\n\n", date->tm_year + 1900 , date->tm_mon + 1 ,
			date->tm_mday , date->tm_hour , date->tm_min , date->tm_sec);

	// 메세지 형태
	struct message
	{
		char p2_send[300];    // Process1.c 에 보낼 문자열(메세지)
		char p2_receive[300]; // Process1.c 로부터 받아올 문자열(메세지)
	} m;

	while (1) 
	{ 
		// [2nd]
		// 읽기 전용으로 파일을 연다 
		fd = open(FIFO_PIPE, O_RDONLY);
		// Process1.c 이 쓴 문자열을 읽는다
		read(fd, m.p2_receive, 300); 
		// 읽어온 문자열을 출력하고 파일을 닫는다
		printf("Sawyer: %s\n\n", m.p2_receive);
		close(fd);
		sleep(1);

		// [4th]
		// 쓰기 전용으로 파일을 연다 
		fd = open(FIFO_PIPE, O_WRONLY);
		printf("메세지 입력: ");
		// 입력한 문자열을 읽는다
		fgets(m.p2_send, 300, stdin);
		// 읽어온 문자열(m.p2_send)을 FIFO_PIPE 파일에 쓰고 파일을 닫는다
		write(fd, m.p2_send, strlen(m.p2_send)+1);
		close(fd);
		sleep(1);
	} 
	return 0; 
} 
