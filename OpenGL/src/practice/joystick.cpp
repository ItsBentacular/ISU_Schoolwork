#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>

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
    
    while (1)
    {
        if (glfwJoystickIsGamepad(GLFW_JOYSTICK_5))
        {
            
        }
        glfwPollEvents();
    }
    return 0;
}