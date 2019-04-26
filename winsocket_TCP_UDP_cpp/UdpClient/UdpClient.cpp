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

	//�����׽���
	SOCKET servsock;
    printf("Create Socket...\n");
    servsock=socket(AF_INET,SOCK_DGRAM,0);//���ݱ��׽���

	int servport=5555;
	int iSockErr=0;

	//�����������ַ�ṹ
	sockaddr_in udpaddr;
	int len =sizeof(udpaddr);
	memset(&udpaddr,0,sizeof(udpaddr));
   udpaddr.sin_family=AF_INET;
    udpaddr.sin_port=htons(servport);
	//��һ�����ʮ����IP��ַ�ַ���ת����32λ���ֱ�ʾ��IP��ַ
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

	 
  //�ر�����
  shutdown(servsock,2);
  closesocket(servsock);	
	
	WSACleanup();

}