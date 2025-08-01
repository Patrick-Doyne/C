/**
 * Description: Header file containing prototypes for add.c source file
 * and contains IPv4 address struct
 * Author: Patrick Doyne
 * Last Updated: 02/20/2025
 * G#: G01355212
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "struct_address_t.h"

node* findSpot(node* tmp, node* new);
int format(node *tmp,node *new);
int illegal(node*tmp, node *new);
int add();
int contains(node *tmp, node *node, int mode);
