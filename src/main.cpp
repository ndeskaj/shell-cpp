#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::string input;

  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);
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
