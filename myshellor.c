#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<pwd.h>
#include<wait.h>
#include<sys/types.h>
void doPipe(char* cmd);
int main()
{
    printf("welcome to my shell!\n");
    char cmd[1024];
    while(1)
    {
        printf("[myshell]$ ");
        gets(cmd);
		//scanf("%s",&cmd);
        if(strcmp(cmd, "exit")==0) break;
        else if(strcmp(cmd, "help")==0)
        {
            printf("----------------\n内部命令：\n");
            printf("help  : 打印帮助信息\n");
            printf("exit  : 退出\n----------------\n外部命令：\n");
            printf("ls -l + 命令\n----------------\n");
            printf("cat + 命令\n----------------\n");
            printf("dir || more  + 命令\n----------------\n");
        }
        else if(strcmp(cmd, "ls")==0||strcmp(cmd,"cat")==0||strcmp(cmd,"cd")==0)
        {
            char ecmd[1024];
            scanf("%s", ecmd);
            pid_t pid = fork();
            if(pid>0) // parent
            {
                wait(NULL);
                continue;
            }
            if(pid==0)  // child
            {
                int s = execlp(cmd, cmd,ecmd, NULL);
                if(s==-1) printf("fail to exec %s\n", ecmd);
                return 0;
            }
        }//else if(strcmp(cmd,"dir || more")==0){
			else if(strcmp(cmd,"splitStr abc,d,e,fg || toUpperCase")==0){
			doPipe(cmd);
		}
        else
        {
            printf("无效命令\n");
        }
    }
    printf("bye\n");

    return 0;
}
void doPipe(char* cmd){
	char left[100];
    int k = 0, t = 0, m = 0;
    char right[100];
    char str[1024];
    int i = 0;
    for (i = 0; i < strlen(cmd); i++) {
        if (cmd[i] != ' ') {
            left[k++] = cmd[i];
        } else {
            break;
        }//fsdfs fds || fas
    }
    left[k] = '\0';
    for (i = k; i < strlen(cmd); i++) {
        if (cmd[i] != ' ' && cmd[i] != '|') {
            str[m++] = cmd[i];
        }
        if (cmd[i] == '|') {
            break;
        }
    }
    str[m] = '\0';
    for (i = m + k + 1; i < strlen(cmd); i++) {
        if (cmd[i] != ' ' && cmd[i] != '|') {
            right[t++] = cmd[i];
        }
    }
    right[t] = '\0';
    char *left1 = "/home/yajun/temp/";
    char dest[1024];
    char dest2[1024];
    strcpy(dest,left1);
    strcpy(dest2,left1);
	//printf("b1\n");
    strcpy(dest+ strlen(dest),left);
    strcpy(dest2+ strlen(dest2),right);
	//dest[strlen(dest)]='\0';
	//dest2[strlen(dest)]='\0';
	int fd[2];
	int res = pipe(fd);
	int status;
	if(res==-1){
		printf("pipe failed in do_command()\n");
	}
	pid_t pid1 = fork();
	if(pid1 == -1){
		printf("fork failed in do_command()\n");
	}else if(pid1 == 0){//child
		//printf("son1\n");
		dup2(fd[1],1);//stdout  write
		close(fd[0]);
		if(execl(dest,dest,str,NULL) < 0){
			printf("execlp failed!\n");
		}
	}else{
		wait(0);
		pid_t pid2 = fork();
		if(pid2 == -1){
			printf("fork failed in do_command()\n");		
		}
		else if(pid2 == 0){
				close(fd[1]);//close write edge
				dup2(fd[0],0);//dup the stdin
				if(execlp(dest2,dest2,NULL) < 0){
					printf("execlp failed!\n");
				
				}
		}//else if pid2 == 0
		else{
			close(fd[0]);
			close(fd[1]);
			wait(0);
		}
	}
}














