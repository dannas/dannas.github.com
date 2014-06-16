#include <stdio.h>
#include <stdlib.h>
#include <openssl/bio.h>

// cat: Concatenate standard input to standard output
int main()
{
	BIO *in, *out;

	in = BIO_new_fp(stdin, BIO_NOCLOSE);
	out = BIO_new_fp(stdout, BIO_NOCLOSE);

	while (!BIO_eof(in)) {
		char buf[BUFSIZ];

		BIO_gets(in, buf, sizeof(buf));
		BIO_puts(out, buf);
	}
	
	BIO_free(out);
	BIO_free(in);

	return 0;
}
