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

	//获取本机IP地址
	char PCname[100]={""};
	char *IPaddress=NULL;	     
	gethostname(PCname,sizeof(PCname));	   
	printf("Local Hostname is %s.\n",PCname);

	struct hostent FAR * lpHostEnt = gethostbyname(PCname);
	if(lpHostEnt==NULL)
	{
		//产生错误
		printf("gethostbyname failed!\n");
		return;
	}

	//获取IP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//转换为标准格式
		IPaddress=inet_ntoa(inAddr);//将一个32位数字表示的IP地址转换成点分十进制IP地址字符串
		if(sizeof(IPaddress)==0)
			printf("get host IP failed!\n");
		else
			printf("Local HostIP is %s.\n",IPaddress);
	}

	//创建套接字
	SOCKET servsock,clisock;
    printf("Create Socket...\n");
    servsock=socket(AF_INET,SOCK_STREAM,0);

	int servport=5555; //定义服务器所使用的端口号
	int iSockErr=0;

	//定义套接字地址结构指针
	sockaddr_in tcpaddr,cliaddr; 
	memset(&tcpaddr,0,sizeof(tcpaddr)); // 内存初始化
    tcpaddr.sin_family=AF_INET;
    tcpaddr.sin_port=htons(servport);

	//将一个点分十进制IP地址字符串转换成32位数字表示的IP地址
    tcpaddr.sin_addr.s_addr=inet_addr(IPaddress);//"127.0.0.1"//INADDR_ANY

	//绑定套接字到服务器地址结构
	printf("Binding...\n");
	iSockErr=bind(servsock,(sockaddr *)&tcpaddr,sizeof(tcpaddr));
	if (iSockErr==SOCKET_ERROR)
	{
		printf("Binding failed:%d\n",WSAGetLastError());//根据不同的错误类型进行不同的处理
		exit(-3);
	}
	//函数调用成功，进行其他处理

	//监听套接字
   printf("Listening...\n");
   iSockErr=listen(servsock,5);// 5 代表的是 backlog 指定了正在等待连接的最大队列长度
   if(iSockErr!=0)
   {
     printf("Listen failed:%d\n",WSAGetLastError());
     exit(-4);
   }

   int len=0;
   //等待连接请求
   printf("Waitting TCP Request...\n");
   len=sizeof(cliaddr);
   Sleep(20000);
   printf("sleep 20000ms\n");
   clisock=accept(servsock,(struct sockaddr *)&cliaddr,&len); //返回client的socket
   //服务器端在这里会进入一个阻塞状态，直到客户端返回连接信息！

   printf("Accept TCP Client:%s:%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

   char buff[256];
   sprintf(buff,"Welcome you %s",inet_ntoa(cliaddr.sin_addr));
   //发送欢迎词
    send(clisock,buff,strlen(buff),0);

	while (1)
	{
		char buffer[1024]="\0";
		printf("waiting for message from others-------------\n");
		if (recv(clisock,buffer,sizeof buffer,0)!=SOCKET_ERROR)
		{
			if (strcmp(buffer, "q") == 0)
				break;
			else{
				printf("Received datagram from TCP Client %s--%s\n", inet_ntoa(cliaddr.sin_addr), buffer);
				////给cilent发信息
				printf("client's port is %d\n", ntohs(cliaddr.sin_port));
				send(clisock, buff, sizeof buff, 0);
			}

		}//else
		//	break;
		
		Sleep(500);
	}


	
   //关闭连接
	shutdown(clisock,2);
	shutdown(servsock,2);
    closesocket(clisock);
    closesocket(servsock);
    WSACleanup();

}