#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Person.h" 


class Graph {

public:
    Graph();
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
    void addPerson(int id, const Person &person);
    const Person* getPerson(int id) const;
    int getSize() const;
    void removeFriendship(int id1, int id2)const;
    std::vector<int> suggestFriends(int person_id, int mode) const;
    std::vector<std::pair<int, Person>> getGraph() const;
    std::vector<std::set<int>> girvanNewman(int iterations)const;
    std::map<int, double> degreeCentrality() const;
    double clusteringCoefficient(int id) const;
    static int edgeWeight(const Graph &graph, int u, int v); 
    void dfs(int id, std::set<int>& community, std::set<int>& visited) const;
private:
    std::vector<std::pair<int, Person>> graph;
    int size;
    std::vector<int> suggestFriendsByCommonFriends(const Person* person) const;
    std::vector<int> suggestFriendsByOccupation(const Person* person) const;
    std::vector<int> suggestFriendsByAge(const Person* person) const;

};

#endif // GRAPH_H
