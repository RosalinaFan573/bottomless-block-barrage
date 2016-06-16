#include "title_scene.hpp"
// TODO Get a better title screen.
#include "generic_title_screen.h"
#include <util/window.hpp>
#include "endless_config_scene.hpp"
#include "puzzle_select_scene.hpp"
#include "replay_select_scene.hpp"

void TitleScene::initialize()
{
    background.create(generic_title_screen, GENERIC_TITLE_SCREEN_WIDTH, GENERIC_TITLE_SCREEN_HEIGHT, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    command_window.create(0, 0,  7 * 16, 16, 1, {"Endless"/*, "Score", "Puzzle", "Mission", "Versus"*/});
    command_window.center(BOTTOM_SCREEN_WIDTH, BOTTOM_SCREEN_WIDTH);
    command_window.set_active(true);
}

void TitleScene::update()
{
    command_window.update();

    u32 trigger = hidKeysDown();
    u32 held = hidKeysHeld();

    if (trigger & KEY_A)
    {
        switch (command_window.selection())
        {
            case ENDLESS:
                current_scene = new EndlessConfigScene();
                break;
            case SCORE:
                current_scene = NULL;
                break;
            case PUZZLE:
                current_scene = new PuzzleSelectScene();
                break;
            case MISSION:
                current_scene = NULL;
                break;
            case VERSUS:
                current_scene = NULL;
                break;
        }
    }
    else if (held == (KEY_L | KEY_R | KEY_START))
        current_scene = new ReplaySelectScene();
    else if (trigger & KEY_B)
        current_scene = NULL;
}

void TitleScene::draw_top()
{
    background.draw();
}

void TitleScene::draw_bottom()
{
    command_window.draw();
}
