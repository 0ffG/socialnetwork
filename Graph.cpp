#include <queue>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Graph.h"
#include <iostream>
#include <iterator> 
#include <stack>

Graph::Graph() : size(0) {}

void Graph::addPerson(int id, const Person &person) {
    graph.push_back(std::make_pair(id, person));
    size++;
}

const Person *Graph::getPerson(int id) const {
     for (auto &i : graph) {
        if (i.first == id)
            return &i.second;
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    for (const auto& p : graph.graph) {
        os << "ID: " << p.first << ", Person: " << p.second.getName() << std::endl;
    }
    return os;
}

std::vector<std::set<int>> Graph::girvanNewman(int iterations) const {
    std::vector<std::set<int>> communities;
    
    Graph modifiableGraph = *this;
    
    for (int i = 0; i < iterations; i++) {
        std::map<std::pair<int, int>, double> edgeBetweenness;

        for (const auto &[id, person] : modifiableGraph.getGraph()) {
            std::map<int, int> numShortestPaths;
            std::map<int, double> dependency;
            std::map<int, std::vector<int>> predecessors;
            std::queue<int> q;
            std::stack<int> s;
            
            for (const auto &[nid, nperson] : modifiableGraph.getGraph()) {
                numShortestPaths[nid] = 0;
                dependency[nid] = 0.0;
            }
            numShortestPaths[id] = 1;
            q.push(id);
            
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                s.push(v);
                for (int w : modifiableGraph.getPerson(v)->getFriends()) {
                    if (numShortestPaths[w] == 0) {
                        q.push(w);
                    }
                    if (numShortestPaths[w] < numShortestPaths[v] + 1) {
                        numShortestPaths[w] += numShortestPaths[v];
                        predecessors[w].push_back(v);
                    }
                }
            }

            while (!s.empty()) {
                int w = s.top();
                s.pop();
                for (int v : predecessors[w]) {
                    double c = (double(numShortestPaths[v]) / numShortestPaths[w]) * (1.0 + dependency[w]);
                    edgeBetweenness[{std::min(v, w), std::max(v, w)}] += c;
                    dependency[v] += c;
                }
            }
        }

        auto maxEdge = std::max_element(edgeBetweenness.begin(), edgeBetweenness.end(),
                                         [](const auto &a, const auto &b) {
                                             return a.second < b.second;
                                         });

        if (maxEdge != edgeBetweenness.end()) {
            modifiableGraph.removeFriendship(maxEdge->first.first, maxEdge->first.second);
        }
    }

    std::set<int> visited;
    for (const auto &[id, person] : modifiableGraph.getGraph()) {
        if (visited.find(id) == visited.end()) {
            std::set<int> community;
            modifiableGraph.dfs(id, community, visited);
            communities.push_back(community);
        }
    }
    return communities;
}
void Graph::dfs(int id, std::set<int>& community, std::set<int>& visited) const {
    visited.insert(id);
    community.insert(id);
    const Person* person = getPerson(id);
    if (person != nullptr) {
        for (int friendId : person->getFriends()) {
            if (visited.find(friendId) == visited.end()) {
                dfs(friendId, community, visited);
            }
        }
    }
}

void Graph::removeFriendship(int id1, int id2) const{
    Person *person1 = const_cast<Person *>(getPerson(id1));
    if (person1 != nullptr) {
        std::vector<int> friends1 = person1->getFriends();
        friends1.erase(std::remove(friends1.begin(), friends1.end(), id2), friends1.end());
        person1->setFriends(friends1);
    }

    Person *person2 = const_cast<Person *>(getPerson(id2));
    if (person2 != nullptr) {
        std::vector<int> friends2 = person2->getFriends();
        friends2.erase(std::remove(friends2.begin(), friends2.end(), id1), friends2.end());
        person2->setFriends(friends2);
    }
}

