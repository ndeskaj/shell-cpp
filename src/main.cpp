#include <iostream>
#include <cstring>

// get PATH environment variable and split the paths
std::string getPath() {
  char* path = getenv("PATH");
  if (path == NULL) {
    return "Error: no PATH environment variable";
  }
  char delims[] = ":";
  char *pathToken = strtok(path, delims);
  std::string result;
  while (pathToken != NULL) {
    result = pathToken;
    pathToken = strtok(NULL, delims);
  }
  return result;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::string input;

  // built-in commands
  const std::string builtins[] = {"echo", "exit 0", "exit", "type"};
  const int numBuiltins = sizeof(builtins) / sizeof(builtins[0]);

  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);
    
    if (input.find("type") == 0 ){
      std::string command = input.substr(5); // get command
      bool found = false;

      // check if command is in PATH
      std::string path = getPath();
      if (path.empty()) {
        std::cout << command << " not found\n";      
      } else {
        std::cout << command << " is " << path << command << std::endl;
      }


      // check if command is a built-in command
      for (int i = 0; i < numBuiltins; i++) {
        if (command == builtins[i]) {
          std::cout << command << " is a shell builtin" << std::endl;
          found = true;
          break;
        }
      }
      if (!found) {
          std::cout << command << " not found" << std::endl;
        } 
      continue;
    }
    
    // handle echo command
    if (input.find("echo") == 0) {
      std::cout << input.substr(5) << std::endl;
      continue;
    }
    // handle exit 0 command and terminate with 0
    if (input == "exit 0") {
      break;
    }
    // handling invalid comments
    std::cout << input << ": command not found" << std::endl;
  }
}
