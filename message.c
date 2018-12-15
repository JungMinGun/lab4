#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFSIZE 100

// 수신된 메시지를 전체 client에게 전송하며, 연결이 끊어지면 client hanle을 소멸시킨다.
void clnt_connection(void *arg);
// 메시지를 모든 client에게 전송한다.
void send_message(char * message, int len);
// 에러 발생에 대한 처리
void error_handling(char *message);

int clnt_number = 0;        // 연결된 client의 개수를 저장할 변수 선언
int clnt_socks[10];         // 연결된 모든 client 모두에게 메시지를 전송하여야 하기 때문에
                            // 모든 client 소켓의 정보를 전역변수로 선언 함
pthread_mutex_t     mutx;   // mutex 동기화 하기 위한 mutex선언


int main(int argc, char *argv[])
{
    int     serv_sock;                  // server socket handle 선언
    int     clnt_sock;                  // cleint socket handle 선언
    struct  sockaddr_in     serv_addr;  // server socket address 구조체 선언
    struct  sockaddr_in     clnt_addr;  // client socket address 구조체 선언
    int     clnt_addr_size;             // client socket address 구조체의 크기
    pthread_t   thread;                 // pthread_t 자료형 선언(thread ID)
    void     *thread_rtn;

    // port번호 인자값 확인
    if(argc!=2){
        printf("Usage : %s <port> \n", argv[0]); exit(1);
    }

    // mutex 초기화
    if(pthread_mutex_init(&mutx, NULL))
        error_handling("pthread_mutex_init() error");

    // server socket handle 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // server socket address 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons((unsigned short)atoi(argv[1]));

    // server socket handle에  server address 정보 bind
    if(bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    // server socket handle의 연결 대기 상태로 변경
    if(listen(serv_sock, 10) == -1)
        error_handling("listen() error");

    while(1){
        // 연결요청 시 사용될 client socket address의 크기 설정
        clnt_addr_size = sizeof(clnt_addr);
        // 연결을 수락하여 client socket handle을 생성한다.
        // 이때 client socket address 정보도 함께 받아 진다.
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        // 전역으로 선언된 변수가 임계영역으로 다른 thread의 영향을 받으므로 mutex lock을 건다
        pthread_mutex_lock(&mutx);
        // 연결되어 생성된 client handle을 client handle소켓 배열에 저장한다.
        clnt_socks[clnt_number++]=clnt_sock;
        // mutex unlock으로 해제한다.
        pthread_mutex_unlock(&mutx);

        // 연결된 client 소켓의 handle을 clnt_connection함수 호출 시 인자로 넘겨주어 thread를 생성한다.
        pthread_create(&thread, NULL, clnt_connection, (void *)clnt_sock);
        printf("New client connected, IP: %s \n", inet_ntoa(clnt_addr.sin_addr));
    }

    pthread_join(thread, &thread_rtn);
    
    // mutex를 해제한다.
    pthread_mutex_destroy(&mutx);
    return EXIT_SUCCESS;
}

void * clnt_connection(void * arg)
{
    int clnt_sock = (int)arg;   // 인자로 넘겨 받은 client handle을 형변환을 하여 새로운 변수로 저장한다.
    int str_len = 0;            // client로 부터 수신한 메시지의 길이를 저장하는 변수 선언
    char message[BUFSIZE];      // client로 부터 수신된 메시지를 저장하는 변수 선언
    int i;

    // client로 부터 메시지를 수신 받는다.
    // 수신된 메시지의 길이가 0인 경우는 client의 연결종료를 의미하며, 루프를 빠져 나온다.
    while( (str_len=read(clnt_sock, message, sizeof(message))) != 0)
        send_message(message, str_len);  // 수신된 메시지를 모든 클라이언트에 전송한다.

    // 전역으로 선언된 변수가 임계영역으로 다른 thread의 영향을 받으므로 mutex lock을 건다
    // lock의 위치를 for문 밖에서 선언하고 있다.
    // for문 안에 선언시 수시로 반복되기 때문에 실행속도에 영향을 미친다.
    pthread_mutex_lock(&mutx);
    for(i=0; i<clnt_number; i++){

        // 연결종료된 client handle값을 clent배열의 값과 같으면
        if(clnt_sock == clnt_socks[i]){
            // 그위치를 시작점으로 하여 뒤의 소켓 정보로 한개씩 앞으로 이동시킨다.
            // 즉, 현재 연결종료된 client의 handle 정보는 삭제가 된다.
            for( ; i<clnt_number-1; i++)    
                clnt_socks[i]=clnt_socks[i+1];
            break;
        }
    }
    clnt_number--;
    // mutex unlock으로 해제한다.
    pthread_mutex_unlock(&mutx);

    close(clnt_sock);
    return 0;
}

// 현재 연결되어 있는 모든 client에게 메시지를 전송한다.
void send_message(char * message, int len)
{
    int i;

    // 단순히 참조하는 과정에서도 전역변수로 선언된 임계영역의 변수는 변경될 수 있기 때문에
    // mutex lock을 설정하여야 한다.
    pthread_mutex_lock(&mutx);
    for(i=0; i<clnt_number; i++)
        // 연결된 client socket handle수 만큼 반복하여
        // 모든 client에게 같은 메시지를 전송한다.
        write(clnt_socks[i], message, len);
    pthread_mutex_unlock(&mutx);
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
