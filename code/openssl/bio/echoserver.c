#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>

void die()
{
    ERR_print_errors_fp(stderr);
    exit(1);
}

void echo(BIO *bio)
{
    int n;
    char buf[BUFSIZ];

    while ((n = BIO_read(bio, buf, sizeof(buf))) > 0) {
        if (BIO_write(bio, buf, n) < 0)
            die();
    }

    if (n < 0)
        die();
}

// ./echoserver
int main(int argc, char *argv[])
{
    BIO *sock, *conn;

    ERR_load_crypto_strings();

    if ((sock = BIO_new_accept("7")) == NULL)
        die();

    // Simplify restart of server during testing.
    BIO_set_bind_mode(sock, BIO_BIND_REUSEADDR);

    // Listen.
    if (BIO_do_accept(sock) <= 0) // TODO(dannas): Is ret === 0 an error?
        die();

    // Accept
    while (BIO_do_accept(sock) > 0) {

        conn = BIO_pop(sock);

        echo(conn);

        BIO_free(conn);
    }

    // TODO(dannas): Call die() when BIO_do_accept() fails

    return 0;
}
