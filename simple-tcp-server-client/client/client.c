a#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
const short port = 8080;
char *ip = "127.0.0.1";
 
int main()
{
	//1.创建套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("socket");
		return 1;
	}
	
	//2.建立链接
	struct sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	remote.sin_addr.s_addr = inet_addr(ip);
	
	if (connect(fd, (struct sockaddr *)&remote, sizeof(remote)) < 0)
	{
		perror("connect");
		return 2;
	}
 
	char buf[1024];
	memset(buf, '\0', sizeof(buf));
	while (1)
	{
		printf("Please Enter# ");
		fflush(stdout);
		ssize_t _s = read(0, buf, sizeof(buf) - 1);
		if (_s > 0)
		{
			buf[_s] = '\0';
		}
		write(fd, buf, strlen(buf));
		ssize_t sz = read(fd, buf, sizeof(buf) - 1);
		if (sz < 0)
		{
			perror("read");
			return 3;
		}
		else if (sz == 0)
		{
			printf("read done...");
			break;
		}
		else
		{
			buf[sz] = '\0';
			printf("server echo# %s\n", buf);
		}
	}
 
 
	return 0;
}