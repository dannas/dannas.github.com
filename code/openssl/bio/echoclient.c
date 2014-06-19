#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void die()
{
	ERR_print_errors_fp(stderr);
	exit(1);
}

void echo(BIO *sock, BIO *in, BIO *out)
{
    int n;
    char buf[BUFSIZ];
    while (BIO_gets(in, buf, sizeof(buf))) {
        if ((n = BIO_write(sock, buf, strlen(buf))) < 0)
            die();
        if ((n = BIO_read(sock, buf, sizeof(buf))) == 0) {
            exit(1);
        }
        if (n < 0)
            die();

        buf[n] = '\0';
        if (BIO_puts(out, buf) < 0)
            die();
    }
}

// ./echoclient
int main(int argc, char *argv[])
{
    BIO *sock, *in, *out;

	ERR_load_crypto_strings();

    if ((sock = BIO_new_connect("localhost:7")) == NULL)
        die();

    if ((in = BIO_new_fp(stdin, BIO_NOCLOSE)) == NULL)
        die();

    if ((out = BIO_new_fp(stdout, BIO_NOCLOSE)) == NULL)
        die();

    echo(sock, in, out);

    return 0;
}
