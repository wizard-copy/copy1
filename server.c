#include "myheadfiles.h"


#define NUM  1024
int flag1_rev;
int serverFd;
int clientFds[NUM];
int clientFdsIdx;//clientFds下标   当前clientFds数
int i_rev=0;
int i;
int cnt;
fd_set fds;//定义描述符号集合

void myExit();

int main(){
    //初始化客户端描述符号数组
    for(int i=0;i<NUM;i++){
        clientFds[i] = -1;
    }

    //当前连过的客户端数量  可以稍微优化一下
    clientFdsIdx = 0;
    //1
     serverFd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == serverFd) printf("创建socketfd失败:%m\n"),exit(-1);
    printf("创建socketfd %m\n");
    //2 协议地址簇
    struct sockaddr_in sAddr={0};
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sAddr.sin_port = htons(6569);//使用1W左右的

    //3 绑定

    int r = bind(serverFd,(struct sockaddr*)&sAddr,sizeof sAddr);
    if(-1 == r) printf("绑定失败:%m\n"),close(serverFd),exit(-1);
    printf("绑定 %m\n");
    


    //4 监听
    r = listen(serverFd,10);
     if(-1 == r) printf("监听失败:%m\n"),close(serverFd),exit(-1);
    printf("监听 %m\n");
   
   

    struct sockaddr_in cAddr={0};
    int addr_len=0;
    
    //记录当前最大的描述符号
    int maxFd = serverFd;
    int cfd;

    char buff[1024];
    memset(buff,-1,1024);
    char temp[1024];
    while(1){
        //把服务器的fd放进fds中
        FD_ZERO(&fds);
        FD_SET(serverFd,&fds);
        maxFd = serverFd;
        //把客户端的fd放进fds中
        for(int i=0;i<NUM;i++){
            if(-1 != clientFds[i]){
                FD_SET(clientFds[i],&fds);
                maxFd = ((maxFd > clientFds[i]) ? maxFd : clientFds[i]);
            }
        }

        r = select(maxFd+1,&fds,NULL,NULL,NULL);
         if(0 == r){
            printf("木有动静!\n");
        }else if(-1 == r){
            printf("监视失败，被发现了，赶紧跑路:%m!\n");
            myExit();
        }else{
            //printf("%d有动静!\n",r);
            if(FD_ISSET(serverFd,&fds)){
               
                clientFds[clientFdsIdx] = accept(serverFd,NULL,NULL);
                
                cfd = clientFds[clientFdsIdx];
                //maxFd = ((maxFd > cfd) ? maxFd : cfd);
                if(-1 == cfd){
                    printf("服务器崩溃:%m!");
                    myExit();
                }
                printf("%d连接上服务器了!\n",cfd);
                clientFdsIdx++;
                //把新链接上服务器的cfd放入fds
                //FD_SET(cfd,&fds);
            }


            for(int i=0;i<NUM;i++){
                if(-1 != clientFds[i] && 
                    FD_ISSET(clientFds[i],&fds)){
                        //意味着 clientFds[i]有动静
                       
                    r = recv(clientFds[i],buff,1023,0);
                    flag1_rev=buff[0];
                    i_rev=buff[1];
                    if(r > 0){

                        buff[r] = 0;
                        printf("\n服务器收到来自%d的数据:%s\n",
                        clientFds[i],buff);
                        memset(buff,0,1023);
                        //转发给当前连上服务器的所有客户端
                        //准备好要转发的数据
                        memset(temp,0,1024);

                        sprintf(temp,"%d:%s",clientFds[i],buff);
                       
                        
                        //转发
                        if(flag1_rev=='@'){//群聊
                            for(int j=0;j<NUM;j++){
                            if(clientFds[j] != -1){
                                send(clientFds[j],temp,strlen(temp),0);
                            }
                            memset(temp,0,1024);
                        }
                        }else if(flag1_rev=='#'){//私聊
                          char *temp1="连接用户成功\n";
                          printf("向用户%c转发数据\n", i_rev);
                          send(clientFds[i_rev-48],temp1,strlen(temp1),0);
                          send(clientFds[i_rev-48],temp,strlen(temp),0);
                          memset(temp,0,1024);
                        }
                        else{
                              printf("请选择群聊还是私聊\n");  
                        }
                        
                        break;//结束当前层for循环
                    }else{//客户端主动断开连接
                        printf("%d已下线!\n",clientFds[i]);
                        //fds中清除这个fd
                        //FD_CLR(clientFds[i],&fds);
                        //服务器关闭客户端fd
                        close(clientFds[i]);
                        //数组里重新设置为-1
                        clientFds[i] = -1;
                        break;//结束当前层for循环
                    }
                }//end of if(-1 != clientFds[i] && FD_ISSET(clientFds[i],&fds))
            }//end of for(int i=0;i<NUM;i++){
        }//end of select 返回值判断 if else
    }//end of while(1)
   


    



    return 0;
}


void myExit(){
    close(serverFd);
    for(int i=0;i<clientFdsIdx;i++)
        if(-1 != clientFds[i]) 
            close(clientFds[i]);
    exit(-1);
}