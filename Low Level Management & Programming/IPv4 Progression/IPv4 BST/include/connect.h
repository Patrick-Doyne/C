/**
 * Description: Header file containing prototypes for connect.c source file
 * and contains global MYSQL connection
 * Author: Patrick Doyne
 * Last Updated: 03/20/2025
 * G#: G01355212
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "menu.h"

extern MYSQL* conn;

int connect();