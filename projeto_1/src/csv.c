#include <csv.h>

FILE *pfile=NULL;

/*
Creates a new csv file with the correct header. If the file exits, its data will be erased.
*/
void create_file(){
    char FILENAME[20] = "log.csv";

    pfile = fopen(FILENAME, "w+");
    fprintf(pfile, "Data/hora, TI, TE, TREF, Intensidade\n");
}

void close_file(){
    fclose(pfile);
}

/*
Writes a new line into file
*/
void write_to_file(float ti, float te, float tr, double intensity){
    time_t rawtime;
    struct tm *info;
    char datetime[80];

    time(&rawtime);

    info = localtime(&rawtime);

    strftime(datetime, 80, "%d-%m-%Y %H:%M:%S", info);

    fprintf(pfile, "%s, %f, %f, %f, %lf\n", datetime, ti, te, tr, intensity);

}
