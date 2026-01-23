#ifndef GUI_HPP
#define GUI_HPP

class GUI {
    public:
        void begin();
        void destroy();
        void someWindow(const char* title, float x_pos, float y_pos, float width, float length);
};

#endif