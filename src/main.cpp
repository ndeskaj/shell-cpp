#include <iostream>

#include <sstream>

#include <string>

#include <filesystem>

std::string get_path(std::string command) {

    std::string path_env = std::getenv("PATH");

    std::stringstream ss(path_env);

    std::string path;

    while(!ss.eof()) {

        getline(ss, path, ':');

        std::string abs_path = path + "/" + command;

        if (std::filesystem::exists(abs_path)) {

            return abs_path;

        }

    }

    return "";

}

int main() {

    bool exit = false;

    while (!exit) {

        // Flush after every std::cout / std:cerr

        std::cout << std::unitbuf;

        std::cerr << std::unitbuf;

        std::cout << "$ ";

        std::string input;

        std::getline(std::cin, input);

        if (input == "exit 0") {

            exit = true;

        } else if (input.substr(0, 5) == "type ") {

            std::string cmd = input.substr(5);

            if (cmd.substr(0,4) == "type" || cmd.substr(0,4) == "exit" || cmd.substr(0,4) == "echo") {

                std::cout << cmd << " is a shell builtin\n";

            } else {
              std::string path = get_path(cmd);

                if (path.empty()) {

                    std::cout << cmd << ": not found\n";

                } else {

                    std::cout << input.substr(5) << " is " << path << std::endl;

                }

            }

        } else if (input.substr(0, 5) == "echo ") {

            std::string cmd = input.substr(5);

            std::cout << cmd << "\n";

        } else {

            std::cout << input << ": command not found\n";

        }

    }

    return 0;

}