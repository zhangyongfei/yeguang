#include "SslServer.h"

SslServer::SslServer(){

}

SslServer::~SslServer(){

}

int SslServer::init(std::string custom, std::string commonName,
            std::string crtFile, 
            std::string keyFile,
            std::string caFile){
    
    int ret = 0;
    
    ssl_cache_init( &cache );
    x509_crt_init( &srvcert );
    pk_init( &pkey );

    entropy_init( &entropy );
    if( ( ret = ctr_drbg_init( &ctrDrbg, entropy_func, &entropy,
                               (const unsigned char *)custom.c_str(),
                               custom.length()) ) != 0 )
    {
        return -1;
    }

    /*
     * 1.1 Load the certificates and private RSA key
     */
    if( caFile.length() ){
        if( caFile == "none" ){
            ret = 0;
        }
        else{
            ret = x509_crt_parse_path( &cacert, caFile.c_str() );
        }
    } else{
        ret = x509_crt_parse( &cacert, (const unsigned char *) test_ca_list,
                              strlen( test_ca_list ) );
    }

    if(ret != 0){
        return -1;
    }

    /*
     * 1.2. Load own certificate and private key
     */
    if( crtFile.length() ){
        if( crtFile == "none" ){
            ret = 0;
        } else {
            ret = x509_crt_parse_file( &srvcert, crtFile.c_str() );
        }
    } else {
        ret = x509_crt_parse( &srvcert, (const unsigned char *) test_cli_crt,
                strlen( test_cli_crt ) );
    }

    if(ret != 0){
        return -1;
    }

    if( keyFile.length() ) {
        if( keyFile == "none" ){
            ret = 0;
        } else {
            ret = pk_parse_keyfile( &pkey, keyFile.c_str(), "" );
        }
    } else {
        ret = pk_parse_key( &pkey, (const unsigned char *) test_cli_key,
                strlen( test_cli_key ), NULL, 0 );
    }

    /*
     * 3. Setup stuff
     */
    if( ( ret = ssl_init( &ssl ) ) != 0 )
    {
        return -1;
    }

    ssl_set_endpoint( &ssl, SSL_IS_SERVER );
    ssl_set_authmode( &ssl, SSL_VERIFY_NONE );

    ssl_set_rng( &ssl, ctr_drbg_random, &ctrDrbg );
    ssl_set_dbg( &ssl, sslDebug, stdout );

    ssl_set_session_cache( &ssl, ssl_cache_get, &cache,
                                 ssl_cache_set, &cache );

    ssl_set_ca_chain( &ssl, srvcert.next, NULL, NULL );
    if( ( ret = ssl_set_own_cert( &ssl, &srvcert, &pkey ) ) != 0 )
    {
        return -1;
    }

    ssl_set_bio( &ssl, sslRecv, this, sslSend, this);

    ssl_session_reset( &ssl );

    return 0;
}

int SslServer::exit(){

    ssl_close_notify( &ssl );

    x509_crt_free( &cacert );
    ssl_free( &ssl );
    ctr_drbg_free( &ctrDrbg );
    entropy_free( &entropy );

    memset( &ssl, 0, sizeof( ssl ) );

    return 0;
}

int SslServer::handshake(){
    int ret;
    
    while( ( ret = ssl_handshake( &ssl ) ) != 0 )
    {
        if( ret != POLARSSL_ERR_NET_WANT_READ && ret != POLARSSL_ERR_NET_WANT_WRITE )
        {
        	ssl_session_reset( &ssl );
            return ret;
        }
    }

    return ret;
}

int SslServer::closeSession(){
    int ret;

	while( ( ret = ssl_close_notify( &ssl ) ) < 0 )
    {
        if( ret != POLARSSL_ERR_NET_WANT_READ &&
            ret != POLARSSL_ERR_NET_WANT_WRITE )
        {
            ssl_session_reset( &ssl );
            return -1;
        }
    }

    ssl_session_reset( &ssl );
    return 0;
}

int SslServer::write(const unsigned char *buf, size_t len){
    return ssl_write( &ssl, buf, len );
}

int SslServer::read(unsigned char *buf, size_t len){
    return ssl_read( &ssl, buf, len );
}

void SslServer::sslDebug( void *ctx, int level, const char *str ){

    if (ctx != NULL)
    {
        SslServer *pthis = (SslServer *)ctx;
        /* code */
        if(pthis->debugCB != NULL){
            pthis->debugCB(pthis->debugContext, level, str);
        }
    }
}

int SslServer::sslRecv( void *ctx, unsigned char *buf, size_t len ){
    static int first_try = 1;
    int ret;

    if( first_try )
    {
        first_try = 0;
        return( POLARSSL_ERR_NET_WANT_READ );
    }

    if (ctx != NULL)
    {
        /* code */
        SslServer *pthis = (SslServer *)ctx;

        if (pthis->recvCB != NULL)
        {
            /* code */
            ret = pthis->recvCB(pthis->recvContext, buf, len);
        } else {
            ret = len;
        }
    } else {
        ret = len;
    }

    if( ret != POLARSSL_ERR_NET_WANT_READ )
        first_try = 1; /* Next call will be a new operation */

    return( ret );
}

int SslServer::sslSend( void *ctx, const unsigned char *buf, size_t len ){
    static int first_try = 1;
    int ret;

    if( first_try )
    {
        first_try = 0;
        return( POLARSSL_ERR_NET_WANT_WRITE );
    }

    if (ctx != NULL)
    {
        /* code */
        SslServer *pthis = (SslServer *)ctx;

        if (pthis->sendCB != NULL)
        {
            /* code */
            ret = pthis->sendCB(pthis->sendContext, buf, len);
        } else {
            ret = len;
        }
    } else {
        ret = len;
    }

    if( ret != POLARSSL_ERR_NET_WANT_WRITE )
        first_try = 1; /* Next call will be a new operation */

    return( ret );
}

void SslServer::setRecvCB(LPSslSRecvCB recvCB, void* context){
    this->recvCB = recvCB;
    this->recvContext = context;
}

void SslServer::setSendCB(LPSslSSendCB recvCB, void* context){
    this->sendCB = sendCB;
    this->sendContext = context;
}

void SslServer::setDebugCB(LPSslSDebugCB debugCB, void *context){
    this->debugCB = debugCB;
    this->debugContext = context;
}