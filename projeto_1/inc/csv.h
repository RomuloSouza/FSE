#ifndef CSV_H_
#define CSV_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void create_file();
void close_file();
void write_to_file(float ti, float te, float tr, int intensity);

#endif /* CSV_H_ */
