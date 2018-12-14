// Process1.c

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
		char p1_send[300];	  // Process2.c 에 보낼 문자열(메세지)
		char p1_receive[300]; // Process2.c 로부터 받아올 문자열(메세지)
	} m;

	// FIFO_PIPE 파일 생성, 퍼미션 값 = 0707
	mkfifo(FIFO_PIPE, 0707);
 
	while (1) 
	{ 
		// [1st]
		// 쓰기 전용으로 파일을 연다 
		fd = open(FIFO_PIPE, O_WRONLY); 
		printf("메세지 입력: ");
		// 입력한 문자열을 읽는다
		fgets(m.p1_send, 300, stdin);
		// 읽어온 문자열(m.p1_send)을 FIFO_PIPE 파일에 쓰고 파일을 닫는다
		write(fd, m.p1_send, strlen(m.p1_send)+1);
		close(fd);
		sleep(1); // 채팅이 넘어가는 과정을 보여주기 위해 슬립을 1초간 걸어둠

		// [3rd]
		// 읽기 전용으로 파일을 연다
		fd = open(FIFO_PIPE, O_RDONLY); 
		// Process2.c 가 쓴 문자열을 읽는다
		read(fd, m.p1_receive, 300);
		// 읽어온 문자열을 출력한다
		printf("Dexter: %s\n\n", m.p1_receive);
		// 파일을 닫는다
		close(fd);
		sleep(1);
	} 
	return 0; 
} 
