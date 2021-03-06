#include "replay_scene.hpp"
#include "title_scene.hpp"
#include "game_common.hpp"
#include "panels_gfx.hpp"

void ReplayScene::init_recorder()
{
    recorder.load(config.replay_filename);
    config.level = level = recorder.level;
    config.difficulty = static_cast<Difficulty>(recorder.difficulty);
}

void ReplayScene::init_menu()
{
    GameScene::init_menu();
    info.set_level(config.level);
    info.set_difficulty(config.difficulty);
}

void ReplayScene::update_input()
{
    trigger = 0;
    held = 0;
    recorder.keys(trigger, held);

    if (hidKeyRepeatQuick(repeat.get(KEY_LEFT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x - 1, panel_table.columns - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_RIGHT), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_x = std::max(std::min(selector_x + 1, panel_table.columns - 2), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_UP), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y - 1, panel_table.rows - 1), 0);
    if (hidKeyRepeatQuick(repeat.get(KEY_DOWN), SELECTOR_REPEAT_MS, 1, SELECTOR_QUICK_MS, held))
        selector_y = std::max(std::min(selector_y + 1, panel_table.rows - 1), 0);
    if (trigger & KEY_A)
        panel_table.swap(selector_y, selector_x);
    if (hidKeysDown() & KEY_START)
        current_scene = new TitleScene();

    recorder.update();
}

void ReplayScene::update_windows()
{
    GameScene::update_windows();
    if (held & KEY_R || held & KEY_L)
        info.clear_timeout();
    info.update();
    ccc_stats.set_matchinfo(current_match);
}

void ReplayScene::update_end_match()
{
    GameScene::update_end_match();
    info.start_timeout_timer();
}

void ReplayScene::update_on_timeout()
{
    GameScene::update_on_timeout();
    info.set_timeout(current_timeout);
}

void ReplayScene::update_on_matched()
{
    GameScene::update_on_matched();
    int needed = get_exp_to_level(level);
    info.set_score(score);
    info.pause_timeout_timer();
    info.set_level(level);
    info.set_experience(experience, needed);
    if (current_match.is_timeout())
    {
        int timeout = calculate_timeout(current_match.combo, current_match.chain + 1,
                                        3 - (int)config.difficulty, panel_table.is_danger());
        info.set_timeout(timeout);
    }
}

//TODO write this and GameScene so I don't have to override this method.
void ReplayScene::update_match()
{
    u64 current_frame = osGetTime();
    int max_wait = get_current_speed(level);

    if (panel_table.is_rised())
        selector_y = std::max(std::min(selector_y - 1, 10), 0);

    current_match = panel_table.update(current_frame - last_frame, max_wait, held & KEY_R);
    if (current_match.empty() && !last_match.empty())
    {
        if (last_match.is_timeout())
            update_on_timeout();

        update_end_match();

        last_match = current_match;
    }

    if (current_match.matched())
    {
        update_create_markers();
        update_score();
        update_level();
        update_on_matched();
        last_match = current_match;
    }

    frames.update(panel_table.get_state(), danger_panel);
    markers.update();
}

void ReplayScene::draw_game_top()
{
    ccc_stats.draw();
    info.draw();
}

void ReplayScene::draw_game_bottom()
{
    GameScene::draw_game_bottom();
    int startx = (BOTTOM_SCREEN_WIDTH - border.width()) / 2 + 9 + 4;
    int starty = BOTTOM_SCREEN_HEIGHT - border.height() + 9;
    const int panel_size = PANEL_SIZE;
    const int step = get_current_speed(level) / panel_size;
    int offset = panel_table.rise / step;

    for (int i = 0; i < panel_table.height() + 1; i++)
    {
        for (int j = 0; j < panel_table.width(); j++)
        {
            const Panel& panel = panel_table.get(i, j);
            if (panel.value == Panel::EMPTY) continue;
            int x = startx + j * panel_size;
            int y = starty + (i + 1) * panel_size - offset;

            debug.draw(x, y, panel.state * 5, 0, 5, 10);
            debug.draw(x + 11, y + 6, panel.chain * 5, 0, 5, 10);
        }
    }
}
