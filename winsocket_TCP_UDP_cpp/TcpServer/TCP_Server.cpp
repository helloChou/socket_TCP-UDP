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

	struct hostent FAR * lpHostEnt = gethostbyname(PCname);
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
    servsock=socket(AF_INET,SOCK_STREAM,0);

	int servport=5555; //�����������ʹ�õĶ˿ں�
	int iSockErr=0;

	//�����׽��ֵ�ַ�ṹָ��
	sockaddr_in tcpaddr,cliaddr; 
	memset(&tcpaddr,0,sizeof(tcpaddr)); // �ڴ��ʼ��
    tcpaddr.sin_family=AF_INET;
    tcpaddr.sin_port=htons(servport);

	//��һ�����ʮ����IP��ַ�ַ���ת����32λ���ֱ�ʾ��IP��ַ
    tcpaddr.sin_addr.s_addr=inet_addr(IPaddress);//"127.0.0.1"//INADDR_ANY

	//���׽��ֵ���������ַ�ṹ
	printf("Binding...\n");
	iSockErr=bind(servsock,(sockaddr *)&tcpaddr,sizeof(tcpaddr));
	if (iSockErr==SOCKET_ERROR)
	{
		printf("Binding failed:%d\n",WSAGetLastError());//���ݲ�ͬ�Ĵ������ͽ��в�ͬ�Ĵ���
		exit(-3);
	}
	//�������óɹ���������������

	//�����׽���
   printf("Listening...\n");
   iSockErr=listen(servsock,5);// 5 ������� backlog ָ�������ڵȴ����ӵ������г���
   if(iSockErr!=0)
   {
     printf("Listen failed:%d\n",WSAGetLastError());
     exit(-4);
   }

   int len=0;
   //�ȴ���������
   printf("Waitting TCP Request...\n");
   len=sizeof(cliaddr);
   Sleep(20000);
   printf("sleep 20000ms\n");
   clisock=accept(servsock,(struct sockaddr *)&cliaddr,&len); //����client��socket
   //������������������һ������״̬��ֱ���ͻ��˷���������Ϣ��

   printf("Accept TCP Client:%s:%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

   char buff[256];
   sprintf(buff,"Welcome you %s",inet_ntoa(cliaddr.sin_addr));
   //���ͻ�ӭ��
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
				////��cilent����Ϣ
				printf("client's port is %d\n", ntohs(cliaddr.sin_port));
				send(clisock, buff, sizeof buff, 0);
			}

		}//else
		//	break;
		
		Sleep(500);
	}


	
   //�ر�����
	shutdown(clisock,2);
	shutdown(servsock,2);
    closesocket(clisock);
    closesocket(servsock);
    WSACleanup();

}