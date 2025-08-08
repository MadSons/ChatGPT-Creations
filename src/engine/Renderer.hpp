#ifndef RENDERER_HPP
#define RENDERER_HPP

// SDL rendering wrapper.
class Renderer {
public:
    bool init();
    void clear();
    void present();
};

#endif // RENDERER_HPP
