#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#define PORT 50000						/*侦听端口地址*/
#define BACKLOG 10						/*侦听队列长度*/

class Pragma
{
public:
    int sockfd;
    std::string ip;
    int port;
public:
    Pragma(int _sockfd,std::string _ip,int _port)
    :sockfd(_sockfd)
    ,ip(_ip)
    ,port(_port)
    { 
    }
};
class TcpServer
{
private:
    int port;
    std::string ser_ip;
    int listen_sock;
public:
    TcpServer(std::string _ip,int _port)//std::string _ip,int _port
    :port(_port),
    ser_ip(_ip),
    listen_sock(-1)
    {

    }
    //初始化服务器
    void InitTcpServer()
    {
        listen_sock=socket(AF_INET,SOCK_STREAM,0);
        if(listen_sock<0)
        {
            std::cerr<<"socket failing"<<std::endl;
            exit(2);
        }
    
        struct sockaddr_in lock;
        lock.sin_family=AF_INET;
        lock.sin_port=htons(port);
        //lock.sin_addr.s_addr=INADDR_ANY;
        lock.sin_addr.s_addr=inet_addr(ser_ip.c_str());
        if(bind(listen_sock,(struct sockaddr*)&lock,sizeof(lock))<0)
        {
            std::cerr<<"bind failing"<<std::endl;
            exit(2);
        }

        if(listen(listen_sock,5)<0)
        {
            std::cerr<<"listen failing"<<std::endl;
            exit(3);
        }
        std::cout<<"listen success....."<<std::endl;
    }
    //主循环，接收客户端连接，并给连接的客户端生成一个子线程
    void Loop()
    {
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        for(;;)
        {
            int sockfd=accept(listen_sock,(struct sockaddr*)&peer,&len);
            if(sockfd<0)
            {                                                                                            
            continue;
            }
            std::string ip=inet_ntoa(peer.sin_addr);
            int port=ntohs(peer.sin_port);
            std::cout<<"get new link ["<<ip<<"] :"<<port<<std::endl;
            pthread_t id;
            Pragma* p=new Pragma(sockfd,ip,port);//创建一个套接字参数模块
            pthread_create(&id,NULL,Routine,p);//创建子线程
        }
    }
    //子线程创建实体
    static void* Routine(void* arg)
    {
        Pragma* p=(Pragma*)arg;
        //pthread_detach()即主线程与子线程分离，两者相互不干涉，子线程结束同时子线程的资源自动回收。
        pthread_detach(pthread_self());//将状态改为unjoinable状态，确保资源的释放。
        Server(p->sockfd,p->ip,p->port);
        close(p->sockfd);
        delete p;
    }   
    //服务器收发数据代码                                                                                             
    static void Server(int sockfd,std::string ip,int port)
    {                                                     
        char buffer[1024];
        std::string message ="server rev data ok!";
        while(true)       
        {          
            buffer[0]='0';
            int sz=read(sockfd,buffer,sizeof(buffer)-1);
            if(sz>0){
                buffer[sz]='\0';
                std::cout<<ip<<":"<<port<<"#"<<buffer<<std::endl;
                write(sockfd,message.c_str(),message.size());
            }
            else if(sz==0){
                std::cerr<<"client close"<<std::endl;
                break;
            }
            else{
                break;                                                                                       
            }
        }
        close(sockfd);
    }
    ~TcpServer()
    {
        if(listen_sock>0)
            close(listen_sock);
    }
};  
//编译指令：g++ linux_tcp_server.cpp  -o tcp_server -lpthread    
//启动服务器的方式：./tcp_server    
int main(int argv,char* argc[])    
{                                                                                               
  TcpServer* ts=new TcpServer("127.0.0.1",10000);//创建服务器    
  ts->InitTcpServer();//初始化服务器                                   
  ts->Loop();//启动服务器                                           
  return 0;                                                         
}
/**********************创建线程函数**************************/
// #include <pthread.h>

// int pthread_create(pthread_t *restrict tidp,
//                    const pthread_attr_t *restrict attr,
//                    void *(*start_rtn)(void), 
//                    void *restrict arg);
// 参数含义：
// 1）tidp参数表示指向线程标识符的指针。
// 2）attr参数用来设置线程属性，如果没有则填NULL。
// 3）start_rtn参数表示线程运行函数的地址。
// 4）arg参数表示线程运行函数的参数，如果没有参数则填NULL。

// 返回值：成功返回0，失败返回错误代码。
 