std::vector<int> Graph::suggestFriends(int person_id, int mode) const {
    const Person *person = getPerson(person_id);
    
    
    switch(mode) {
        case 1:
            return suggestFriendsByCommonFriends(person);
        case 2:
            return suggestFriendsByOccupation(person);
        case 3:
            return suggestFriendsByAge(person);
        default:
            throw std::invalid_argument("Invalid mode");
    }
}
std::vector<int> Graph::suggestFriendsByCommonFriends(const Person* person) const {
    //arkadaslik sayisi o kisi ile ortak a. sayi ile toplam arkadas sayisindan fazla ise
    std::vector<int> suggested_friends;
    if (person == nullptr) {
        return suggested_friends;
    }

    std::vector<int> current_friends = person->getFriends();

    for (const auto& node : graph) {
        if (node.first != person->getId()) {
            std::vector<int> potential_friends = node.second.getFriends();

            int common_friends = 0;
            for (int friend_id : current_friends) {
                for (int potential_friend : potential_friends)
                {
                    if (potential_friend == friend_id)
                    {
                        common_friends++;
                    }
                    
                }
            }

            if (common_friends >= current_friends.size() / 2) {
                suggested_friends.push_back(node.first);
            }
        }
    }
        for (int id : current_friends) {
        auto it = std::find(suggested_friends.begin(), suggested_friends.end(), id);
        if (it != suggested_friends.end()) {
            suggested_friends.erase(it);
        }
    }

    return suggested_friends;
}
std::vector<int> Graph::suggestFriendsByOccupation(const Person* person) const {
    std::vector<int> suggested_friends;
    if (person == nullptr) {
        return suggested_friends;
    }

    std::string occupation = person->getOccupation();

    for (const auto& node : graph) {
        if (node.first != person->getId()) {
            if (node.second.getOccupation() == occupation) {
                suggested_friends.push_back(node.first);
            }
        }
    }
    const std::vector<int>& current_friends = person->getFriends();
    for (int id : current_friends) {
        auto it = std::find(suggested_friends.begin(), suggested_friends.end(), id);
        if (it != suggested_friends.end()) {
            suggested_friends.erase(it);
        }
    }

    return suggested_friends;
}
std::vector<int> Graph::suggestFriendsByAge(const Person* person) const {
    std::vector<int> suggested_friends;
    if (person == nullptr) {
        return suggested_friends;
    }

    int age = person->getAge();

    for (const auto& node : graph) {
        if (node.first != person->getId()) {
            int friend_age = node.second.getAge();

            if (std::abs(age - friend_age) <= 5) {
                suggested_friends.push_back(node.first);
            }
        }
    }
    const std::vector<int>& current_friends = person->getFriends();
    for (int id : current_friends) {
        auto it = std::find(suggested_friends.begin(), suggested_friends.end(), id);
        if (it != suggested_friends.end()) {
            suggested_friends.erase(it);
        }
    }
    

    return suggested_friends;
}

int Graph::getSize() const {
    return graph.size();
}

std::map<int, double> Graph::degreeCentrality() const {
    std::map<int, double> centralityMap;
    for(const auto &[id, person] : graph) {
        int degree = person.getFriends().size();
        centralityMap[id] = degree;
    }
    return centralityMap;
}

double Graph::clusteringCoefficient(int id) const {
    const Person *person = getPerson(id);
    if (person == nullptr) {
        return 0;
    }

    std::vector<int> friends = person->getFriends();
    int totalFriends = friends.size();
    if (totalFriends < 2) { 
        return 0;
    }

    int mutualConnections = 0;
    for(int i = 0; i < totalFriends; ++i) {
        for(int j = i + 1; j < totalFriends; ++j) {
            const Person *friend1 = getPerson(friends[i]);
            const Person *friend2 = getPerson(friends[j]);
            if (friend1 != nullptr && friend2 != nullptr) {
                const std::vector<int> &friends1 = friend1->getFriends();
                const std::vector<int> &friends2 = friend2->getFriends();
                if (std::find(friends1.begin(), friends1.end(), friends[j]) != friends1.end() &&
                    std::find(friends2.begin(), friends2.end(), friends[i]) != friends2.end()) {
                    ++mutualConnections;
                }
            }
        }
    }

    return (2.0 * mutualConnections) / (totalFriends * (totalFriends - 1));
}

std::vector<std::pair<int, Person>> Graph::getGraph() const {
    return graph;
}

int Graph::edgeWeight(const Graph &graph, int u, int v) {
    const Person *personU = graph.getPerson(u);
    const Person *personV = graph.getPerson(v);
    if (personU == nullptr || personV == nullptr) {
        return 0;
    }

    std::vector<int> friendsU = personU->getFriends();
    std::vector<int> friendsV = personV->getFriends();

    std::set<int> commonFriends;
    std::set_intersection(friendsU.begin(), friendsU.end(),friendsV.begin(), friendsV.end(),
                          std::inserter(commonFriends, commonFriends.begin()));

    return commonFriends.size();
}


