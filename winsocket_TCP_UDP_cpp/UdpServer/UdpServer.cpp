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

	struct hostent FAR * lpHostEnt=gethostbyname(PCname);
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
    servsock=socket(AF_INET,SOCK_DGRAM,0);//数据报套接字

	int servport=5555;
	int iSockErr=0;

	//定义服务器地址结构
	sockaddr_in udpaddr,cliaddr;
	memset(&udpaddr,0,sizeof(udpaddr));
	memset(&cliaddr,0,sizeof(cliaddr));

	int clilen =sizeof(cliaddr);
    udpaddr.sin_family=AF_INET;
    udpaddr.sin_port=htons(servport);
	//将一个点分十进制IP地址字符串转换成32位数字表示的IP地址
    udpaddr.sin_addr.s_addr=inet_addr(IPaddress);//"127.0.0.1"//INADDR_ANY

	//绑定套接字到服务器地址结构
	printf("Binding...\n");
	iSockErr=bind(servsock,(sockaddr *)&udpaddr,sizeof(udpaddr));
	if (iSockErr==SOCKET_ERROR)
	{
		printf("Binding failed:%d\n",WSAGetLastError());//根据不同的错误类型进行不同的处理
		exit(-3);
	}
	//函数调用成功，进行其他处理

	char buff[256]="\0";
   

	while (1)
	{
		char buffer[1024]="\0";
		printf("waiting for message from others-------------\n");
		if (recvfrom(servsock,buffer,sizeof buffer,0,(struct sockaddr*)&cliaddr,&clilen)!=SOCKET_ERROR)
		{
			printf("Received datagram from UDP Client %s--%s\n",inet_ntoa(cliaddr.sin_addr),buffer);

			sprintf(buff,"Welcome you %s",inet_ntoa(cliaddr.sin_addr));
			////给cilent发信息
			sendto(servsock,buff,sizeof buff,0,(struct sockaddr*)&cliaddr,clilen);

		}
		Sleep(500);
	}

	 //关闭连接
	shutdown(clisock,2);
	shutdown(servsock,2);
    closesocket(clisock);
    closesocket(servsock);
    WSACleanup();


}