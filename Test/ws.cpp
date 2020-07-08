#include "UProtocol.h"

#define DEFAULT_PORT 8000
#define MAXLINE 4096





enum{FRAME_SIZE=1024};
string g_sHttpReq;
char g_sWsFrame[FRAME_SIZE];
int g_iWsFrameSize=FRAME_SIZE;
int g_iWsFrameLen=0;

void Init()
{
	g_sHttpReq="GET / HTTP/1.1\r\n";
	g_sHttpReq.append("Host: localhost:8080\r\n");
	g_sHttpReq.append("Connection: Upgrade\r\n");
	g_sHttpReq.append("Upgrade: websocket\r\n");
	g_sHttpReq.append("Origin: http://127.0.0.1:3000\r\n");
	g_sHttpReq.append("Sec-WebSocket-Version: 13\r\n");
	g_sHttpReq.append("Sec-WebSocket-Key: w4v7O6xFTi36lq3RNcgctw==\r\n");
	g_sHttpReq.append("\r\n");

	string str="hello";

	UBWsParser wp;
	g_iWsFrameLen=wp.WSSetFrame(TEXT_FRAME,(uint8_t*)str.data(),str.length(),(uint8_t*)g_sWsFrame,g_iWsFrameSize);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout<<argv[0]<<" 192.168.0.100 80"<<endl;
		return 0;
	}
	string ip=argv[1];
	uint16_t port=atol(argv[2]);

	Init();


	int sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		cout<<"create socket error:"<<strerror(errno)<<endl;
		return 0;
	}

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
		cout<<"inet_pton failed"<<endl;
		return 0;
	}

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		cout<<"connect failed:"<<strerror(errno)<<endl;
		return 0;
	}

	cout<<"send msg to server:"<<endl;
	if (send(sockfd, g_sHttpReq.data(), g_sHttpReq.length(), 0) < 0)
	{
		cout<<"send header failed:"<<strerror(errno)<<endl;
		return 0;
	}

	sleep(1);
	cout<<"send frame to server:"<<endl;
	if (send(sockfd, g_sWsFrame, g_iWsFrameLen, 0) < 0)
	{
		cout<<"send frame failed:"<<strerror(errno)<<endl;
		return 0;
	}

	sleep(1);
	char sBuffer[FRAME_SIZE];
	int ret=recv(sockfd, sBuffer, FRAME_SIZE, 0);

	if (ret<0)
	{
		cout<<"send frame failed:"<<strerror(errno)<<endl;
		return 0;
	}
	cout<<"recv:"<<ret<<endl;
	return 0;
}


