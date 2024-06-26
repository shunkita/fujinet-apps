/**
 * nsh - the N: Shell
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license GPL v. 3 (see LICENSE.md)
 */

#ifndef NETWORK_H
#define NETWORK_H

unsigned char network_open(char *url, unsigned char mode, unsigned char translation);
unsigned char network_close(void);
unsigned char network_status(unsigned short *bw, unsigned char *c, unsigned char *nerr);
unsigned char network_read(unsigned char *b, unsigned short len);
unsigned char network_write(unsigned char *b, unsigned short len);

#endif /* NETWORK_H */
