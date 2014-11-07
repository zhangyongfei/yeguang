#include "SslClient.h"

SslClient::SslClient(){
    debugCB = NULL;
    debugContext = NULL;

    sendCB = NULL;
    sendContext = NULL;

    recvCB = NULL;
    recvContext = NULL;
}

SslClient::~SslClient(){

}

int SslClient::init(std::string custom, std::string commonName, 
	                std::string crtFile, 
	                std::string keyFile,
	                std::string caFile){

    int ret = 0;

    /*
     * 0. Initialize the RNG and the session data
     */

    memset( &ssl, 0, sizeof( ssl_context ) );
    x509_crt_init( &cacert );
    x509_crt_init( &clicert );
    pk_init( &pkey );

    entropy_init( &entropy );
    if( ( ret = ctr_drbg_init( &ctrDrbg, entropy_func, &entropy,
                               (const unsigned char *)custom.c_str(),
                               custom.length() ) ) != 0 ) {
        return -1;
    }

    /*
     * 1.1. Load the trusted CA
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
     *
     * (can be skipped if client authentication is not required)
     */
    if( crtFile.length() ){
    	if( crtFile == "none" ){
            ret = 0;
    	} else {
            ret = x509_crt_parse_file( &clicert, crtFile.c_str() );
    	}
    } else {
        ret = x509_crt_parse( &clicert, (const unsigned char *) test_cli_crt,
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

    if(ret != 0){
    	return -1;
    }

	/*
     * 2. Setup stuff
     */        
    if( ( ret = ssl_init( &ssl ) ) != 0 )
    {
        return -1;
    }

    ssl_set_endpoint( &ssl, SSL_IS_CLIENT );
    /* OPTIONAL is not optimal for security,
     * but makes interop easier in this simplified example */
    ssl_set_authmode( &ssl, SSL_VERIFY_OPTIONAL );
    ssl_set_ca_chain( &ssl, &cacert, NULL, commonName.c_str() );

    ssl_set_rng( &ssl, ctr_drbg_random, &ctrDrbg );
    ssl_set_dbg( &ssl, sslDebug, this );
    ssl_set_bio( &ssl, sslRecv, this, sslSend, this);


    return 0;
}

int SslClient::exit(){
    return 0;
}

int SslClient::handshake(){
	int ret;
	
	while( ( ret = ssl_handshake( &ssl ) ) != 0 )
    {
        if( ret != POLARSSL_ERR_NET_WANT_READ && ret != POLARSSL_ERR_NET_WANT_WRITE )
        {
            return ret;
        }
    }

    return ssl_get_verify_result( &ssl );
}

int SslClient::write(const unsigned char *buf, size_t len){
	return ssl_write( &ssl, buf, len );
}

int SslClient::read(unsigned char *buf, size_t len){
	return ssl_read( &ssl, buf, len );
}

void SslClient::sslDebug( void *ctx, int level, const char *str ){

    if (ctx != NULL)
    {
    	SslClient *pthis = (SslClient *)ctx;
    	/* code */
    	if(pthis->debugCB != NULL){
    		pthis->debugCB(pthis->debugContext, level, str);
    	}
    }
}

int SslClient::sslRecv( void *ctx, unsigned char *buf, size_t len ){
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
    	SslClient *pthis = (SslClient *)ctx;

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

int SslClient::sslSend( void *ctx, const unsigned char *buf, size_t len ){
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
    	SslClient *pthis = (SslClient *)ctx;

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

void SslClient::setDebugCB(LPSslDebugCB debugCB, void* context) {
    this->debugCB = debugCB;
    this->debugContext = context;
}

void SslClient::setRecvCB(LPSslRecvCB recvCB, void* context) {
	this->recvCB = recvCB;
    this->recvContext = context;
}

void SslClient::setSendCB(LPSslSendCB sendCB, void* context) {
	this->sendCB = sendCB;
	this->sendContext = context;
}