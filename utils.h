#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "Graph.cpp"
#include "Person.cpp"
std::vector<std::string> split(const std::string &s, char delimiter);

std::vector<int> parseIntList(const std::string &str);

std::vector<std::string> parseCsvLine(const std::string &line);

Graph readData(const std::string &filename);
