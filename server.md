##ONLY FOR STUDY write by myself
/* server_tcp.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define DEFAULT_PORT 5050


struct movielist{ 
               char ver[10];
               char  director[30];
               int year;
               char  name[30];
                 };   
struct movielist movlist[10];

int judge1(char s1)

{
   if((s1 >= 'a'&& s1 <= 'z' )||(s1 >= 'A' && s1 <= 'Z')||(s1 >= '0'&& s1<= '9'))
   return 1;
   else    return 0;
   }
int judge2(char s2)
{
    if((s2 >= 'a'&& s2 <= 'z')||(s2 >= 'A'&& s2 <= 'Z')||(s2 >= '0'&& s2 <='9')||s2 =='.'|| s2==' ')
     return 1;
    else   return 0;
 }
 
void get_list(char *str)
{
 char *i=str,*j,*k;
 int l=0,m=0,n=0,x=0,s=0,f=0;
  char a[30]={0};
  char b[30]={0};

 while(*i)
     {
  if(*i==':')
   {
     if(*(i-1)=='@') 
         { 
            j=i-3;
            k=i+2;
            l=0;
            m=0;
             while(judge1(*j)==1)
                   {
                    l++;
                    j--;
                   }
                for(n=0;n<l;n++)a[n]=*(j+n+1);   
                while(judge2(*(k))==1)
                     {
                      m++;
                      k++;
                      }
           if(m>30)m=30;
           for(f=0;f<m;f++){b[f]=*(k-m+f);} 
           if(0 == strcmp(a,"Name"))
              {
                 static int c=0;
                 strcpy(movlist[c].name,b);                 
                 c=c+1;
                }

           if(0 == strcmp(a,"Year"))
             {
                static int c_0=0;
                movlist[c_0].year=atoi(b);
                c_0=c_0+1;
              }
           if(0 ==strcmp(a,"Director"))
              {
                static int c_1=0;
               strcpy(movlist[c_1].director,b);
               c_1=c_1+1;  
                }
             }
          else
               {    
                j=i-1;
                l=0;
                n=0;
                k=i+2;
           while(judge2(*k)==1)
                {
                   n++;
                   k++;
                 }
            for(x=0;x<n;x++)
                  {
                   b[x]=*(k-n+x);}
            while(judge1(*j)==1)
                 {l++;j--;}
                 for(x=0;x<l;x++)a[x]=*(j+x+1);
   
               if(strcmp(a,"Version")==0)
                                 {
                                    for(s=0;s<10;s++){
                                    strcpy(movlist[s].ver,b);
                                    }
           
                 }
         }
   }
   i++;
}
 
      }
void *pro_thread(void*pPara)
{
    int sPort = DEFAULT_PORT;
    int sSocket = 0;
    int sLen = 0;
    int sSend = 0;
    int sRecv = 0;
    char send_buf[1024]={0};
    char recv_buf[1024] = {0};
    char str1[1024]={0};
    struct sockaddr_in ser;
    struct sockaddr_in cli;
    
     FILE* file5=fopen("movielist.txt","r");
          while(!feof(file5))
                {
                   fgets(str1,1024,file5);
                   get_list(str1);
                 }
            if(file5)fclose(file5);

    printf("Server waiting...\n");
   
    sSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if(sSocket < 0)
    {
          printf("socket() failure!\n");
        
    }
   
    ser.sin_family = AF_INET;
    ser.sin_port = htons(sPort);
    ser.sin_addr.s_addr = htonl(INADDR_ANY);
   
    if(bind(sSocket, (struct sockaddr*)&ser, sizeof(ser)) < 0)
    {
          printf("bind() failure!\n");
     
    }
   
    sLen = sizeof(cli);
    memset(recv_buf, 0, sizeof(recv_buf));
   
    while(1)
    {
        sRecv = recvfrom(sSocket, recv_buf, 1024, 0, (struct sockaddr*)&cli, (unsigned int*)&sLen);
        if((sRecv < 0)||(sRecv == 0))
        {
              printf("recvfrom() failure!\n");
            break;
        }
        else
        {   if(strcmp(movlist[0].ver,recv_buf)==0)
             {
              sSend = sendto(sSocket, movlist[0].ver, sizeof(movlist[0].ver), 0, (struct sockaddr*)&cli, sizeof(cli));            
              printf("recvfrom(): [%s]\n", recv_buf);
              printf("recvfrom() client IP: [%s]\n", inet_ntoa(cli.sin_addr));
              printf("recvfrom() client PORT: [%d]\n", ntohs(cli.sin_port));}
           if(strcmp(movlist[0].ver,recv_buf)!=0){
            sSend = sendto(sSocket, movlist[0].ver, sizeof(movlist[0].ver), 0, (struct sockaddr*)&cli, sizeof(cli)); 
            FILE* p1=fopen("movielist.txt","r");
                     if(p1 == NULL)
                     {
                      printf("can't open it!!");
                      return 0;
                      }
                    while(fgets(send_buf,1024,p1)!=NULL)
                          {
                          sSend = sendto(sSocket,send_buf, sizeof(send_buf), 0, (struct sockaddr*)&cli, sizeof(cli));
                          }
     
              if(sSend ==0||sSend <0 )
                      {
                            printf("send() failure!\n");
                             break;
                       }
              }
                   
        }
       
     
    }
    close(sSocket);
   
    return 0;
}
int main(int argc, char* argv[])
{   char str[1024]={0};
    int sPort = DEFAULT_PORT;
    int sListen = 0;
    int sAccept = 0;
    unsigned int sLen = 0;
    int sSend = 0;
    char cbuf[1024] = {0};
    char sbuf[1024] = {0};
    
    struct sockaddr_in ser;
    struct sockaddr_in cli;

    FILE* file=fopen("movielist.txt","r");
    if(!file)
  {
    printf("can't open movielist\n");
    exit(0);
  }
      while(!feof(file))
     {
         fgets(str,1024,file);
          get_list(str);
           }
     printf("%s\n",movlist[1].ver);      
  
     printf("Server waiting...\n");
   
    sListen = socket(AF_INET, SOCK_STREAM, 0);
   if(sListen < 0)
    {
        printf("socket() failure!\n");
        return -1;
    }
   
    ser.sin_family = AF_INET;
    ser.sin_port = htons(sPort);
    ser.sin_addr.s_addr = htonl(INADDR_ANY);
   
   if(bind(sListen, (struct sockaddr*)&ser, sizeof(ser)) < 0)
    {
        printf("bind() failure!\n");       
        return -1;
    }
   
    if(listen(sListen, 5) < 0)
    {
        printf("listen() failure!\n");        
        return -1;
    }
   
    sLen = sizeof(cli);

 pthread_t p_mon;
 pthread_create(&p_mon, NULL, pro_thread, NULL);  

    while(1)
    {
        sAccept = accept(sListen, (struct sockaddr*)&cli, (unsigned int*)&sLen);
        if(sAccept < 0)
        {
            printf("accept() failure!\n");             
            break;
        }
        printf("accept() client IP: [%s]\n", (char*)inet_ntoa(cli.sin_addr));
        printf("accept() client PORT: [%d]\n", ntohs(cli.sin_port));
       
           recv(sAccept, cbuf, sizeof(cbuf),0);  
       
           
           if(strcmp(cbuf,movlist[0].ver) !=0)
             {
               FILE* p=fopen("movielist.txt","r");
                     if(p == NULL)
                     {
                      printf("can't open it!!");
                      return 0;
                      }
                    while(fgets(sbuf,1024,p)!=NULL)
                          {
                          sSend = send(sAccept, sbuf, sizeof(sbuf), 0);
                          }
     
              if(sSend ==0||sSend <0 )
                      {
                            printf("send() failure!\n");
                             break;
                       }
                 
                 }
         if(strcmp(cbuf,movlist[0].ver) ==0) 
           sSend = send(sAccept,"your are already new", sizeof("your are already new"), 0);
   
              

         if((sSend < 0)||(sSend == 0))
         {
            printf("send() failure!\n");
            break;
         }
        else
        {
            printf("send() byte: [%d]\n", sSend);
        }
        close(sAccept);
    }
    close(sListen);

    if(file)fclose(file);
 
    return 0;
}
