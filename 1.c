#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>//下面需要的各种函数需要的头文件
int main(int argc, char *argv[])
{   //服务器的搭建首先进行网络编程。
    //1，创建套接字
    int sockfd;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("brind1:");
        return 0;
    }//2，绑定
    struct sockaddr_in ser;
    ser.sin_family=AF_INET;
    ser.sin_port=htons(9001);//端口号
    ser.sin_addr.s_addr=inet_addr("192.111.111.111"); //如果为0则自动寻找
    int ret=bind(sockfd,(struct sockaddr *)&ser,sizeof(ser));
    if(ret==-1)
    {
        perror("bind2:");
        return 0;
    }
    //监听
    while(1)
    {
    printf("listing----------\n");
    ret=listen(sockfd,5);//能容纳的最多的客户端的访问。
     2if(ret ==-1)
    {
        perror("bind3:");
        return 0;
    }
    /  /接受
    int connfd=accept(sockfd,NULL,NULL);
    if(connfd<=0)
    {
        perror("accept:");
        return 0;
    }
    printf("accept connfd=%d\n",connfd);
    //数据收发

    while(1)//以下是一些功能。
    {
        char recv_buf[1024]="";
        ret=read(connfd,recv_buf,sizeof(recv_buf));
        if(ret<=0)
        {
            printf("客户端推出\n");
            break;
        }
	if(!strcmp(recv_buf,"chakan"))//查看服务器程序所在文件的目录
	{   	 
		memset(recv_buf,0,sizeof(recv_buf));
		printf("客户端申请查看目录\n");
		DIR * fd;
  		fd=opendir(".");
               struct dirent *dir;
        	while(dir=readdir(fd))
        	{
         		printf("name:%s\n",dir->d_name);
				strcat(recv_buf,"\n");
				strcat(recv_buf,dir->d_name);
        	}
		printf("目录访问完成！\n");
		printf("总共的目录：%s\n",recv_buf);
        int a=write(connfd,recv_buf,strlen(recv_buf));
		printf("write=%d\n",a);
	}
	else if(!strcmp(recv_buf,"xiazai"))//下载服务器中的文件。
	{
		printf("客户端请求下载文件：\n");
	    char buf[1024]="请输入文件名字：";
		write(connfd,buf,strlen(buf));//写入了客户端的显示面版上。
		memset(buf,0,sizeof(buf));

		read(connfd,buf,sizeof
        (buf));
		printf("文件名：%s",buf);
		int fd;
		fd=open(buf,O_RDWR,0777);//在要求路径下进行下载文件的创建。
          
        char buf0[1024]="";
		//若想要客户端的显示面版上显示文字,就须定义一个buf0
		read(fd,buf0,sizeof(buf0));
		printf("文件内容：%s",buf0);
        int a= write(connfd,buf0,strlen(buf0));
        memset(buf0,0,sizeof(buf0));
		printf("write=%d\n",a);
	}
	else if(!strcmp(recv_buf,"shangchuang"))
	{
	    int fd2;
		printf("客户端请求上传文件:\n");//上传一个文件
		char buf[1024]="请输入文件名字";//这是上传文件在服务器的名字
		write(connfd,buf,strlen(buf));//写入了客户端的显示面版上。
		memset(buf,0,sizeof(buf));//置零
		read(connfd,buf,sizeof(buf));//由客户端读出。
		fd2=open(buf,O_RDWR|O_CREAT,0777);
        //在根据上传文件的名字服务器所在的目录上创建或覆盖一个文件储存上传的数据。
		printf("文件名：%s",buf);
	    memset(buf,0,sizeof(buf));//置零

	    char buf1[1024]="请输入文件内容";
		//若想要客户端的显示面版上显示文字,就须定义一个buf1
		write(connfd,buf1,strlen(buf1));
		read(connfd,buf,sizeof(buf));
        write(fd2, buf, strlen(buf));
		printf("文件内容：%s",buf);
        memset(buf,0,sizeof(buf));
		close(fd2);

	    char buf2[1024]="文件上传成功";
		//若想要客户端的显示面版上显示文字,就须定义一个buf2
		printf("%s\n",buf2);
		write(connfd,buf2,strlen(buf2));
		memset(buf2,0,sizeof(buf2));		
	}
	else
	  {
		printf("输入有误：");
	  }
        printf("recv:%s\n",recv_buf);
    }
 }
    return 0;    
} 
