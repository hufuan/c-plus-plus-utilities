#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
 
const short port = 50000;
char *ip = "127.0.0.1";
 
void *pthread_run(void *arg)
{
	int fd = (int)arg;
	 
 	char buf[1024];
 	memset(buf, '\0', sizeof(buf));
 	
	while (1)
	{
	 	ssize_t sz = read(fd, buf, sizeof(buf) - 1);
	 	if (sz < 0)
	 	{
	 		perror("read");
	 		break;
	 	}
	 	else if (0 == sz)
	 	{
	 		printf("read done...\n");
	 		break;
	 	}
	 	else
	 	{
	 		buf[sz - 1] = '\0';
			printf("client# %s\n", buf);
			write(fd, buf, strlen(buf));
		}
	}
	return 0;
}
 
int main()
{
	//1.创建套接字
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0)
	{
		perror("socket");
		return 1;
	}
	
	//2.绑定
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
 
	if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
	{
		perror("bind");
		return 2;
	}
	
	//3.监听
	if (listen(listen_sock, 5) < 0)
	{
		perror("listen");
		return 3;
	}
	
 
	while (1)
	{	
		//4.获取收到的套接字
		struct sockaddr_in peer;	//远端
		socklen_t len = sizeof(peer);	
		int fd = accept(listen_sock, (struct sockaddr *)&peer, &len);
		
		printf("new connect: %s:%d\n", inet_ntoa(peer.sin_addr), peer.sin_port);
 
		pthread_t id = pthread_create(&id, NULL, pthread_run, (void *)fd); //创建线程来服务客户
		pthread_detach(id);		
	}
 
 
	return 0;
}