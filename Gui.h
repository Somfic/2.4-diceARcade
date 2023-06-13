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
#include "game.h"
#include "ObjectManager.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

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
    void initGame(int totalPlayers);
    void drawCamera();

private:
    GLFWwindow* window;
    std::shared_ptr<GameObject> model;
    std::shared_ptr<GameObject> model3;
    std::shared_ptr < std::list<std::shared_ptr<GameObject>>> objects;
    ObjModel* model2;
    glm::vec3 camPostion;
    glm::vec3 camLookat;
    int speed;
    bool started;
    int numPlayers;
    std::vector<int> dices;
    ResultCode diceStatus;
    Game game;
    ObjectManager objectManager;
    GLuint cameraTextureId;
    std::vector<std::vector<glm::vec3>> cameraCoordinates;

    std::map<ResultCode, const char*> resultCodeToString;

    float rotation;
    double lastFrameTime;
    // Other private member variables and functions

};

#endif // GUI_H
