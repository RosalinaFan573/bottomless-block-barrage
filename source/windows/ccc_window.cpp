#include "ccc_window.hpp"
#include <scenes/scene.hpp> // for TOP_SCREEN_WIDTH
#include <cstdio>


CCCWindow::CCCWindow() : Window(TOP_SCREEN_WIDTH - 8 * 16, 0, 8 * 16, TOP_SCREEN_HEIGHT)
{

}

void CCCWindow::draw()
{
    if (is_hidden()) return;

    Window::draw();

    char buf[9];

    sprintf(buf, "%8d", info.cascade);
    draw_text(0, 0, "Cascade");
    draw_text(0, 16, buf);

    sprintf(buf, "%8d", info.combo);
    draw_text(0, 32, "Combo:");
    draw_text(0, 48, buf);

    sprintf(buf, "%8d", info.chain);
    draw_text(0, 64, "Chain:");
    draw_text(0, 80, buf);

    sprintf(buf, "%d %d", info.swap_match, info.fall_match);
    draw_text(0, 96, buf);
}
