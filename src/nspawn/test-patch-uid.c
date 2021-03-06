/* SPDX-License-Identifier: LGPL-2.1+ */
/***
  This file is part of systemd.

  Copyright 2016 Lennart Poettering
***/

#include <stdlib.h>

#include "log.h"
#include "nspawn-patch-uid.h"
#include "user-util.h"
#include "util.h"

int main(int argc, char *argv[]) {
        uid_t shift, range;
        int r;

        log_set_max_level(LOG_DEBUG);
        log_parse_environment();
        log_open();

        if (argc != 4) {
                log_error("Expected PATH SHIFT RANGE parameters.");
                return EXIT_FAILURE;
        }

        r = parse_uid(argv[2], &shift);
        if (r < 0) {
                log_error_errno(r, "Failed to parse UID shift %s.", argv[2]);
                return EXIT_FAILURE;
        }

        r = parse_gid(argv[3], &range);
        if (r < 0) {
                log_error_errno(r, "Failed to parse UID range %s.", argv[3]);
                return EXIT_FAILURE;
        }

        r = path_patch_uid(argv[1], shift, range);
        if (r < 0) {
                log_error_errno(r, "Failed to patch directory tree: %m");
                return EXIT_FAILURE;
        }

        log_info("Changed: %s", yes_no(r));

        return EXIT_SUCCESS;
}
