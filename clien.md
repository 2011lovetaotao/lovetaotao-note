##ONLY FOR STUDY write by myself
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
struct movielist movlist[5];

/****************************************************************
* analysis   
*****************************************************************/
int judge1(char s1) 
{
      if((s1 >='a'&& s1 <='z')||(s1 >='A'&& s1 <='Z')||(s1 >= '0'&& s1 <= '9'))
         return 1;
      else    
         return 0;
}

int judge2(char s2)
   {
     if((s2 >= 'a'&& s2 <= 'z')||(s2 >= 'A' && s2 <='Z')||(s2 >='0'&&s2<='9')||s2 =='.'||s2==' ')
       return 1;
     else 
       return 0;
 }
 
void get_list(char *str)
{
  char *i=str,*j,*k;
  int l= 0,m= 0,n= 0,x= 0,s= 0,f= 0;
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
                 for(f=0;f<m;f++){b[f]=*(k-m+f);
                    } 
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
                 {
                 l++;j--;
                 }
                 for(x=0;x<l;x++)a[x]=*(j+x+1);
   
                 if(strcmp(a,"Version")==0)
                      {
                 for(s=0;s<10;s++)
                     {
                      strcpy(movlist[s].ver,b);
                     }
           
                 }
         }
   }
   i++;
        }
}
/***************************************** 
 selections
 *******************************************/
  int work(char *order )
       {   char str[1024]={0};
           int y=0;
           int z=0;
           int i=1;
           int x=0;

      FILE* file1=fopen("movielist.txt","r");
            if(!file1){printf("can't open movielist\n");
                       }
      while(!feof(file1)){
           fgets(str,1024,file1);
           get_list(str);
           }
        if(file1)fclose(file1);
        if(strcmp(order,"Name")==0){for(x =0;x<5;x++)
            printf("%s\n",movlist[x].name);}
     
        else if(strcmp(order,"director")==0)
            {
                 for(x =0;x<5;x++)
                {
                  y++;
                          
                 if(x==0) printf("%s\n",movlist[x].director);
                               
                 else { for(z =0;z<(y-1);z++)
                          {
                         if(strcmp(movlist[x].director,movlist[z].director) ==0 )
                            {
                              i=0;
                             }  
                            }  
                       if(i==1)printf("%s\n",movlist[x].director);
                       }
                     
                   }
              }
          else 
              {
                 for(x=0;x<5;x++)
                    {
                     if(strcmp(movlist[x].director,order)==0)
                        printf("%s\n",movlist[x].name);
                     if(movlist[x].year==atoi(order))
                        printf("%s\n",movlist[x].name);}
                  }
     
         return 0;
       }
/****************************
   download
 **************************** */
int download(char *ip1)
  { 
    int cPort = DEFAULT_PORT;
    int cClient = 0;
    int cLen = 0;
    struct sockaddr_in cli;
    char cbuf[1024] = {0};
    FILE *fil;
   
    memset(cbuf, 0, sizeof(cbuf));
   
    cli.sin_family = AF_INET;
    cli.sin_port = htons(cPort);
    cli.sin_addr.s_addr = inet_addr(ip1);   
    cClient = socket(AF_INET, SOCK_STREAM, 0);

    if(cClient < 0)
    {
        printf("socket() failure!\n");
        return -1;
    }
   
    if(connect(cClient, (struct sockaddr*)&cli, sizeof(cli)) < 0)
    {
        printf("connect() failure!\n");
        return -1;
    }


      send(cClient, "Name", sizeof("Name"), 0);
      fil= fopen("movielist.txt","w");

   if(!fil){
              printf("can't open movielist\n");
            }

while(1)
    {
    cLen = recv(cClient, cbuf, sizeof(cbuf),0);  

    if((cLen < 0)||(cLen == 0))
      {  printf("renew OK\n");
        return -1;
       }
           fputs(cbuf,fil);
           fflush(fil);
    }
  
     if(fil)fclose(fil);  
    close(cClient);
   
   
   return 0;
 }
/**********************************
      ***udp***
 ***************************** */
int udp(char *ip2)
{
    int cPort = DEFAULT_PORT;
    int cClient = 0;
    unsigned int cLen = 0;
    int cSend = 0;
    int cRecv = 0;
    char str1[1024]={0};  
    char recv_buf[1024] = {0};

    struct sockaddr_in cli;
       FILE* file5=fopen("movielist.txt","r");
       if(!file5) file5=fopen("movielist.txt","w");
          while(!feof(file5))
                       {
                        fgets(str1,1024,file5);
                        get_list(str1);
                        }
            if(file5)fclose(file5);
    
    memset(recv_buf, 0, sizeof(recv_buf));
   
    cli.sin_family = AF_INET;
    cli.sin_port = htons(cPort);
    cli.sin_addr.s_addr = inet_addr(ip2);
   
    cClient = socket(AF_INET, SOCK_DGRAM, 0);
    if(cClient < 0)
    {
        printf("socket() failure!\n");
        return -1;
    }
   
    cLen = sizeof(cli);  
 
    printf("the edition is :%s\n",movlist[0].ver);
    cSend = sendto(cClient, movlist[0].ver, sizeof(movlist[0].ver), 0, (struct sockaddr*)&cli, cLen);  
    cRecv = recvfrom(cClient, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&cli, (unsigned int*)&cLen);

    if(strcmp(recv_buf,movlist[0].ver)!=0)
    {
          download(ip2);
    }
    else printf("need't renew\n");
     
    close(cClient);  
   return 0;
}

void empty_qinkong(){
     char ch;
     ch=getchar();
     while(ch!=EOF&&ch!='\n'){
            ch= getchar();
            }
     }
int main(int argc, char* argv[])
{
    int i=0;
    char ip[20]={0};
    char year[5]={0};
    char name[20]={0};

    printf("please input the id: ");
    scanf("%s",ip);
    empty_qinkong();
 
while(1){
         printf("\n");
         printf("************************\n"); 
    printf("please input you need function number\n1 renew movielist \n2 download movielist\n3 list all movie \n4 list movie by year\n5 list directories name\n6 chose movie by director name\n7 other key quit\n");

    scanf("%d",&i);
    empty_qinkong();

    switch(i){
             case 1:udp(ip);break;
            
             case 2:download(ip);break;
               
             case 3:work("Name");break;      
               
             case 4:
                      printf("please input a year\n");
                      scanf("%s",year);
                      empty_qinkong();
                      work(year)  ;break;

             case 5:work("director");break;

             case 6:
                     printf("please input a director's name\n");
                     scanf("%s",name);
                     empty_qinkong();
                     work(name);break;
              default : return -1;

              }
          
        } 
return 0;
}
