#include "myheadfiles.h"
#include "menu.h"
#include "client.h"
extern int flag1;
extern  int clientFd;
extern int r ;
struct sockaddr_in sAddr;
extern char buff1;
void close_socket(int fd) {  
    close(fd);  
}  
int check_login(const char *username, const char *password) {  
    FILE *file = fopen("userdata.txt", "r");  
    if (!file) {  
        printf("登录检查失败，无法打开文件\n");  
        return -1;  
    }   
    
    char buffer[256];  
    while (fgets(buffer, sizeof(buffer), file)) {  
        char stored_username[128], stored_password[128];  
        if (sscanf(buffer, "%127s %127s", stored_username, stored_password) == 2) {  
            if (strcmp(stored_username, username) == 0 && strcmp(stored_password, password) == 0) {  
                fclose(file);  
                return 1;  
            }  
        }  
    }  
  
    fclose(file);  
    return 0;  
}  

int check_username_exists(const char *username) {  
    FILE *file = fopen("userdata.txt", "r");  
    if (!file) {  
        printf("检查用户名失败，无法打开文件\n");  
        return -1;  
    }  
  
    char buffer[256];  
    while (fgets(buffer, sizeof(buffer), file)) {  
        char stored_username[128];  
        if (sscanf(buffer, "%127s", stored_username) == 1) {  
            if (strcmp(stored_username, username) == 0) {  
                fclose(file);  
                return 1;  
            }  
        }  
    }  
  
    fclose(file);  
    return 0;  
}  
  
  
void get_user_input(char *username, char *password) {  
    printf("请输入用户名:");  
    fgets(username, 128, stdin);  
    username[strcspn(username, "\n")] = 0;  
    printf("请输入密码:");  
    fgets(password, 128, stdin);  
    password[strcspn(password, "\n")] = 0;  
}  
  
void send_message(int clientFd) {  
    char message[1024];  
    printf("请输入你的消息:");  
    fgets(message, sizeof(message), stdin);  
    message[strcspn(message, "\n")] = 0; // 移除fgets留下的换行符  
  
    char buff[1024];  
    snprintf(buff, sizeof(buff), "MESSAGE %s", message);  
    if (send(clientFd, buff, strlen(buff), 0) == -1) {  
        perror("发送失败");  
        close(clientFd);  
        exit(EXIT_FAILURE);  
    } else {  
        printf("消息已发送\n");  
    }  
}  
int logged_in = 0;  // 用于跟踪用户是否登录 


//功能实现
void menu()
{
    char username[128] = {0}, password[128] = {0};  // 初始化用户名和密码数组  
   
    int choice;
   
  
  
    while (1) {  
        printf("\n1. 注册\n2. 登录\n3.退出\n选择操作:");  
        scanf("%d", &choice);  
        char c;  
        while ((c = getchar()) != '\n' && c != EOF) { } // 清除输入缓冲区  
  
  
        switch (choice) {  
            case 1: // 注册  
                get_user_input(username, password);  
  
                if (check_username_exists(username)) {  
                    printf("用户名已存在，请尝试其他用户名\n");  
                    break;  
                }  
  
                FILE *file = fopen("userdata.txt", "a");  
                if (!file) {  
                    perror("打开文件失败");  
                    break;  
                }  
                fprintf(file, "%s %s\n", username, password);  
                fclose(file);  
  
                char buff[256];  
                snprintf(buff, sizeof(buff), "REGISTER %s %s", username, password);  
                printf("注册成功");
                break;  
  
            case 2: // 登录  
                get_user_input(username, password);  
  
                if (check_login(username, password)) {  
                    printf("登录成功\n"); 
                    sock(); 
                    logged_in = 1; 
                    tongxing(); 
                } else {  
                    printf("登录失败\n");  
                }  
                break;  
  
               
  
            case 3: // 退出  
                close(clientFd);  
                exit(0); 
        
  
            default:  
                printf("无效的操作选项\n");  
        }  
    }  
  
    close(clientFd);  
}

   
void tongxing(){      
    if (logged_in) {
        while (1) {  
            printf("\n0. 群发\n1. 私发\n2. 接受数据\n3.退出\n选择操作:"); 
            int choice1; 
            scanf("%d", &choice1);  
            char c;  
            while ((c = getchar()) != '\n' && c != EOF) { } // 清除输入缓冲区 
            switch (choice1) {  
                case 0:communication();//群发
                case 1:communication1();//私发
                case 2:recv_data();//接受数据
                case 3:tongxing();
                default:  
                printf("无效的操作选项\n");  

            }  
                
       }
    }
                }
            
             void recv_data(){
                printf("接受数据");
                 char buff3[1024];
                    while(1){
                    r = recv(clientFd,buff3,1023,0);
                    if(r>0){
                    buff3[r] = 0;//添加结束符号
                    printf("%s\n",buff3); 
                }
                    }
                }