#ifndef SSL_SERVER
#define SSL_SERVER
#include <string>
#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/ssl.h"
#include "polarssl/certs.h"
#include "polarssl/ssl_cache.h"

typedef void (*LPSslSDebugCB)(void* lpvoid, int level, std::string content);
typedef int (*LPSslSRecvCB)(void* lpvoid, unsigned char *buf, size_t len);
typedef int (*LPSslSSendCB)(void* lpvoid, const unsigned char *buf, size_t len);

class SslServer{
public:
	SslServer();
	~SslServer();

	int init(std::string custom, std::string commonName,
            std::string crtFile = "", 
            std::string keyFile = "",
            std::string caFile  = "");

	int exit();

	int handshake();

	int closeSession();

	void setRecvCB(LPSslSRecvCB recvCB, void* context);

    void setSendCB(LPSslSSendCB recvCB, void* context);

    void setDebugCB(LPSslSDebugCB debugCB, void *context);
    
    int write(const unsigned char *buf, size_t len);

    int read(unsigned char *buf, size_t len);

protected:
    static void sslDebug( void *ctx, int level, const char *str );

    static int sslRecv( void *ctx, unsigned char *buf, size_t len );

    static int sslSend( void *ctx, const unsigned char *buf, size_t len );

private:
    entropy_context entropy;
    ctr_drbg_context ctrDrbg;
    x509_crt cacert;
    ssl_context ssl;
    x509_crt srvcert;
    pk_context pkey;
    ssl_cache_context cache;

    LPSslSDebugCB debugCB;
    void*        debugContext;

    LPSslSRecvCB  recvCB;
    void*        recvContext;

    LPSslSSendCB  sendCB;
    void*        sendContext;	
};

#endif