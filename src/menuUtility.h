#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <queue>
#include <fstream>

namespace utility {

std::vector<std::string> split(const std::string str, const char separator, const unsigned splits);
bool IsNumeric(std::string str);
void InputTypeError();
bool RequestBool();
std::string RequestString();
bool VerifyInt(int min, int max, int value);
int RequestInt(int min, int max);
std::queue<std::string> ReadFile(std::string filename = "", char option = 'v');
std::fstream OpenFile(std::string filename = "", char option = 'r');
std::string generateRandomString(int n);

}