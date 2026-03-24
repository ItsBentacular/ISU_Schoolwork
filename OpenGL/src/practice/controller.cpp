#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>

// lists all connected controllers/input devices
void listConnectedJoysticks() {
    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
        if (glfwJoystickPresent(i)) {
            const char* name = glfwGetJoystickName(i);
            if (name) {
                printf("Joystick ID %d is present. Name: %s\n", i, name);
            } else {
                printf("Joystick ID %d is present, but name is unavailable.\n", i);
            }
        }
    }
}

// loads mappings for buttons on controllers.
std::string loadGamepadMappings() {
    const std::string& path = "../../include/inputs/gamecontrollerdb.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open gamepad mappings file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char *argv[])
{
    glfwInit();
    listConnectedJoysticks();
    std::string mappings = loadGamepadMappings();
    if(!mappings.empty()) {
        if(glfwUpdateGamepadMappings(mappings.c_str()) == GLFW_TRUE) {
            std::cout << "Gamepad load Good!" << std::endl;
        } else {
            std::cout << "Gamepad load Bad!" << std::endl;
        }
    }
    
    bool wasApressed = false;
    bool wasBpressed = false;
    bool wasXpressed = false;
    bool wasYpressed = false;
    while (1)
    {
    GLFWgamepadstate state;
        if (glfwGetGamepadState(GLFW_JOYSTICK_5, &state))
        {
            bool isApressed = state.buttons[GLFW_GAMEPAD_BUTTON_A];
            bool isBpressed = state.buttons[GLFW_GAMEPAD_BUTTON_B];
            bool isXpressed = state.buttons[GLFW_GAMEPAD_BUTTON_X];
            bool isYpressed = state.buttons[GLFW_GAMEPAD_BUTTON_Y];
            bool isMenuPressed = state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];
            bool isHomePressed = state.buttons[GLFW_GAMEPAD_BUTTON_GUIDE];
            if(isApressed && !wasApressed && !isMenuPressed) {
                std::cout << "A" << std::endl;
            }
            if(isBpressed && !wasBpressed) {
                std::cout << "B" << std::endl;
            }
            if(isXpressed && !wasXpressed) {
                std::cout << "X" << std::endl;
            }
            if(isYpressed && !wasYpressed && !isHomePressed) {
                std::cout << "Y" << std::endl;
            }
            if((isMenuPressed) && (isApressed && !wasApressed)) {
                std::cout << "SHORTCUT" << std::endl;
                system("distrobox-host-exec flatpak run app.zen_browser.zen &");
            }
            if(isHomePressed && (isYpressed && !wasYpressed)) {
                std::cout << "PLEX" << std::endl;
                system("distrobox-host-exec konsole &");
            }
            wasApressed = isApressed;
            wasBpressed = isBpressed;
            wasYpressed = isYpressed;
            wasXpressed = isXpressed;

        }
        glfwPollEvents();
    }
    return 0;
}