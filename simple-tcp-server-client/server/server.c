#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include<signal.h>
const int MYPORT = 50000;
int listen_sock = -1;
int current_connection_sock = -1;
volatile int sub_thread_quit = 0; 
void handler(int sig)
{
	printf("Received INT sig: %d\n",sig);
	sub_thread_quit = 1;
	if ( listen_sock > 0) {
		close(listen_sock);
		listen_sock = -1;
	}
	exit(1);
}
 
void *pthread_run(void *arg)
{
	int fd = *(int*)arg;
	const int buff_size = 8 * 1024;
 	char* buf = malloc(buff_size);
 	memset(buf, '\0', buff_size);
 	//printf("handing fd:%d \n", fd);
 	int msgId = 0;
	while (1)
	{
	 	ssize_t sz = read(fd, buf, buff_size - 1);
	 	if (sz < 0)
	 	{
	 		perror("\nread");
	 		break;
	 	}
	 	else if (0 == sz)
	 	{
	 		printf("\nclient quit...\n");
	 		break;
	 	}
	 	else
	 	{
	 		buf[sz - 1] = '\0';
			printf("\n[%04d] server > %s", msgId++, buf);
			fflush(stdout);
			write(fd, buf, strlen(buf));
		}
	}
	close(fd);
	return 0;
}
 
int main(int argc, char* argv[])
{
	int port = 0;
 	if (argc > 1 && argv[1] != NULL)
    {
        port = atoi(argv[1]);
    }
    else
    {
        port = MYPORT;
    }

	signal(SIGINT,handler);
	
	//1.创建套接字
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0)
	{
		perror("socket");
		return 1;
	}
	
	//2.绑定
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((unsigned short)port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
 	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
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
	
 	printf("Server listening on port: %d ......\n", (int)port);
	while (!sub_thread_quit)
	{	
		//4.获取收到的套接字
		struct sockaddr_in peer;	//远端
		socklen_t len = sizeof(peer);	
		int fd = accept(listen_sock, (struct sockaddr *)&peer, &len);
		if (fd < 0){
			perror("accept");
			continue;
		}
		printf("new connect: %s:%d, fd: %d \n", inet_ntoa(peer.sin_addr), peer.sin_port, fd);
 
		pthread_t id;
		pthread_create(&id, (const pthread_attr_t *)NULL, pthread_run, (void *)&fd); //创建线程来服务客户
		//sleep(300);
		pthread_join(id, NULL);		
	}
 
 
	return 0;
}