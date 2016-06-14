#include "text.hpp"
#include "font.hpp"
#include <memory>

extern std::unique_ptr<Font> font;

Text::Text(int x, int y, const std::string& str) : Widget(x, y, str.size() * 16, 16), text(str)
{

}

void Text::create(int x, int y, const std::string& str)
{
    Widget::create(x, y, str.size() * 16, 16);
    text = str;
}

void Text::draw()
{
    if (is_hidden()) return;
    font->draw(x, y, text);
}