#ifndef PUZZLE_SCENE
#define PUZZLE_SCENE

#include "game_scene.hpp"
#include <string>
#include <deque>
#include <windows/time_window.hpp>
#include <windows/puzzle_status_window.hpp>
#include <util/background.hpp>

class PuzzleScene : public GameScene
{
public:
    PuzzleScene(const GameConfig& config);
    ~PuzzleScene() {}

    void init_panel_table();
    void init_menu();

    void update_input();
    void update_windows();
    void update_gameover();
    void update_on_gameover();

    bool is_gameover() const;

    void draw_bottom();
    void draw_gameover_top();
private:
    std::deque<PanelTable> snapshots;
    TimeWindow time_window;
    PuzzleStatusWindow status_window;
    Text result_text;
};

#endif
