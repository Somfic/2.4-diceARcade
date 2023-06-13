#include "Gui.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "DiceDetection.h"
#include "GameObject.h"
#include "obj-model.h"
#include "stb_image.h"
#include <map>
#include "ObjectManager.h"
#include "tigl.h"

cv::VideoCapture capture(0);


Gui::Gui(GLFWwindow* window)
{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    this->window = window;
    this->dices = std::vector<int>();
    this->diceStatus = NotCalibrated;
    this->camPostion = glm::vec3(0.0f);
    this->camLookat = glm::vec3(0.0f);
    this->speed = 1;
    this->game = Game();
    this->objects = std::make_shared<std::list<std::shared_ptr<GameObject>>>();
    this->objectManager = ObjectManager::ObjectManager(objects, &game);//game, ;
    std::string successString= "You threw";
    successString += std::to_string(0);
    successString += " and ";
    successString += std::to_string(0);
    this->cameraTextureId = 0;
    this->cameraCoordinates;
    std::string win = "";
    resultCodeToString = {
        {Success,successString.c_str()},
        {DiceTooNearby, "Trying to read dice...\ndice may be to nearby"},
        {TooManyDice, "Too many dice"},
        {TooLittleDice, "Place one more dice"},
        {NotCalibrated, "Not calibrate, show a 6-eyed dice to calibrate"},
        {InconsistentDiceCount, "Trying to read dice..."},
        {NoDice, "There are no dice"}
    };

}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::initGame(int totalPlayers) {
    if (!objectManager.loaded) {
        objectManager.initEnvironment("V1.goosegame");
    }
    cameraCoordinates = objectManager.cameraScreens;

    std::shared_ptr<Player> player1 = std::make_shared<Player>(0, "Green", &game);
    objectManager.addPlayer(player1);
    totalPlayers--;
    if (totalPlayers <= 0)
        return;
    std::shared_ptr<Player> player2 = std::make_shared<Player>(1, "Blue", &game);
    objectManager.addPlayer(player2);
    totalPlayers--;
    if (totalPlayers <= 0)
        return;
    std::shared_ptr<Player> player3 = std::make_shared<Player>(2, "Red", &game);
    objectManager.addPlayer(player3);
    totalPlayers--;
    if (totalPlayers <= 0)
        return;
    std::shared_ptr<Player> player4 = std::make_shared<Player>(3, "Yellow", &game);
    objectManager.addPlayer(player4);
    totalPlayers--;
    if (totalPlayers <= 0)
        return;
    std::shared_ptr<Player> player5 = std::make_shared<Player>(4, "Purple", &game);
    objectManager.addPlayer(player5);
    totalPlayers--;
    if (totalPlayers <= 0)
        return;
    std::shared_ptr<Player> player6 = std::make_shared<Player>(5, "Orange", &game);
    objectManager.addPlayer(player6);
}

void Gui::updateDice(const std::vector<int>& dice) {
    if (!dice.empty()) {
        this->dices.assign(dice.begin(), (dice.end() - 1));
        this->diceStatus = (ResultCode)dice.back();
    }
}

void Gui::win(std::string color) {
    
}

void Gui::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::draw()
{
    if (started && !loading) {
        drawGame();
        drawGameOverlay();
    }
    else if (loading) {
        drawLoadingScreen();
        init = true;
    }
    else {
        drawStartOverlay();
    }
}

void Gui::update()
{
    if (started && !loading) {
        double frameTime = glfwGetTime();
        float deltaTime = lastFrameTime - frameTime;
        lastFrameTime = frameTime;
        rotation += 0.1f * deltaTime;

        for (auto& object : *objects) {
            object->update(deltaTime);
        }
        
        if (diceStatus == Success && game.currentPlayer->getComponent<PlayerMovmentComponent>()->isFinished) {
           
            game.nextPlayer();
            game.currentPlayer->getComponent<PlayerMovmentComponent>()->isFinished = false;
            std::cout << "there was a roll with value:" << dices[0] << " " << dices[1] << std::endl;
            successString = "You threw ";
            successString += std::to_string(dices[0]);
            successString += " and ";
            successString += std::to_string(dices[1]);
            resultCodeToString[Success] = successString.c_str();
            
            std::cout << "string = " << successString << std::endl;
            game.currentPlayer->roll(dices[0] + dices[1]);
            std::cout << "PLayer " << game.currentPlayer->getId() << " is at : " << game.currentPlayer->getCurrentSpaceIndex() << std::endl;
        }
        else if (game.currentPlayer->getComponent<PlayerMovmentComponent>()->isFinished) {
            camLookat = game.getNextPlayer()->position;
            camPostion = glm::vec3(game.getNextPlayer()->position.x, game.getNextPlayer()->position.y + 10, game.getNextPlayer()->position.z + 5);
        }
        else {
            camLookat = game.currentPlayer->position;
            camPostion = glm::vec3(game.currentPlayer->position.x + 3, game.currentPlayer->position.y + 10, game.currentPlayer->position.z + 3);
        }
    }
    else if (init) {
        if (numPlayers > 0) {
            initGame(numPlayers);
            init = false;
            loading = false;
            started = true;
        }
    }
}

void Gui::drawGame() {
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 500.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(camPostion.x, camPostion.y, camPostion.z), camLookat, glm::vec3(0, 1, 0)));
    tigl::shader->setModelMatrix(glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)));

    tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);


    glPointSize(10.0f);
    for (auto& object : *objects) {
        object->draw();
    }

    drawCamera();
}

