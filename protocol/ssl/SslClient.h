#ifndef SSL_CLIENT
#define SSL_CLIENT
#include <string>
#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/ssl.h"
#include "polarssl/certs.h"


namespace yeguang{
typedef void (*LPSslDebugCB)(void* lpvoid, int level, std::string content);
typedef int (*LPSslRecvCB)(void* lpvoid, unsigned char *buf, size_t len);
typedef int (*LPSslSendCB)(void* lpvoid, const unsigned char *buf, size_t len);

class SslClient
{
public:
    SslClient();
    ~SslClient();

    int init(std::string custom, std::string commonName,
            std::string crtFile = "none", 
            std::string keyFile = "none",
            std::string caFile  = "");

    int exit();
    
    void setRecvCB(LPSslRecvCB recvCB, void* context);

    void setSendCB(LPSslSendCB recvCB, void* context);

    void setDebugCB(LPSslDebugCB debugCB, void *context);

    int handshake();

    int getVerifyResult();

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
    x509_crt clicert;
    pk_context pkey;

    LPSslDebugCB debugCB;
    void*        debugContext;

    LPSslRecvCB  recvCB;
    void*        recvContext;

    LPSslSendCB  sendCB;
    void*        sendContext;
};
}

#endif
