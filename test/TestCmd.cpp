#include "CmdParse.h"
#include "Daemon.h"
#include "ConfigParse.h"
#include "ShareMemoryManager.h"
#include "Logger4.h"
#include "Log.h"
//#include "polarssl/net.h"
//#include "SslClient.h"
//#include "SslServer.h"
#include <stdio.h>
#include <unistd.h>

class MyCmdParse : public yeguang::CmdParse,
        public yeguang::Daemon{
public:
	MyCmdParse(){
		yeguang::CmdParse::TCmdList cmd_list;

        start = false;
        stop = false;

        cmd_list.push_back(TCommand('s', false, "???ˉ??3ì"));
        cmd_list.push_back(TCommand('d', false, "′ò?aμ÷ê??￡ê?"));
        cmd_list.push_back(TCommand('k', true,  "?ò??3ì·￠?íD?o?"));
        cmd_list.push_back(TCommand('c', true,  "???????t?·??"));
        cmd_list.push_back(TCommand('u', false, "í￡?1??3ì"));

        SetCmdList(cmd_list);

        SetChildCnt(1);
	}

	~MyCmdParse(){

	}

protected:
	int OnCmdFlagParser(char flag, std::string optarg){
        switch (flag)
        {
		case 's':
            start = true;
			break;
		case 'd':
			break;
		case 'k':
			break;
        case 'c':
            break;
		case 'u':
            stop = true;
			break;
		default:
			Usage();
			break;
        }

		return 0;
	}

    int OnParserEnd(){
        if(start && stop){
            Usage();
            return-1;
        }

        if(start){
            StartDaemon();
        } else if(stop){
            Stop();

            yeguang::ShareMemoryManager test;

            test.Init(0x1234, 0x1235, 100, 3);
            sleep(2);
            test.Exit();
        }
    }

	int Worker(){
        yeguang::Log::CreateLog(new yeguang::Logger4("./log4cplus.properties"));

        yeguang::ShareMemoryManager manager;

        manager.Init(0x1234, 0x1235, 100, 3);

        char buffer[255] = {0};

        while(1){
            int len = manager.FromMaster(buffer, sizeof(buffer));
            LogDebug("-----FromMaster: " << buffer);
            //printf("-----FromMaster:%s, len:%d\n", buffer, len);
            sleep(1);
            manager.ToMaster("Child Write", 11);
            sleep(1);
        }

        manager.Exit();
		return 0;
	}

    int Run(){
        yeguang::Log::CreateLog(new yeguang::Logger4("./log4cplus.properties"));

        yeguang::ShareMemoryManager manager;

        manager.Init(0x1234, 0x1235, 100, 3);

        char buffer[255] = {0};

        while(1){
            manager.ToChild("Master Write", 12);
            sleep(1);  
            int len = manager.FromChild(buffer, sizeof(buffer));            
            LogDebug("-------FromChild: " << buffer);
            //printf("-------FromChild:%s\n", buffer);          
            sleep(1);
        }

        manager.Exit();
        return 0;
    }

private:
    bool start;
    bool stop;
};

