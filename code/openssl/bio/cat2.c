#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/bio.h>

int main()
{
	BIO *in, *out;

	in = BIO_new_fd(STDIN_FILENO, BIO_NOCLOSE);
	out = BIO_new_fd(STDOUT_FILENO, BIO_NOCLOSE);

	while (!BIO_eof(in)) {
		char buf[BUFSIZ];

		BIO_gets(in, buf, sizeof(buf));
		BIO_puts(out, buf);
	}
	return 0;
}
