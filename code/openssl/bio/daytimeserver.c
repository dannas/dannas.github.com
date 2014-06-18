#include <openssl/bio.h>
#include <stdio.h>
#include <openssl/err.h>
#include <time.h>

void die()
{
    ERR_print_errors_fp(stderr);
    exit(1);
}

// ./daytimeserver
int main(int argc, char *argv[])
{
    BIO *sock, *conn;
    time_t ticks;
    char buf[BUFSIZ];

    ERR_load_crypto_strings();

    if ((sock = BIO_new_accept("13")) == NULL)
        die();

    while (BIO_do_accept(sock)) {
        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));

        conn = BIO_pop(sock);
        if (BIO_puts(sock, buf) <= 0)
            die();
        BIO_free(conn);
    }
    return 0;
}