#define GET_REQUEST "GET / HTTP/1.0\r\n\r\n"
/*
int sslClient(){
    int ret = 0, server_fd = -1;
    unsigned char buf[8192];

    if( ( ret = net_connect( &server_fd, "192.168.2.100",
                                         4433 ) ) != 0 )
    {
        printf( " failed\n  ! net_connect returned %d\n\n", ret );
        return -1;
    }

    yeguang::SslClient client;

    client.init("test", "localhost");

    client.setRecvCB(net_recv, (void*)&server_fd);
    client.setSendCB(net_send, (void*)&server_fd);

    ret = client.handshake();

    if (ret != 0)
    {
        printf( " failed\n  ! handshake returned %d\n\n", ret );
        return -1;
    }

    ret = client.getVerifyResult();

    if (ret != 0)
    {
         printf( " failed\n  ! handshake returned %d\n\n", ret );
        return -1;
    }

    int len = sprintf( (char *) buf, GET_REQUEST );

    while( ( ret = client.write( buf, len ) ) <= 0 )
    {
        if( ret != POLARSSL_ERR_NET_WANT_READ && ret != POLARSSL_ERR_NET_WANT_WRITE )
        {
            printf( " failed\n  ! ssl_write returned %d\n\n", ret );
            return -1;
        }
    }

    len = ret;

     do
    {
        len = sizeof( buf ) - 1;
        memset( buf, 0, sizeof( buf ) );
        ret = client.read( buf, len );

        if( ret == POLARSSL_ERR_NET_WANT_READ || ret == POLARSSL_ERR_NET_WANT_WRITE )
            continue;

        if( ret == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY )
            break;

        if( ret < 0 )
        {
            printf( "failed\n  ! ssl_read returned %d\n\n", ret );
            break;
        }

        if( ret == 0 )
        {
            printf( "\n\nEOF\n\n" );
            break;
        }

        len = ret;
        printf( " %d bytes read\n\n%s\n\n", len, (char *) buf );
    }while( 1 );
}

#define HTTP_RESPONSE \
    "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" \
    "<h2>PolarSSL Test Server</h2>\r\n" \
    "<p>Successful connection using: %s</p>\r\n"

int sslServer(){
    int ret = 0, listen_fd = 0;

    if( ( ret = net_bind( &listen_fd, NULL, 5544 ) ) != 0 )
    {
        printf( " failed\n  ! net_bind returned %d\n\n", ret );
        return -1;
    }

    SslServer server;

    server.init("test", "localhost");

    while(1){
        int client_fd = -1, len = 0;
        unsigned char buf[4096] = {0};

        if( ( ret = net_accept( listen_fd, &client_fd, NULL ) ) != 0 )
        {
            break;
        }

        printf("收到客服端连接\n");

        server.setRecvCB(net_recv, (void*)&client_fd);
        server.setSendCB(net_send, (void*)&client_fd);

        if ((ret = server.handshake()) != 0)
        {
            printf( " failed\n  ! handshake returned 0x%X\n\n", ret );
            continue;
        }

        printf("握手成功\n");

        do
        {
            len = sizeof( buf ) - 1;
            memset( buf, 0, sizeof( buf ) );
            ret = server.read( &buf[0], len );

            if( ret == POLARSSL_ERR_NET_WANT_READ || ret == POLARSSL_ERR_NET_WANT_WRITE )
                continue;

            if( ret <= 0 )
            {
                switch( ret )
                {
                    case POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY:
                        printf( " connection was closed gracefully\n" );
                        break;

                    case POLARSSL_ERR_NET_CONN_RESET:
                        printf( " connection was reset by peer\n" );
                        break;

                    default:
                        printf( " ssl_read returned -0x%x\n", -ret );
                        break;
                }

                break;
            }

            len = ret;
            printf( " %d bytes read\n\n%s", len, (char *) buf );

            if( ret > 0 )
                break;
        }while( 1 );

        len = sprintf( (char *) buf, HTTP_RESPONSE,
                   "Successful" );

        while( ( ret = server.write( buf, len ) ) <= 0 )
        {
            if( ret == POLARSSL_ERR_NET_CONN_RESET )
            {
                printf( " failed\n  ! peer closed the connection\n\n" );
                continue;
            }

            if( ret != POLARSSL_ERR_NET_WANT_READ && ret != POLARSSL_ERR_NET_WANT_WRITE )
            {
                printf( " failed\n  ! ssl_write returned %d\n\n", ret );
                return -1;
            }
        }

        len = ret;
        printf( " %d bytes written\n\n%s\n", len, (char *) buf );

        printf( "  . Closing the connection..." );

        server.closeSession();

        printf( " ok\n" );

        if(client_fd != -1){
            net_close( client_fd );
        }

        ret = 0;
    }

}*/

int main(int argc, char *argv[]){

    //sslServer();

    //yeguang::Log::CreateLog(new yeguang::Logger4("./log4cplus.properties"));
    //LogDebug("-------FromChild");

    //MyCmdParse cmdparse;

    //cmdparse.Parse(argc, argv);

    //yeguang::ConfigParse config;

    //config.Parse("./test.conf");

    //std::string value = "";
    //std::string line_num = "";

    //config.GetCofing("pid", value, line_num);

    //ConfgAssert(value != "", "pid isn't exist", line_num);

    //printf("%s\n", value.c_str());

	return 0;
}
