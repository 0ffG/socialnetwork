#include <iostream>
#include "utils.cpp"
#include "Graph.h"

using namespace std;

void displayMenu() {
    cout << "Social Network Analysis Program" << endl;
    cout << "--------------------------------" << endl;
    cout << "1. Display the social network" << endl;
    cout << "2. Suggest friends for users" << endl;
    cout << "3. Calculate degree centrality for each user" << endl;
    cout << "4. Calculate clustering coefficient for each user" << endl;
    cout << "5. Detect communities using the Girvan-Newman algorithm" << endl;
    cout << "6. Exit the program" << endl;
    cout << "Enter your choice: ";
}
void displaySocialNetwork(const Graph& graph) {
    std::cout << "Social Network:" << std::endl;
    for (const auto& node : graph.getGraph()) {
        std::cout << "ID: " << node.first << ", Name: " << node.second.getName() << std::endl;
    }
}

void suggestFriendsMenu(const Graph& graph) {
    int personId, mode;
    std::cout << "Enter the person ID: ";
    std::cin >> personId;
    std::cout << "Select suggestion mode:" << std::endl;
    std::cout << "1. Common Friends" << std::endl;
    std::cout << "2. Similar Occupation" << std::endl;
    std::cout << "3. Similar Age" << std::endl;
    std::cout << "Enter the mode: ";
    std::cin >> mode;

    std::vector<int> suggestedFriends = graph.suggestFriends(personId, mode);

    std::cout << "Suggested Friends:" << std::endl;
    for (int friendId : suggestedFriends) {
        const Person* person = graph.getPerson(friendId);
        if (person != nullptr) {
            std::cout << "ID: " << person->getId() << ", Name: " << person->getName() << std::endl;
        }
    }
}

void calculateDegreeCentrality(const Graph& graph) {
    std::cout << "Degree Centrality:" << std::endl;
    std::map<int, double> degreeCentrality = graph.degreeCentrality();
    for (const auto& entry : degreeCentrality) {
        int personId = entry.first;
        double centrality = entry.second;
        std::cout << "ID: " << personId << ", Centrality: " << centrality << std::endl;
    }
}

void calculateClusteringCoefficient(const Graph& graph) {
    int personId;
    std::cout << "Enter the person ID: ";
    std::cin >> personId;

    double clusteringCoefficient = graph.clusteringCoefficient(personId);
    std::cout << "Clustering Coefficient for ID " << personId << ": " << clusteringCoefficient << std::endl;
}

void detectCommunities(const Graph& graph, int iterations) {
    std::cout << "Communities:" << std::endl;

    std::vector<std::set<int>> communities = graph.girvanNewman(iterations);
    int communityCount = 1;
    for (const std::set<int>& community : communities) {
        std::cout << "Community " << communityCount << ":" << std::endl;
        for (int personId : community) {
            const Person* person = graph.getPerson(personId);
            if (person != nullptr) {
                std::cout << person->getName() << std::endl;
            }
        }
        communityCount++;
        std::cout << std::endl;
    }
}




int main() {
    std::string filename = "social_network.csv";
    Graph graph = readData(filename);
    int choice;
    while (true) {
        std::cout << " " << std::endl;
        displayMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                displaySocialNetwork(graph);
                break;
            case 2:
                suggestFriendsMenu(graph);
                break;
            case 3:
                calculateDegreeCentrality(graph);
                break;
            case 4:
                calculateClusteringCoefficient(graph);
                break;
            case 5:
                detectCommunities(graph ,10);
                break;
            case 6:
                std::cout << "Exiting the program." << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }

        std::cout << std::endl;
    }


    return 0;
}
