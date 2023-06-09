#ifndef GUI_H
#define GUI_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "GameObject.h"
#include "obj-model.h"

class Gui {
public:
    enum ResultCode {
        Success = 0,
        DiceTooNearby = 1,
        TooManyDice = 2,
        TooLittleDice = 3,
        NotCalibrated = 4,
        InconsistentDiceCount = 5
    };

    Gui(GLFWwindow* window);
    ~Gui();

    void updateDice(const std::vector<int>& dice);
    void newFrame();
    void render();
    void draw();
    void update();
    void drawGame();
    void drawStartOverlay();
    void drawGameOverlay();

private:
    GLFWwindow* window;
    std::shared_ptr<GameObject> model;
    std::shared_ptr<GameObject> model3;
    std::shared_ptr < std::list<std::shared_ptr<GameObject>>> objects;
    ObjModel* model2;
    glm::vec3 camPosition;
    int speed;
    bool started;
    int numPlayers;
    std::vector<int> dices;
    ResultCode diceStatus;

    std::map<ResultCode, const char*> resultCodeToString;

    float rotation;
    double lastFrameTime;
    // Other private member variables and functions

};

#endif // GUI_H
