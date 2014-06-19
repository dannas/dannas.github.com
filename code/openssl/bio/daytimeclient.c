#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/err.h>

void die()
{
	ERR_print_errors_fp(stderr);
	exit(1);
}

// ./daytimeclient example.com:13
int main(int argc, char *argv[])
{
    BIO *sock, *out;
    int n;
    char buf[BUFSIZ];

	ERR_load_crypto_strings();

    if ((sock = BIO_new_connect("localhost:13")) == NULL)
        die();

    if ((out = BIO_new_fp(stdout, BIO_NOCLOSE)) == NULL)
        die();

    if ((n = BIO_read(sock, buf, sizeof(buf))) <= 0)
        die();

    buf[n] = '\0';

    if (BIO_puts(out, buf) <= 0)
        die();

    return 0;
}
