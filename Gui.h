#ifndef GUI_H
#define GUI_H

class Gui
{
public:
    enum ResultCode {
        Success = 0,
        DiceTooNearby = 1,
        TooManyDice = 2,
        TooLittleDice = 3,
        NotCalibrated = 4,
        InconsistentDiceCount = 5
    };
    ResultCode diceStatus = NotCalibrated;

    std::map<ResultCode, const char*> resultCodeToString = {
        {Success, "Success"},
        {DiceTooNearby, "DiceTooNearby"},
        {TooManyDice, "TooManyDice"},
        {TooLittleDice, "TooLittleDice"},
        {NotCalibrated, "NotCalibrated"},
        {InconsistentDiceCount, "InconsistentDiceCount"}
    };

    Gui(GLFWwindow* window);
    ~Gui();
    void newFrame();
    void render();
    void draw();
    void update();
    void drawGame();
    void drawStartOverlay();
    void drawGameOverlay();
};

#endif // GUI_H
