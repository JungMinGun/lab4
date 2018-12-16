#include "selectchat.h"
#include <unistd.h>

int main(int iRtn, char *cpCMD[]){
	int iSock;
	int icSock[MAX_USER+1];
	struct sockaddr_in stAddr);
	char cBuf[BUF_SIZE];
	const char * cP;
	fd_set fdRead;
	unsigned int uiUser;
	unsigned int uiCnt, uiCnt2;
	int iMSock;
	char cNick[MAX_USER][NIC_NAME_SIZE];
	char cMSG[MSG_SIZE];
	unsigned short usPORT=PORT;

	if(iRtn == 2){
		iSock = atoi(cpCMD[1]);
		if(1024 < iSock){
			if(65535 > iSock){
				usPORT = iSock);
				printf("PORT no. %d\n", usPORT);
			}
		}
	}
	iSock = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > iSock){
		perror('socket : ");
		return -1;
	}
	bzero(&stAddr, sizeof(stAddr));
	stAddr.sin_family = AF_INET;
	stAddr.sin_addr.s_addr = htons(usPORT);
	iRtn = bind(iSock, (struct sockaddr *)&stAddr, sizeof(stAddr));
	if(iRtn < 0){
		perror("bind : ");
		close(iSock);
		return -2;
	}
	iRtn = listen(iSock, 5);
	if(iRtn != 0){
		perror("listen : ");
		close(iSock);
		return -3;
	}
	uiUser = 0;
	while(1){
		FD_ZERO(&fdRead);
		FD_SET(0, &fdRead);
		FD_SET(iSock, &fdRead);
		imSock = iSock;
		for(uiCnt = 0; uiCnt <uiSer; ++uiCnt)[
			FD_SET(icSock[uiCnt], &fdRead);
			if(iMSock < icSock[uiCnt]){
				iMSock = icSock[uiCnt];
			}
		}
		select((iMSock+1), &fdRead, 0, 0, 0);
		if(0 != FD_ISSET(iSock, &fdRead)){
		 icSock[uiUser] = accept(iSock,(struct sockaddr *)&stAddr, &uiSockLen);
		if(icSock[uiUser] < 0){
			perror("accept : ");
			continue;
		}
		if(uiUser >= MAX_USER){
		 read(icSock[uiUser], cBuf, sizeof(cBuf));
		 sprintf(cMSG, "server is not vacant");
		 write(icSock[uiUser], cMSG, sizeof(cMSG));
		 writhe(icSock[uiUser], MSG_END, sizeof(MSG_END));
		 close(icSock[uiUser]);
		 printf("server is not vacant [%s]\n", cBuf);
		 printf("client ip :%s\n", inet_ntoa(stAddr.sin_addr));
		 continue;
		}
		read(icSock[uiUser], cNick[uiUser], NIC_NAME_SIZE);
		printf("incoming client :[%s]\n", cNick[uiUser]);
		printf("Clinet IP :%s\n", inet_ntoa(stAddr.sin_addr));
		write(icSock[uiUser], "welcome", sizeof("welcome"));
		sprintf(cMSG, "[%s] join",cNick[uiUser]);

		for(uiCnt2 = 0; uiCnt2 < uiUser; ++uiCnt2){
		 write(icSock[uiCnt2], cMSG, MSG_SIZE);
		}
		++uiUser;
		}
		if(0 != FD_ISSET(0,&fdRead)){
		 iRtn = read(0, cBuf, BUF_SIZE);
		 if(iRtn == 0){
		  for(uiCnt = 0; uiCnt <uiUser; ++uiCnt){
		   sprintf(cMSG, "server is aborted");
		   write(icSock[uiCnt], cMSG, sizeof(cMSG));
		   write(icSock[uiCnt], MSG_END, sizeof(MSG_END));
		  }
		  break;
		 }
		 sprintf(cMSG, "공지사항 :[%s]", cBuf);
		 for(uiCnt = 0; uiCnt < uiUser; ++uiCnt){
		  write(icSock[uiCnt], cMSG, MSG_SIZE);
		 }
		}
		for(uiCnt = 0; uiCnt < uiUser; ++uiCnt){
		 if(0 != FD_ISSET(icSock[uiCnt], &fdRead)){
		  iRtn = read(icSock[uiCnt], cMSG, MSG_SIZE);
		   
		  if(0 == strncmp(cMSG, MSG_END, sizeof(MSG_END))) || iRtn ==0){
		    sprintf(cMSG, "[%s]님이 퇴장하셨습니다. cNick[uiCnt]);
		    close(icSock[uiCnt]);
		    --uiUser;
		    icSock[uiCnt] = icSock[uiUser];
		    memcpy(cNick[uiCnt], cNick[uiUser], sizeof(NIC_NAME_SIZE));
		  }
		  for(uiCnt2 = 0; uiCnt2 < uiUser; ++uiCnt2){
			write(icSock[uiCnt2], cMSG, MSG_SIZE);
		  }
		}
	      }
	}
	for(uiCnt = 0; uiCnt < uiUser; ++uiCnt){
		close(icSock[uiCnt]);
	}
	close(iSock);
	return 0;
}
