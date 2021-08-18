#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
const int MYPORT = 50000;

char *TARGET_HOST = "127.0.0.1";
 
int main(int argc, char* argv[])
{
	const char *targeHost = NULL;
	int port = 0;
	if (argc > 1 && argv[1] != NULL)
    {
        targeHost = argv[1];
    }
    else
    {
        targeHost = TARGET_HOST;
    }
    if (argc > 2 && argv[2] != NULL)
    {
        port = atoi(argv[2]);
    }
    else
    {
        port = MYPORT;
    }
	//1.创建套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		perror("socket");
		return 1;
	}
	
	//2.建立链接
	struct hostent *he;
	if((he = gethostbyname(targeHost)) == NULL)
    {
        printf("gethostbyname() error\n");
        return -1;
    }
	printf("Target Host name : %s\n", he->h_name);
    printf("Target IP Address : %s\n", inet_ntoa(*((struct in_addr *)he->h_addr)));
	printf("Target Host name : %s\n", he->h_name);
	struct sockaddr_in remote;
	memset(&remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_port = htons((unsigned short)port);
	remote.sin_addr = *((struct in_addr *)he->h_addr);;
	printf("Client is connecting to %s:%d ......\n", targeHost, port);
	if (connect(fd, (struct sockaddr *)&remote, sizeof(remote)) < 0)
	{
		perror("connect");
		return 2;
	}
 
	char buf[1024];
	memset(buf, '\0', sizeof(buf));
	int msgId = 0;
	while (1)
	{
		printf("[%04d] Client > ", msgId++);
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
			printf("\nquit...\n");
			break;
		}
		else
		{
			buf[sz] = '\0';
			printf("	 echo <<  %s\n", buf);
		}
	}
 
 
	return 0;
}