void Gui::drawStartOverlay() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //image data
    GLuint texture_id;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image_data = stbi_load("images/titleScreen.png", &width, &height, &channels, 0);
    if (image_data == nullptr)
    {
        // Error loading the image
        // Handle the error accordingly
        std::cout << "Error loading image\n";
    }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    // Create a simple GUI window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::Begin("GUI Window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // Add GUI elements here

    // Create a bullet option menu for selecting the number of players
    ImGui::Text("Select the number of players:");
    ImGui::RadioButton("2 players", &numPlayers, 2);
    ImGui::RadioButton("3 players", &numPlayers, 3);
    ImGui::RadioButton("4 players", &numPlayers, 4);
    ImGui::RadioButton("5 players", &numPlayers, 5);
    ImGui::RadioButton("6 players", &numPlayers, 6);

    if (ImGui::Button("Start Game"))
    {
        if (numPlayers > 0) {
            loading = true;
        }
        std::cout << "Starting game with " << numPlayers << " players!" << std::endl;
    }
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glDeleteTextures(1, &texture_id);
    stbi_image_free(image_data);
}
void Gui::drawLoadingScreen() {
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    GLuint texture_id;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image_data = stbi_load("images/goose game loading.jpeg", &width, &height, &channels, 0);
    if (image_data == nullptr)
    {
        // Error loading the image
        // Handle the error accordingly
        std::cout << "Error loading image\n";
    }
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::Begin("Goosegame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Image((void*)(intptr_t)texture_id, ImVec2(screenWidth, screenHeight), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glDeleteTextures(1, &texture_id);
    stbi_image_free(image_data);
}

void Gui::drawGameOverlay() {
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    //image data
    GLuint texture_id;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image_data = stbi_load("images/titleScreen.png", &width, &height, &channels, 0);
    if (image_data == nullptr)
    {
        // Error loading the image
        // Handle the error accordingly
        std::cout << "Error loading image\n";
    }


    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    // Create a simple GUI window
    ImGui::SetNextWindowPos(ImVec2(0, screenHeight * 0.75));
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight * 0.25));
    ImGui::Begin("Goosegame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    //TODO: Add the player with te current turn

    ImGui::SameLine();


    ImGui::Columns(4, "myColumns", true);

    // First column
    std::string turnString = "Turn: " + game.currentPlayer->color;
    ImGui::Text(turnString.c_str());

    for (int i = 0; i < numPlayers; i++) {
        std::string playerString = game.players[i]->color;
        playerString += " is on space ";
        playerString += std::to_string(game.players[i]->getCurrentSpaceIndex());
        playerString += ", ";
        game.players[i]->getCurrentSpace()->addName(playerString);
        ImGui::Text(playerString.c_str());
    }

    //Second column
    ImGui::NextColumn();

    ImGui::Image((void*)(intptr_t)texture_id, ImVec2(screenWidth * 0.25, screenHeight * 0.2), ImVec2(0, 1), ImVec2(1, 0));
    //Third column
    ImGui::NextColumn();
    ImGui::Text(resultCodeToString[diceStatus]);

    //Fourth column
    ImGui::NextColumn();

    if (ImGui::Button("Quit")) {
        started = false;
        for (std::shared_ptr<GameObject> player : game.players) {
            objects->remove(player);
        }
        game.restart();
        
    }

    ImGui::Columns(1); // Reset to single column layout

    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glDeleteTextures(1, &texture_id);
    stbi_image_free(image_data);
}

void Gui::drawCamera() {
    cv::Mat frame;
    capture.read(frame);
    if (!frame.empty()) {
        // If the camera is available
        glDeleteTextures(1, &cameraTextureId);

        //thanks to:
        //https://stackoverflow.com/questions/16809833/opencv-image-loading-for-opengl-texture

        cv::flip(frame, frame, 0);
        glGenTextures(1, &cameraTextureId);
        glBindTexture(GL_TEXTURE_2D, cameraTextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


        glTexImage2D(GL_TEXTURE_2D,     // Type of texture
            0,                 // Pyramid level (for mip-mapping) - 0 is the top level
            GL_RGB,            // Internal colour format to convert to
            frame.cols,          // Image width  i.e. 640 for Kinect in standard mode
            frame.rows,          // Image height i.e. 480 for Kinect in standard mode
            0,                 // Border width in pixels (can either be 1 or 0)
            GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
            GL_UNSIGNED_BYTE,  // Image data type
            frame.ptr());        // The actual image data itself

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    if (cameraTextureId == 0)
    {
        return;
    }

    // Unbind and rebind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, cameraTextureId);

    // Render a quad with the texture
    tigl::shader->enableTexture(true);
    tigl::shader->enableLighting(false);

    std::vector<Vertex> verts;
    glm::mat4 modelMatrix(1.0f);
    tigl::shader->setModelMatrix(modelMatrix);

    for (std::vector<glm::vec3> quad : cameraCoordinates)
    {
        verts.push_back(tigl::Vertex::PT(quad[0], glm::vec2(0, 0)));
        verts.push_back(tigl::Vertex::PT(quad[1], glm::vec2(1, 0)));
        verts.push_back(tigl::Vertex::PT(quad[2], glm::vec2(1, 1)));
        verts.push_back(tigl::Vertex::PT(quad[3], glm::vec2(0, 1)));
    }
    tigl::drawVertices(GL_QUADS, verts);
    tigl::shader->enableTexture(false);
    tigl::shader->enableLighting(true);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}