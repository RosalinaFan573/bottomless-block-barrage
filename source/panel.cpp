#include "panel.hpp"

Panel::Type Panel::random(int colors)
{
    return (Panel::Type)randomInt(1, colors);
}

void Panel::update()
{
    if (is_swapping())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            value = swap_to;
            swap_to = Type::EMPTY;
            state = State::SWAPPED;
        }
    }
    else if (is_swapped())
    {
        state = State::IDLE;
    }
    else if (is_pending_fall())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            state = State::FALLING;
            countdown = FALLING_FRAMES;
        }
    }
    else if (is_falling())
    {
        countdown -= 1;
        if (countdown <= 0)
            state = State::END_FALL;
    }
    else if (is_fall_end())
    {
        state = State::IDLE;
        cascade = false;
    }
    else if (is_pending_match())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            state = State::MATCHED;
            countdown = match_time;
        }
    }
    else if (is_matched())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            state = State::REMOVED;
            countdown = remove_time - match_time;
        }
    }
    else if (is_removed())
    {
        countdown -= 1;
        if (countdown <= 0)
        {
            value = Type::EMPTY;
            state = State::END_MATCH;
        }
    }
    else if (is_match_end())
    {
        state = State::IDLE;
    }
}

void Panel::swap(Panel::Type swap_panel, bool is_left)
{
    swap_to = swap_panel;
    state = is_left ? State::LEFT_SWAP : State::RIGHT_SWAP;
    countdown = SWAP_FRAMES;
}

void Panel::fall(bool already_falling, bool is_cascade)
{
    state = already_falling ? State::FALLING : State::PENDING_FALL;
    countdown = already_falling ? FALLING_FRAMES : PENDING_FALL_FRAMES;
    cascade = is_cascade;
}

void Panel::match(int index, int total)
{
    state = State::PENDING_MATCH;
    match_time = index * MATCH_FRAMES;
    remove_time = total * MATCH_FRAMES;
    countdown = PENDING_MATCH_FRAMES;
}