#include "location.h"

#include <glib.h>
#include <stdio.h>

extern gchar *appname;

const char *bp_get_save_file_name()
{
    static const char *save_file_name = NULL;

    // Create save directory
    gchar *dirname = g_build_filename(g_get_user_data_dir(), appname, NULL);
    g_mkdir_with_parents(dirname, 0755);
    g_free(dirname);

    if (!save_file_name) {
        save_file_name = g_build_filename(g_get_user_data_dir(), appname, "save", NULL);
    }

    return save_file_name;
}
