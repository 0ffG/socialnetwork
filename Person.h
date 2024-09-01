#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>

class Person {
private:
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string occupation;
    std::vector<int> friends;

public:
    Person(int id, const std::string& name, int age, const std::string& gender, const std::string& occupation, const std::vector<int>& friends);
    int getId() const;
    std::string getName() const;
    int getAge() const;
    std::string getGender() const;
    std::string getOccupation() const;
    std::vector<int> getFriends() const;
    void setFriends(std::vector<int> newFriends);
};

#endif // PERSON_H
