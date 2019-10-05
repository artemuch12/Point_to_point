#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

/*Структура определяющая вид посылки*/
struct message
{
	long types;		/*Тип необходим для отделения технических и личных сообщений*/
	char text[255];
};

struct sockaddr_in addr_server;
int flag = 0;
int id_mq;
int fd_sock_inet;
socklen_t sockaddr_in_size;
pthread_t tech_channel;
pthread_t recevier;
key_t  mess_que;

/*Функция для обработки технических запросов*/
void *tech_function()
{
	struct message mess_tech;
	while(flag != 1)
	{
		msgrcv(id_mq, (void *) &mess_tech, sizeof(mess_tech), 2L, 0);
		mess_tech.text[0] = '1';
		mess_tech.types = 2L;
		sendto(fd_sock_inet, &mess_tech, sizeof(mess_tech), 0, (struct sockaddr *)&addr_server, sockaddr_in_size);
	}
	pthread_cancel(tech_channel);
}
/*Функция приема сообщений с той стороны*/
void *recevier_function()
{
	struct message mess_recev;
	while(flag != 1)
	{
		recvfrom(fd_sock_inet, &mess_recev, sizeof(mess_recev), 0, (struct sockaddr *) &addr_server, &sockaddr_in_size);
		if(mess_recev.types = 1L)
		{
			printf("(Companion): %s", mess_recev.text);
		}
		else if((mess_recev.types = 2L) && (0 == strcmp(mess_recev.text, "I'm going!")))
		{
			flag = 1;
			puts("(Companion): The companion left.");
		}
		else
		{
			mess_recev.types = 2L;
			msgsnd(id_mq, (void *) &mess_recev, sizeof(mess_recev), 0);
		}
	}
	pthread_cancel(recevier);
}

int main(int argc, char *argv[])
{
	void *status;
	int error;
	struct message mess;
	
	if(argc == 2)
	{
		inet_aton(argv[1], &addr_server.sin_addr);
	}
	else if(argc == 1)
	{
		inet_aton("127.0.0.1", &addr_server.sin_addr);
	}
	else
	{
		puts("Input error!");
		exit(-1);
	}
	mess_que = ftok("./client", 'C');
	sockaddr_in_size = sizeof(struct sockaddr_in);
	fd_sock_inet = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd_sock_inet < 1)
	{
		puts("Socket not open! Exit!");
		exit(-1);
	}
	
	/*Регистриуем интернет сокет*/
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(5005);
	
	mess.types = 3L;
	mess.text[0] = '0';
	sendto(fd_sock_inet, &mess, sizeof(mess), 0, (struct sockaddr *)&addr_server, sockaddr_in_size);
	recvfrom(fd_sock_inet, &mess, sizeof(mess), 0, (struct sockaddr *) &addr_server, &sockaddr_in_size);
	printf("%s", mess.text);
	
	/*Создаем очередь сообщений дял общения между потоками*/
	id_mq = msgget(mess_que, IPC_CREAT | 0666);
	if(id_mq == -1)
	{
		puts("Error! Not create message turn. Exit.");
		exit(-3);
	}
	/*Создаем всопомогательные потоки и начинаем пичатать сообщения*/
	error = pthread_create(&tech_channel, NULL, tech_function, NULL);
	if(error != 0)
	{
		puts("Error! Technical channel not create. Exit.");
		exit(-2);
	}
	error = pthread_create(&recevier, NULL, recevier_function, NULL);
	if(error != 0)
	{
		puts("Error! Recevier channel not create. Exit.");
		exit(-2);
	}
	
	while(flag != 1)
	{
		mess.types = 1L;
		fgets(mess.text, 255, stdin);
		if(flag != 1)
		{
			if(0 != strcmp(mess.text, "!exit!"))
			{
				sendto(fd_sock_inet, &mess, sizeof(mess), 0, (struct sockaddr *)&addr_server, sockaddr_in_size);
				
			}
			else
			{
				flag = 1L;
				strcpy(mess.text, "I'm going!");
				sendto(fd_sock_inet, &mess, sizeof(mess), 0, (struct sockaddr *)&addr_server, sockaddr_in_size);
			}
		}
	}
	pthread_join(recevier, &status);
	pthread_join(tech_channel, &status);
	msgctl(id_mq, IPC_RMID, 0);
	close(fd_sock_inet);
	exit(0);
}
