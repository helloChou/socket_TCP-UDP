#include <Winsock2.h>
#include <stdio.h>

//��Ҫ��Project->Settings->Link->Object/library modules�м���ws2_32.lib
//  ***************************������ģʽ***********************************


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

	printf("Load ws2_32.dll successfully!\n");


	//�����׽��� ������ģʽ
	unsigned long ul = 1;
	SOCKET servsock;
    printf("Create Socket...\n");
    servsock=socket(AF_INET,SOCK_STREAM,0); // SOCK_STREAM or SOCK_DGRAM
	int ret = ioctlsocket(servsock, FIONBIO, (unsigned long *)&ul);  // ����Ϊ������ģʽ
	if (ret == SOCKET_ERROR){
		printf("������ģʽ����ʧ�ܣ�\n");
		return;
	}




	int servport=5555;
	int iSockErr=0;

	//�����������ַ�ṹ
	sockaddr_in tcpaddr;
	memset(&tcpaddr,0,sizeof(tcpaddr));
    tcpaddr.sin_family=AF_INET;
    tcpaddr.sin_port=htons(servport);
	//��һ�����ʮ����IP��ַ�ַ���ת����32λ���ֱ�ʾ��IP��ַ
    tcpaddr.sin_addr.s_addr=inet_addr("115.156.135.127");//INADDR_ANY"127.0.0.1"
	
	



	// ���ӷ�����
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
	
	//���ջ�ӭ��
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
  
  //�ر�����
  shutdown(servsock,2);

  closesocket(servsock);	
	
	WSACleanup();

}