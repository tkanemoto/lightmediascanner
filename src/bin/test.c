#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <lightmediascanner.h>
#include <stdio.h>
#include <stdlib.h>

void
usage(const char *prgname)
{
    fprintf(stderr,
            "Usage:\n"
            "\t%s <slave-timeout> <db-path> <parser> <scan-path>\n"
            "\n",
            prgname);
}

int
main(int argc, char *argv[])
{
    char *db_path, *parser_name, *scan_path;
    lms_t *lms;
    lms_plugin_t *parser;
    int slave_timeout;

    if (argc < 5) {
        usage(argv[0]);
        return 1;
    }

    slave_timeout = atoi(argv[1]);
    db_path = argv[2];
    parser_name = argv[3];
    scan_path = argv[4];

    lms = lms_new(db_path);
    if (!lms) {
        fprintf(stderr,
                "ERROR: could not create light media scanner for DB \"%s\".\n",
                db_path);
        return -1;
    }

    lms_set_slave_timeout(lms, slave_timeout);

    parser = lms_parser_find_and_add(lms, parser_name);
    if (!parser) {
        fprintf(stderr, "ERROR: could not create parser \"%s\".\n",
                parser_name);
        lms_free(lms);
        return -2;
    }

    if (lms_process(lms, scan_path) != 0) {
        fprintf(stderr, "ERROR: processing \"%s\".\n", scan_path);
        lms_free(lms);
        return -3;
    }

    if (lms_free(lms) != 0) {
        fprintf(stderr, "ERROR: could not close light media scanner.\n");
        return -4;
    }

    return 0;
}
