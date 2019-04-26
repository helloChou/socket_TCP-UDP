#include <Winsock2.h>
#include <stdio.h>

//需要在Project->Settings->Link->Object/library modules中加入ws2_32.lib

void main()
{



WORD wVersionRequested;

	WSADATA wsaData;

	wVersionRequested=MAKEWORD(2,2);

	if(WSAStartup(wVersionRequested,&wsaData)!=0)//初始化ws2_32.dll动态库
	{
		printf("WSAStartup() failed!\n");//Winsock初始化错误

		exit(-1);
	}

	if(wsaData.wVersion!=wVersionRequested)
	{
		printf("The version of Winsock is not suited!\n");//Winsock版本不匹配
		
		WSACleanup();//结束对ws2_32.dll的调用

		exit(-2);

	}
	
	//说明ws2_32.dll正确加载
	printf("Load ws2_32.dll successfully!\n");

	//创建套接字
	SOCKET servsock;
    printf("Create Socket...\n");
    servsock=socket(AF_INET,SOCK_DGRAM,0);//数据报套接字

	int servport=5555;
	int iSockErr=0;

	//定义服务器地址结构
	sockaddr_in udpaddr;
	int len =sizeof(udpaddr);
	memset(&udpaddr,0,sizeof(udpaddr));
   udpaddr.sin_family=AF_INET;
    udpaddr.sin_port=htons(servport);
	//将一个点分十进制IP地址字符串转换成32位数字表示的IP地址
    udpaddr.sin_addr.s_addr=inet_addr("127.0.0.1");////INADDR_ANY

	while (1)
	{

		char buffer[1024]="\0";
		printf("input message\n");
		scanf("%s",buffer);
		if (strcmp(buffer,"q")==0)
			break;
		if (sendto(servsock,buffer,sizeof buffer,0,(struct sockaddr*)&udpaddr,len)!=SOCKET_ERROR)
		{	
			if (recvfrom(servsock,buffer,sizeof buffer,0,(struct sockaddr*)&udpaddr,&len)!=SOCKET_ERROR)
				printf("Received datagram from UDP server:%s\n",buffer);
		}		
	}

	 
  //关闭连接
  shutdown(servsock,2);
  closesocket(servsock);	
	
	WSACleanup();

}