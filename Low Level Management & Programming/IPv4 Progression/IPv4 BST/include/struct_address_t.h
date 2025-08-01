/**
 * Description: Header file contains IPv4 address struct and one global variable
 * Author: Patrick Doyne
 * Last Updated: 02/20/2025
 * G#: G01355212
 */

# ifndef IPv4_H
# define IPv4_H

typedef struct address_t{

	int octet[4];
	char alias[100];
	struct address_t *left;
	struct address_t *right;

}node;

extern node *head;

#endif
