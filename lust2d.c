#include <stdio.h>
#include <stdlib.h>
#include <mujs.h>
#include <raylib.h>
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

static const char *console_js = "var console = { log: print, debug: print, warn: print, error: print };";

static Color js_tocolor(js_State *J, int i)
{
    Color result = {0};

    int c = i++;
    js_getproperty(J, c, "r");
    result.r = js_tointeger(J, i++);
    js_getproperty(J, c, "g");
    result.g = js_tointeger(J, i++);
    js_getproperty(J, c, "b");
    result.b = js_tointeger(J, i++);
    js_getproperty(J, c, "a");
    result.a = js_tointeger(J, i++);

    js_pop(J, 4);

    return result;
}

static void jsB_width(js_State *J)
{
    js_pushnumber(J, GetScreenWidth());
}

static void jsB_height(js_State *J)
{
    js_pushnumber(J, GetScreenHeight());
}

static void jsB_background(js_State *J)
{
    ClearBackground(js_tocolor(J, 1));
    js_pushundefined(J);
}

static void jsB_rectangle(js_State *J)
{
    int i = 1;
    int x = js_tointeger(J, i++);
    int y = js_tointeger(J, i++);
    int w = js_tointeger(J, i++);
    int h = js_tointeger(J, i++);

    int c = i++;

    js_getproperty(J, c, "r");
    unsigned char r = js_tointeger(J, i++);
    js_getproperty(J, c, "g");
    unsigned char g = js_tointeger(J, i++);
    js_getproperty(J, c, "b");
    unsigned char b = js_tointeger(J, i++);
    js_getproperty(J, c, "a");
    unsigned char a = js_tointeger(J, i++);

    DrawRectangle(x, y, w, h, (Color) {r, g, b, a});

    js_pushundefined(J);
}

static void jsB_print(js_State *J)
{
    int i, top = js_gettop(J);
    for (i = 1; i < top; ++i) {
        const char *s = js_tostring(J, i);
        if (i > 1) putchar(' ');
        fputs(s, stdout);
    }
    putchar('\n');
    js_pushundefined(J);
}

int main(int argc, char **argv)
{
    const char *program_name = shift(argv, argc);

    if (argc <= 0) {
        fprintf(stderr, "Usage: %s <game.js>\n", program_name);
        fprintf(stderr, "ERROR: no game is provided\n");
        return 1;
    }
    const char *input_path = shift(argv, argc);

    int ret;
    js_State *J = js_newstate(NULL, NULL, 0);
    assert(J != NULL);

    js_newcfunction(J, jsB_print, "print", 0);
    js_setglobal(J, "print");

    js_newcfunction(J, jsB_background, "background", 0);
    js_setglobal(J, "background");
    js_newcfunction(J, jsB_rectangle, "rectangle", 0);
    js_setglobal(J, "rectangle");
    js_newcfunction(J, jsB_width, "width", 0);
    js_setglobal(J, "width");
    js_newcfunction(J, jsB_height, "height", 0);
    js_setglobal(J, "height");

    ret = js_dostring(J, console_js);
    assert(ret == 0);

    InitWindow(800, 600, "Lust2D");

    SetTargetFPS(60);

    ret = js_dofile(J, input_path);
    assert(ret == 0);

    js_pushstring(J, "init();");
    js_eval(J);
    js_setglobal(J, "state");

    while (!WindowShouldClose()) {
        BeginDrawing();
        if (IsKeyPressed(KEY_F5)) {
            ret = js_dofile(J, "game.js");
            assert(ret == 0);
        }
        if (js_try(J)) {
            // TODO: report the location withing the file where the exception has happened
            TraceLog(LOG_ERROR, "%s", js_tostring(J, -1));
            js_pop(J, 1);
            return 1;
        }
        js_pushstring(J, "render();");
        js_eval(J);
        js_pop(J, 1);
        js_endtry(J);
        EndDrawing();
    }

    return 0;
}
