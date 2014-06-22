#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/select.h>
#include <openssl/bio.h>
#include <openssl/err.h>

void die(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    fprintf(stderr, "\n");

	ERR_print_errors_fp(stderr);
	exit(1);
}

// Non-blocking daytimeclient
int main(int argc, char *argv[])
{
    BIO *sock, *out;
    int n;
    int ret;
    int fd;
    fd_set rset, wset;
    char buf[BUFSIZ];

	ERR_load_crypto_strings();

    if ((sock = BIO_new(BIO_s_connect())) == NULL)
        die("BIO_new");

    // Need to be set before the connect call is executed.
    BIO_set_nbio(sock, 1);

    BIO_set_conn_hostname(sock, "localhost:13");

    // TODO(dannas): What is the purpose of BIO_should_io_special()? Why not
    // just use BIO_should_retry()?
    if  ((ret = BIO_do_connect(sock)) != 1) {
        if (BIO_should_io_special(sock))
            ;
        else
            die("BIO_do_connect");
    }

    BIO_get_fd(sock, &fd);

    FD_ZERO(&rset);
    FD_SET(fd, &rset);
    wset = rset;

    // TODO(dannas): Why check rset?
    if ((n = select(fd+1, &rset, &wset, NULL, NULL)) == -1) {
        die("select, %s", strerror(errno)); // TODO(dannas): check errno
    }

    if (FD_ISSET(fd, &rset) || FD_ISSET(fd, &wset)) {
        ret = BIO_do_connect(sock);
        if (ret < 0)
            die("BIO_do_connect");
    } else {
        die("select returned, but no fd readable/writable");
    }

    if ((out = BIO_new_fp(stdout, BIO_NOCLOSE)) == NULL)
        die("BIO_new_fp");

    do {
        n = BIO_read(sock, buf, sizeof(buf));
    } while (BIO_should_retry(sock));

    if (n < 0)
        die("BIO_read");

    buf[n] = '\0';

    if (BIO_puts(out, buf) < 0)
        die("BIO_puts");

    return 0;
}
