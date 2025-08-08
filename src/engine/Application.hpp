#ifndef APPLICATION_HPP
#define APPLICATION_HPP

class Config;

// Main application managing the game loop.
class Application {
public:
    bool init(const Config& cfg);
    void run();
    void shutdown();

private:
    void processEvents();
    void fixedUpdate(float dt);
    void variableUpdate(float dt);
    void render(float alpha);
};

#endif // APPLICATION_HPP
