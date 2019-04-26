#include <Winsock2.h>
#include <stdio.h>

//需要在Project->Settings->Link->Object/library modules中加入ws2_32.lib
//  ***************************非阻塞模式***********************************


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

	printf("Load ws2_32.dll successfully!\n");


	//创建套接字 非阻塞模式
	unsigned long ul = 1;
	SOCKET servsock;
    printf("Create Socket...\n");
    servsock=socket(AF_INET,SOCK_STREAM,0); // SOCK_STREAM or SOCK_DGRAM
	int ret = ioctlsocket(servsock, FIONBIO, (unsigned long *)&ul);  // 设置为非阻塞模式
	if (ret == SOCKET_ERROR){
		printf("非阻塞模式设置失败！\n");
		return;
	}




	int servport=5555;
	int iSockErr=0;

	//定义服务器地址结构
	sockaddr_in tcpaddr;
	memset(&tcpaddr,0,sizeof(tcpaddr));
    tcpaddr.sin_family=AF_INET;
    tcpaddr.sin_port=htons(servport);
	//将一个点分十进制IP地址字符串转换成32位数字表示的IP地址
    tcpaddr.sin_addr.s_addr=inet_addr("115.156.135.127");//INADDR_ANY"127.0.0.1"
	
	



	// 连接服务器
	iSockErr=connect(servsock,(const sockaddr*)&tcpaddr,sizeof(tcpaddr));
	if (iSockErr == SOCKET_ERROR)
	{
		printf("connect unsccessful!\n");
		iSockErr = closesocket(servsock);
		if (iSockErr == SOCKET_ERROR)
			printf("closesocket unsccessful!\n");		
	}

	 char buffer[1024]="\0";
	int len=0;
	
	//接收欢迎词
  memset(buffer,0,sizeof(buffer));
  len=recv(servsock,buffer,sizeof(buffer),0);
  
  printf("%s\n",buffer);

  while (1)
  {

		printf("input message\n");
		scanf_s("%s",buffer,sizeof(buffer));


		if (send(servsock,buffer,sizeof(buffer),0)!=SOCKET_ERROR)
		{	
			if (recv(servsock,buffer,sizeof buffer,0)!=SOCKET_ERROR)
				printf("Received datagram from TCP server:%s\n",buffer);
		}

		if (strcmp(buffer, "q") == 0)
			break;
  
  }
  
  //关闭连接
  shutdown(servsock,2);

  closesocket(servsock);	
	
	WSACleanup();

}