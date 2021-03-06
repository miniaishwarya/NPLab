//FTP 
//CLIENT PROGRAM
//AUTHOR :AISWARYA M
//ROLL NO : 7

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
	struct sockaddr_in address1,address2;
	struct stat obj;
	int sock_descr,temp_descr;
	socklen_t len;
	char buf[100],choice[10],length[100],filename[100];
	int k,i;
	int filehandle;
	memset(&address1,0,sizeof(address1));//setting memory
	memset(&address2,0,sizeof(address2));
	sock_descr=socket(AF_INET,SOCK_STREAM,0);//socket creation
	if(sock_descr==-1)
	{
		printf("socket creation failed");
		exit(1);
	}
	address1.sin_family=AF_INET;
	address1.sin_port=3003;
	address1.sin_addr.s_addr=INADDR_ANY;//inet_addr(argv[1]);
	k=bind(sock_descr,(struct sockaddr*)&address1,sizeof(address1));//binding socket to port
	if(k==-1)
	{
		printf("binding error");
		exit(1);
	}
	k=listen(sock_descr,5);
	if(k==-1)
	{
		printf("listen failed");
		exit(1);
	}
	len=sizeof(address2);
	temp_descr=accept(sock_descr,(struct sockaddr*)&address2,&len);//creating temporary descriptor
	if(temp_descr==-1)
	{
		printf("temp: socket creation failed");
		exit(1);
	}
	while(1)
	{
		k=recv(temp_descr,buf,100,0);//receiving from socket
		if(k==-1)
		{
			printf("receive failed");
			exit(1);
		}
		for(i=0;i<4;i++)
			choice[i]=buf[i];
		choice[4]='\0';
		printf("\n%s",choice);
		if(strcmp(choice,"LIST")==0)
		{
			system("ls -al>list.txt");
			filehandle=open("list.txt",O_RDONLY);//
			stat("list.txt",&obj);//
			sprintf(length,"%d",(int)obj.st_size);
			k=send(temp_descr,length,strlen(length),0);
			if(k==-1)
			{
				printf("send failed");
				exit(1);
			}
			k=sendfile(temp_descr,filehandle,NULL,obj.st_size);
			if(k==-1)
			{
				printf("file sendingfailed");
				exit(1);
			}
		}
		if(strcmp(choice,"LOAD")==0)
		{
			strcpy(filename,buf+4);
			stat(filename,&obj);
			filehandle=open(filename,O_RDONLY);
			if(filehandle==-1)//no file of that name
			{
				printf("NO SUCH FILE\n");
				exit(1);
			}
			sprintf(length,"%d",(int)obj.st_size);
			printf("\n%s\n",length);
			k=send(temp_descr,length,strlen(length),0);//sending to socket
			if(k==-1)
			{
				printf("send failed");
				exit(1);
			}
			sendfile(temp_descr,filehandle,NULL,obj.st_size);
			if(k==-1)
			{
				printf("file sendingfailed");
				exit(1);
			}
		}
	}
return 0;
}

//OUTPUT
/*LIST
LOADNO SUCH FILE
*/
