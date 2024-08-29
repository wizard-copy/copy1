#include "myheadfiles.h"
#include "menu.h"
#include "client.h"
 #include <pthread.h>
#define NUM  1024
int i;
int r ;
extern int clientFds[NUM];
int flag_private=0;
int flag_group=0;
int flag1;
pid_t pid1;
int clientFd;
void handle_signal(int signal);
int main(){
    
    menu();
    return 0;
}


void sock()
{
    clientFd = socket(AF_INET,SOCK_STREAM,0); 
    if(-1 == clientFd) printf("创建socketfd失败:%m\n"),exit(-1);
    printf("创建socketfd %m\n");

    struct sockaddr_in sAddr={0};
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sAddr.sin_port = htons(6569);//使用1W左右的
    r = connect(clientFd,(struct sockaddr*)&sAddr,sizeof sAddr);
    if(-1 == r) printf("连接服务器失败:%m\n"),close(clientFd),exit(-1);
                printf("连接服务器:%m\n");
}

int communication(){
       // 4 通信
    fd_set set;
    int maxFd;
    while(1){//循环监视
        //准备好要监视的描述符号
        FD_ZERO(&set);

        FD_SET(clientFd,&set);
        maxFd=clientFd;
        FD_SET(1,&set);
        maxFd = (maxFd>1) ? maxFd : 1;

        //开始监视
        r = select(maxFd+1,&set,NULL,NULL,NULL);
        if(-1 == r){
            printf("监视失败:%m!\n");
            return -1;
        }else if (0 == r){
            printf("没有动静!\n");
        }else{
            if(FD_ISSET(1,&set)){//用户输入了
                char buff[256] = {0};
                    while(1){
                    // signal(SIGINT, handle_signal);
                    // signal(SIGQUIT, handle_signal);
                    printf("群聊");
                    printf("请输入群发数据\n");
                    memset(buff,'@',256);
                    scanf("%s",buff+1);
                    r = send(clientFd,buff,strlen(buff),0);
                    printf("向服务器发送了%d字节数据!\n",r-1);
                  
            }// while(1){


              
            char buff1[1024];
            if(FD_ISSET(clientFd,&set)){//服务器发数据过来了
                r = recv(clientFd,buff1,1023,0);
                if(r>0){
                    buff1[r] = 0;//添加结束符号
                    printf("%s\n",buff1);
                }else{
                    printf("服务器关闭了!\n");
                    close(clientFd);
                    return -1;
                }
            }
        }//{//用户输入
           }//else{

            
    }//while(1){//循环监视
    
}//int communication()
  
int communication1(){
       // 4 通信
    fd_set set;
    int maxFd;
    while(1){//循环监视
        //准备好要监视的描述符号
        FD_ZERO(&set);

        FD_SET(clientFd,&set);
        maxFd=clientFd;
        FD_SET(1,&set);
        maxFd = (maxFd>1) ? maxFd : 1;

        //开始监视
        r = select(maxFd+1,&set,NULL,NULL,NULL);
        if(-1 == r){
            printf("监视失败:%m!\n");
            return -1;
        }else if (0 == r){
            printf("没有动静!\n");
        }else{
          
            if(FD_ISSET(1,&set)){//用户输入了
                   
                     char buff[256] = {0};
                    char buff1[259]={0};
                    
                    while(1){
                    buff1[0]='#';
                    buff1[2]=':';
                    printf("私聊\n");
                    printf("请输入对象：\n");
                    fflush(stdout);
                    getchar();
                    scanf("%c",&buff1[1]);
                    getchar();
                    printf("请输入私发数据\n");
                    scanf("%s",buff);
                    //  getchar();
                    strcat(buff1,buff);
                    r = send(clientFd,buff1,strlen(buff1),0);
                    memset(buff1,0,259);
                    memset(buff1,0,259);
                    printf("向服务器发送了%d字节数据!\n",r-3);
                    // printf("%d",flag1);
                    // flag1=-1;
                    
                  
            }// while(1){

          
            char buff2[1024];
            if(FD_ISSET(clientFd,&set)){//服务器发数据过来了
                r = recv(clientFd,buff2,1023,0);
                if(r>0){
                    buff2[r] = 0;//添加结束符号
                    printf("%s\n",buff1);
                }else{
                    printf("服务器关闭了!\n");
                    close(clientFd);
                    return -1;
                }
            }
        }//{//用户输入
           }//else{

            
    }//while(1){//循环监视
    
}//int communication()
void handle_signal(int signal) {

    switch (signal) {
        case SIGUSR1:
            break;
        case SIGUSR2:
           
            break;
        case SIGINT:
            printf("群聊\n");
            break;
        case SIGQUIT:
            printf("私聊\n");
            break;
        default:
            printf("未知信号\n");
    }
}