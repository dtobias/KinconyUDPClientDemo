/*
	UDP client for Kincony network relay

	Copyright 2020 "Captain" Dave Tobias

This software is covered by the MIT software license.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and 
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.	
	
*/


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include	<winsock2.h>

#pragma comment(lib,"ws2_32.lib")	//Tells linker to add winsock library to list of dependencies as if manually doing so via Linker->Input->Additional dependencies

#define BUFLEN 512					//Max length of buffer

struct		sockaddr_in SocketIn;
int			s;
int			slen = sizeof(SocketIn);
char		rcvbuf[BUFLEN];
char		message[BUFLEN];
WSADATA		wsa;



int SendUDPMessage(void)
{
	//send the message
	if (sendto(s, message, strlen(message), 0, (struct sockaddr*) & SocketIn, slen) == SOCKET_ERROR)
	{
		return(WSAGetLastError());
	}
	else
	{
		return(0);
	}
}


int GetUDPMessage(void)
{
	//receive a reply back into the global message buffer
	// set all locations to zero, thus ensuring that the resulting string will be null terminated per cstring requirements
	memset(rcvbuf, 0, BUFLEN);

	//try to receive some data, this is a blocking call
	//This assumes incoming port will be the same as destination port
	if (recvfrom(s, rcvbuf, BUFLEN, 0, (struct sockaddr*) & SocketIn, &slen) == SOCKET_ERROR)
	{
		return(WSAGetLastError());	
	}
	else 
	{
		return(0);
	}
}



int	InitWinsock(char *ServerIPAddr, int	ServerPort)
{
	//Initialize winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return(WSAGetLastError());
	}

	//create socket
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == SOCKET_ERROR)
	{
		return(WSAGetLastError());
	}

	//setup address structure
	memset((char*)& SocketIn, 0, sizeof(SocketIn));
	SocketIn.sin_family = AF_INET;
	SocketIn.sin_port = htons(ServerPort);
	SocketIn.sin_addr.S_un.S_addr = inet_addr(ServerIPAddr);

	return(0);
}

void CloseWinsock(void)
{
	closesocket(s);
	WSACleanup();
}