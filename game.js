var RECT_SIZE = 50;
var BACKGROUND = {r: 0x18, g: 0x18, b: 0x18, a: 255};
var FOREGROUND = {r: 0, g: 0, b: 255, a: 255};

function init() {
    return {x: 0, y: 0, dx: 10, dy: 10};
}

function render() {
    background(BACKGROUND);
    rectangle(state.x, state.y, RECT_SIZE, RECT_SIZE, FOREGROUND);
    var nx = state.x + state.dx;
    if (nx < 0 || nx + RECT_SIZE >= width()) {
        state.dx = -state.dx;
    } else {
        state.x = nx;
    }
    var ny = state.y + state.dy;
    if (ny < 0 || ny + RECT_SIZE >= height()) {
        state.dy = -state.dy;
    } else {
        state.y = ny;
    }
}
