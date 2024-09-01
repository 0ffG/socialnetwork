#include "utils.h"

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<int> parseIntList(const std::string &str)
{
    std::vector<int> result;
    std::istringstream ss(str);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        result.push_back(std::stoi(token));
    }

    return result;
}

std::vector<std::string> parseCsvLine(const std::string &line)
{
    std::vector<std::string> result;
    std::stringstream lineStream(line);
    std::string cell;

    bool inside_quotes = false;

    std::ostringstream currentCell;

    while (lineStream)
    {
        char ch = lineStream.get();

        if (ch == '"')
        {
            inside_quotes = !inside_quotes;
        }
        else if (ch == ',' && !inside_quotes)
        {
            result.push_back(currentCell.str());
            currentCell.str("");
            currentCell.clear();
        }
        else if (ch == std::char_traits<char>::eof())
        {
            result.push_back(currentCell.str());
            break;
        }
        else
        {
            currentCell << ch;
        }
    }

    return result;
}

Graph readData(const std::string &filename)
{
    Graph graph;
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    std::getline(file, line); // skip header line

    while (std::getline(file, line))
    {
        std::vector<std::string> data = parseCsvLine(line);
        if (data.size() != 6)
        {
            throw std::runtime_error("Error processing line: Invalid CSV line: " + line);
        }

        int id = std::stoi(data[0]);
        std::string name = data[1];
        int age = std::stoi(data[2]);
        std::string gender = data[3];
        std::string occupation = data[4];
        const std::vector<int> friends = parseIntList(data[5]);

        graph.addPerson(id, Person(id, name, age, gender, occupation, friends));
    }

    return graph;
}
