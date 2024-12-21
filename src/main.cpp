#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>


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

void execute_command(const std::string& input){
    std::stringstream ss(input);
    std::string cmd;
    ss >> cmd;   // extract command name

    // collect the arguments
    std::vector<std::string> args;
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }

    // convert args to char* array
    std::vector<char*> argv;
    argv.push_back(const_cast<char*>(cmd.c_str()));
    for(auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    //create child process and execute the command
    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv.data());
        std::cerr << cmd << ": command not found\n";
        exit(1);
    } else if (pid > 0){
        int status;
        waitpid(pid, &status, 0); // wait for child process to finishs
    } else {
        std::cerr << "fork failed\n";
    }

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
            execute_command(input);
        }
    }
    return 0;
}