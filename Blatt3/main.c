#include <stdio.h>
#include <stdlib.h>

#include "database.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int speichern(int fd, DBRecord *dbr);
int match_filter(DBRecord *rec, const void *data);


int key_filter(DBRecord *rec, const void *data) {
    if (strstr(rec->key, data)) {
		return 1;
	}
    return 0;
}

int cat_filter(DBRecord *rec, const void *data) {
    if (strstr(rec->cat, data)) {
		return 1;
	}
    return 0;
}

int match_filter(DBRecord *rec, const void *data) {
	if (strlen(data) == 0) {
		return 2;
	}
	
	if (strstr(rec->key, data)) {
		return 1;
	}
	if (strstr(rec->cat, data)) {
		return 1;
	}
	return 0;
}

void create_testdata(char *filepath) {
	int fd;
	/* Schreibe Testdaten in Datei */
	DBRecord test = {"key1", "cat1", "value toll"};
	DBRecord test2 = {"key2", "Kategorie 2", "Values sind wirklich super!"};
	DBRecord test3= {"key3", "cat3", "value toll"};
	DBRecord test4 = {"key4", "cat4", "Values sind wirklich super!"};
	

	fd = open(filepath, O_WRONLY|O_TRUNC|O_CREAT, 0644);
	if (fd < 0) {
		perror("Bei Oeffnen der Ausgabedatei");
		exit(3);
	}
	speichern(fd, &test);
	speichern(fd, &test2);
	speichern(fd, &test3);
	speichern(fd, &test4);
	close(fd);
}

int test(char *filepath) {
	int search_result;
	DBRecord such_treffer = {"key4", "Kategoriename", "Toller Value"};
	DBRecord such_niete = {"Schlüssel", "Wursti", "Toller Value"};
	DBRecord replace_record = {"key_ersatz", "Kategorieersatz", "Neuer Wert"};
	const DBRecord update_record = {"key1", "cat1", "Dieser Value ist noch viel toller als der vorherige"};
	DBRecord index_result;
	int outfd;
	
	outfd = open(filepath, O_WRONLY|O_TRUNC|O_CREAT, 0644);
	
	db_get(filepath, 17, &index_result);
	
	search_result = db_search(filepath, 0, &such_treffer);
	search_result = db_search(filepath, 0, &such_niete);
	
	db_get(filepath, search_result, &index_result);

	db_list(filepath, outfd, match_filter,"key");
	

	db_put(filepath, -1, &replace_record);
	db_list(filepath, outfd, match_filter,"");
	
	db_update(filepath, &update_record);
	db_list(filepath, outfd, match_filter, "");
	
	db_del(filepath, 5);
	db_list(filepath, outfd, match_filter, "");
	return 1;
}

int main(int argc, char *argv[]) {
	char *filepath = "serversettings.cfg";
	int fd, search_result;
	char *std_aquarium = "Gruppenaquarium";
	int outfd = 1;
	DBRecord rec;
	
	
	fd = open(filepath, O_RDONLY|O_CREAT,0644);
	close(fd);

	
	if (argc == 2) {
		if (!strcmp(argv[1], "list")) {	db_list(filepath, outfd, match_filter,""); }
	}
	
	if (argc == 3) {
        /* Owner Filter */
		if (!strcmp(argv[1], "list")) {
            db_list(filepath, outfd, key_filter,argv[2]); 
        }
        
        /* Fischname Filter */
		if (!strcmp(argv[1], "clist")) {
            db_list(filepath, outfd, cat_filter,argv[2]); 
        }
        
        /* Suche: Besitzer oder Fischname */
		if (!strcmp(argv[1], "search")) {
			strcpy(rec.key,argv[2]);
			strcpy(rec.cat,argv[2]);
			db_list(filepath, outfd, match_filter, &rec);
		}
        
        if (!strcmp(argv[1], "delete")) {
			strcpy(rec.key,argv[2]);
            while ((search_result = db_get(filepath, 0, &rec))) {
                db_del(filepath, search_result);
            }
		}
	}
	
	if (argc == 4) {
		if (!strcmp(argv[1], "add")) {
			strcpy(rec.key,argv[2]);
			strcpy(rec.cat,argv[3]);
			strcpy(rec.value,std_aquarium);
			db_put(filepath, -1, &rec);
		}
        
        if (!strcmp(argv[1], "update")) {
			strcpy(rec.key,argv[2]);
			strcpy(rec.value,argv[3]);
			db_update(filepath, &rec);
		}
        
        if (!strcmp(argv[1], "delete")) {
			strcpy(rec.key,argv[2]);
			strcpy(rec.cat,argv[3]);
            
            /* Suche Fisch. Danach lösche ihn. */
            if ((search_result = db_get(filepath, 0, &rec))) {
                db_del(filepath, search_result);
            }
		}
        
	}
	
	if (argc == 5) {
		if (!strcmp(argv[1], "add")) {
			strcpy(rec.key,argv[2]);
			strcpy(rec.cat,argv[3]);
			strcpy(rec.value,argv[4]);
			db_put(filepath, -1, &rec);
		}
	}
	
	
	
	return 0;
}
