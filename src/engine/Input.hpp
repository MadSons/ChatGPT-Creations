#ifndef INPUT_HPP
#define INPUT_HPP

// Handles keyboard input state.
// Tracks left/right movement and jumping.
class Input {
public:
    // Update internal edge-triggered states (e.g., jumpPressed).
    void update();

    // Process SDL keyboard events.
    void handleEvent(const struct SDL_Event& e);

    bool left{false};   // true while 'A' held
    bool right{false};  // true while 'S' held
    bool jump{false};   // true while SPACE held
    bool jumpPressed{false}; // true on the frame jump becomes pressed

private:
    bool m_prevJump{false};
};

#endif // INPUT_HPP
