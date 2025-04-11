#include <furi.h>
#include <flipperpose_icons.h>
#include <gui/gui.h>
#include <dolphin/dolphin.h>
#include <input/input.h>

typedef struct {
    int screen_no;
    bool back_pressed;
    bool running;
} GameState;

static void render_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);

    GameState* state = (GameState*)context;

    const Icon* icons[] = {
        NULL, // 0-index unused
        &I_1,
        &I_2,
        &I_3,
        &I_4};

    if(state->screen_no >= 1 && state->screen_no <= 4) {
        canvas_draw_icon(canvas, 0, 0, icons[state->screen_no]);
    } else {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 10, 30, "Invalid screen");
    }
}

static void input_callback(InputEvent* event, void* context) {
    furi_assert(event);
    furi_assert(context);

    GameState* state = (GameState*)context;

    if(event->type != InputTypePress) return;

    switch(event->key) {
    case InputKeyBack:
        state->back_pressed = true;
        state->running = false;
        break;
    case InputKeyUp:
        state->screen_no = 1;
        break;
    case InputKeyRight:
        state->screen_no = 2;
        break;
    case InputKeyDown:
        state->screen_no = 3;
        break;
    case InputKeyLeft:
        state->screen_no = 4;
        break;
    default:
        break;
    }
}

int32_t flipperpose(void* p) {
    UNUSED(p);

    // Initialize game state
    GameState game_state = {.screen_no = 1, .back_pressed = false, .running = true};

    // Set up GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    ViewPort* viewport = view_port_alloc();

    view_port_draw_callback_set(viewport, render_callback, &game_state);
    view_port_input_callback_set(viewport, input_callback, &game_state);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    // Main game loop
    while(game_state.running) {
        view_port_update(viewport);
        furi_delay_ms(50); // Small delay to prevent CPU hogging
    }

    // Cleanup
    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_record_close(RECORD_GUI);

    return 0;
}
