#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "./nob.h"

#define BUILD_FOLDER  "build/"
#define RAYLIB_FOLDER "./raylib-5.5_linux_amd64/"
#define MUJS_FOLDER   "./mujs-1.3.7/"
#define LUST2D_EXE    BUILD_FOLDER"lust2d"

Cmd cmd = {0};

bool run = false;

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program_name = shift(argv, argc);

    while (argc > 0) {
        const char *flag = shift(argv, argc);
        if (strcmp(flag, "-run")   == 0) { run   = true; continue; }
        if (strcmp(flag, "--")     == 0) break;
        fprintf(stderr, "%s:%d: ERROR: unknown flag `%s`\n", __FILE__, __LINE__, flag);
        if (run) {
            fprintf(stderr, "NOTE: use -- to separate %s and %s command line arguments\n", program_name, LUST2D_EXE);
        }
        return 1;
    }

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    cmd_append(&cmd, "cc");
    cmd_append(&cmd, "-ggdb");
    cmd_append(&cmd, "-I"RAYLIB_FOLDER"include/");
    cmd_append(&cmd, "-I"MUJS_FOLDER);
    cmd_append(&cmd, "-o", LUST2D_EXE);
    cmd_append(&cmd, "lust2d.c");
    cmd_append(&cmd, MUJS_FOLDER"one.c");
    cmd_append(&cmd, "-L"RAYLIB_FOLDER"lib/");
    cmd_append(&cmd, "-l:libraylib.a");
    cmd_append(&cmd, "-lm");
    if (!cmd_run(&cmd)) return 1;

    if (run) {
        cmd_append(&cmd, BUILD_FOLDER"lust2d");
        da_append_many(&cmd, argv, argc);
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
