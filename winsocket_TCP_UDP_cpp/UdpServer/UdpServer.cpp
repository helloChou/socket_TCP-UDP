#include <Winsock2.h>
#include <stdio.h>

//��Ҫ��Project->Settings->Link->Object/library modules�м���ws2_32.lib

void main()
{


WORD wVersionRequested;

	WSADATA wsaData;

	wVersionRequested=MAKEWORD(2,2);

	if(WSAStartup(wVersionRequested,&wsaData)!=0)//��ʼ��ws2_32.dll��̬��
	{
		printf("WSAStartup() failed!\n");//Winsock��ʼ������

		exit(-1);
	}

	if(wsaData.wVersion!=wVersionRequested)
	{
		printf("The version of Winsock is not suited!\n");//Winsock�汾��ƥ��
		
		WSACleanup();//������ws2_32.dll�ĵ���

		exit(-2);

	}
	
	//˵��ws2_32.dll��ȷ����
	printf("Load ws2_32.dll successfully!\n");

	//��ȡ����IP��ַ
	char PCname[100]={""};
	char *IPaddress=NULL;	
	gethostname(PCname,sizeof(PCname));	
	printf("Local Hostname is %s.\n",PCname);

	struct hostent FAR * lpHostEnt=gethostbyname(PCname);
	if(lpHostEnt==NULL)
	{
		//��������
		printf("gethostbyname failed!\n");

		return;
	}
	//��ȡIP
	LPSTR lpAddr=lpHostEnt->h_addr_list[0];
	if(lpAddr)
	{
		struct in_addr inAddr;
		memmove(&inAddr,lpAddr,4);
		//ת��Ϊ��׼��ʽ
		IPaddress=inet_ntoa(inAddr);//��һ��32λ���ֱ�ʾ��IP��ַת���ɵ��ʮ����IP��ַ�ַ���
		if(sizeof(IPaddress)==0)
			printf("get host IP failed!\n");
		else
			printf("Local HostIP is %s.\n",IPaddress);
	}

	//�����׽���
	SOCKET servsock,clisock;
    printf("Create Socket...\n");
    servsock=socket(AF_INET,SOCK_DGRAM,0);//���ݱ��׽���

	int servport=5555;
	int iSockErr=0;

	//�����������ַ�ṹ
	sockaddr_in udpaddr,cliaddr;
	memset(&udpaddr,0,sizeof(udpaddr));
	memset(&cliaddr,0,sizeof(cliaddr));

	int clilen =sizeof(cliaddr);
    udpaddr.sin_family=AF_INET;
    udpaddr.sin_port=htons(servport);
	//��һ�����ʮ����IP��ַ�ַ���ת����32λ���ֱ�ʾ��IP��ַ
    udpaddr.sin_addr.s_addr=inet_addr(IPaddress);//"127.0.0.1"//INADDR_ANY

	//���׽��ֵ���������ַ�ṹ
	printf("Binding...\n");
	iSockErr=bind(servsock,(sockaddr *)&udpaddr,sizeof(udpaddr));
	if (iSockErr==SOCKET_ERROR)
	{
		printf("Binding failed:%d\n",WSAGetLastError());//���ݲ�ͬ�Ĵ������ͽ��в�ͬ�Ĵ���
		exit(-3);
	}
	//�������óɹ���������������

	char buff[256]="\0";
   

	while (1)
	{
		char buffer[1024]="\0";
		printf("waiting for message from others-------------\n");
		if (recvfrom(servsock,buffer,sizeof buffer,0,(struct sockaddr*)&cliaddr,&clilen)!=SOCKET_ERROR)
		{
			printf("Received datagram from UDP Client %s--%s\n",inet_ntoa(cliaddr.sin_addr),buffer);

			sprintf(buff,"Welcome you %s",inet_ntoa(cliaddr.sin_addr));
			////��cilent����Ϣ
			sendto(servsock,buff,sizeof buff,0,(struct sockaddr*)&cliaddr,clilen);

		}
		Sleep(500);
	}

	 //�ر�����
	shutdown(clisock,2);
	shutdown(servsock,2);
    closesocket(clisock);
    closesocket(servsock);
    WSACleanup();


}