#include "menuUtility.h"

namespace utility {

std::vector<std::string> split(const std::string str, const char separator, const unsigned splits) {
  std::vector <std::string> str_split(splits);
  unsigned count = 0;
  for (unsigned i = 0; i < str.length(); i++) {
    if (str[i] != separator)
      str_split[count] += str[i]; 
    else
      count++;
  }
  return str_split;
}

bool IsNumeric(std::string str) {
  for(unsigned i = 0; i < str.length(); i++) {
    if(isdigit(str[i]) == false)
        return false;
  }
  return true;
}

void InputTypeError() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  throw std::invalid_argument("Debe introducirse un numero");
}

bool VerifyInt(int min, int max, int value) {
  if (!std::cin >> value)
    InputTypeError();
  if (value < min || value > max)
    throw std::length_error("El valor a introducir debe estar dentro del rango " + std::to_string(min) + "-" + std::to_string(max));
  return 1;
}

int RequestInt(int min, int max) {
  try {
    int value; std::cin >> value;
    VerifyInt(min, max, value);
    return value;
  }
  catch(const std::invalid_argument& e) {
    std::cout << "\033[31m" << "Argumento inválido. " << e.what() << "\033[0m" << "\n";
    return RequestInt(min, max);
  }
  catch(const std::length_error& e) {
    std::cout << "\033[33m" << e.what() << "\033[0m" << "\n";
    return RequestInt(min, max);
  }
}

bool RequestBool() {
  return bool(RequestInt(0, 1));
}

std::string RequestString() {
  std::string str;
  std::cin >> str;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return str;
}

/**
 * @brief Open the file given
 * 
 * @param filename 
 * @param option o for open, c for create
 * @return std::fstream 
 */
std::fstream OpenFile(std::string filename, char option) {
  std::fstream input_file;
  try {
    if(option == 'r')
      input_file.open(filename);
    else
      input_file.open(filename, std::fstream::out);
    if (!input_file.is_open())
      throw std::ifstream::failure(filename);
  }
  catch(const std::ifstream::failure& e) {
    throw std::ifstream::failure(filename);
  }
  return input_file;
}

/**
 * @brief Read a file and returns the content represented as a queue
 * 
 * @param filename 
 * @param option 
 * @return std::queue<std::string> 
 */
std::queue<std::string> ReadFile(std::string filename, char option) {
  std::queue<std::string> data;
  std::fstream file = OpenFile(filename);
  if (option == 'l') {
    std::string line;
    while (std::getline(file, line)) {
      data.push(line);
    }
  }
  else if (option == 'v') {
    std::string value;
    while (file >> value) {
        // if (!IsNumeric(dato))
        //   throw std::invalid_argument(dato);
        data.push(value);
      }
  }
  file.close();
  // catch(const std::ifstream::failure& e) {
  //   std::cout << "\033[31m" << "No se puedo abrir el archivo" << "\033[0m" << "\n";
  //   return ReadFile();
  // }
  return data;
}

std::string generateRandomString(int n) {
	std::vector<char> alphabet = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g',
  'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u',
  'v', 'w', 'x', 'y', 'z'
  };

	std::string res = "";
	for (int i = 0; i < n; i++)
		res += alphabet[rand() % 26];
	return res;
}

}