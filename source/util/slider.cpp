#include "slider.hpp"
#include <algorithm>
#include <3ds.h>
#include <sf2d.h>
#include <cstdio>
#include <scenes/scene.hpp>
#include <util/hid_helper.hpp>

#define DEFAULT_MIN_COLOR 0xFF787878U
#define DEFAULT_MAX_COLOR 0xFFE0E0E0U
#define DEFAULT_STEPS 32


Slider::Slider(int mi, int ma, int val, int wx, int wy, int ww, int wh, int style) : Widget(wx, wy, ww, wh, style),
    min(mi), max(ma), value(val), color(DEFAULT_MIN_COLOR, DEFAULT_MAX_COLOR, DEFAULT_STEPS)
{
}

void Slider::create(int mi, int ma, int val, int x, int y, int w, int h, int style)
{
    Widget::create(x, y, w, h, style);
    min = mi;
    max = ma;
    value = val;
    color.set(DEFAULT_MIN_COLOR, DEFAULT_MAX_COLOR, DEFAULT_STEPS);
}

void Slider::update()
{
    if (!is_active()) return;

    color.update();

    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), 100, 20, 10))
        value -= 1;
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), 100, 20, 10))
        value += 1;

    value = std::max(std::min(value, max), min);
}

void Slider::draw()
{
    if (is_hidden()) return;

    sf2d_draw_rectangle(x, y, width, height, RGBA8(0x40, 0x40, 0x40, 255));
    int percent = value * width / (max - min);
    sf2d_draw_rectangle(x + percent - 2, y - 6 + height / 2, 5, 12, RGBA8(0x60, 0x60, 0x60, 255));
    sf2d_draw_rectangle(x + percent - 2 + 1, y - 6 + height / 2 + 1, 3, 10, !is_active() ? color.start() : color.color());
    if (style & LABELS)
        font->draw(value, x + width, y);
}
