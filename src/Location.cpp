#include "Location.h"

#include "Config.h"

#include <cstdio>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <sys/stat.h>

static std::string g_app_dir;
static std::string g_app_name;

void bp_init_location(int argc, char **argv)
{
    char buf[PATH_MAX];

    const char *progname = argv[0];
    const char *slash = strrchr(progname, '/');

    if (!slash) {
        if (readlink("/proc/self/exe", buf, sizeof(buf)) != -1) {
            progname = buf;
            slash = strrchr(progname, '/');
        }
    }

    if (!slash) {
        std::cerr << "Could not determine application path" << std::endl;
        exit(EXIT_FAILURE);
    }

    g_app_name = slash + 1;
    g_app_dir = std::string(progname, slash - progname);
}

std::string bp_get_save_file_name()
{
    static const char *save_file_name = NULL;

    // Create save directory
    gchar *dirname = g_build_filename(g_get_user_data_dir(), g_app_name.c_str(), NULL);
    g_mkdir_with_parents(dirname, 0755);
    g_free(dirname);

    if (!save_file_name) {
        save_file_name = g_build_filename(g_get_user_data_dir(), g_app_name.c_str(), "save", NULL);
    }

    return save_file_name;
}

static bool
file_exists(const std::string &filename)
{
    struct stat st;
    memset(&st, 0, sizeof(st));

    if (stat(filename.c_str(), &st) != 0) {
        return false;
    }

    return S_ISREG(st.st_mode);
}

std::string
bp_get_data_file(const std::string &filename)
{
    std::vector<std::string> dirs;
    dirs.push_back(g_app_dir + "/../share/" + g_app_name + "/");
    dirs.push_back(g_app_dir + "/Content/");

    for (int i=0; i<dirs.size(); i++) {
        std::string fn = dirs[i] + filename;
        if (file_exists(fn)) {
            return fn;
        }
    }

    std::cerr << "File not found:" << filename << std::endl;
    exit(EXIT_FAILURE);
    return std::string();
}
