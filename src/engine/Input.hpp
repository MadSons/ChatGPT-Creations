#ifndef INPUT_HPP
#define INPUT_HPP

#include <SDL.h>

// Handles keyboard input state and SDL keyboard events.
// Tracks left/right movement, jumping, and running.
class Input {
public:
    // Update internal edge-triggered states (e.g., jumpPressed).
    void update();

    // Process SDL keyboard events.
    void handleEvent(const SDL_Event& e);

    bool left{false};    // true while 'A' held
    bool right{false};   // true while 'D' held
    bool jump{false};    // true while SPACE held
    bool jumpPressed{false}; // true on the frame jump becomes pressed
    bool run{false};     // true while LEFT SHIFT held

private:
    bool m_prevJump{false};
};

#endif // INPUT_HPP
