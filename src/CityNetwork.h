//
// Created by user2 on 29/05/2023.
//

#ifndef CITYNETWORK_CITYNETWORK_H
#define CITYNETWORK_CITYNETWORK_H

#include <list>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include "CSVReader.h"


class CityNetwork {
public:
    struct Edge {
        int origin;
        int dest;
        double dist;
        Edge() : origin(-1), dest(-1), dist(INFINITY) {}
        Edge(int origin, int dest, double dist) :
            origin(origin), dest(dest), dist(dist){}
    };

    struct Node {
        int id;
        std::list<Edge> adj;
        std::string label;
        double lat;
        double lon;
        bool visited;
        Node() : id(-1), lat(INFINITY), lon(INFINITY), visited(false) {};
        explicit Node(int id, std::list<Edge> adj = {}) :
            id(id), adj(std::move(adj)), lat(INFINITY), lon(INFINITY), visited(false) {}
        Node(int id, std::string label, std::list<Edge> adj = {}) :
            id(id), label(std::move(label)), adj(std::move(adj)), lat(INFINITY), lon(INFINITY), visited(false) {}
        Node(int id, double lat, double lon, std::list<Edge> adj = {}) :
            id(id), lat(lat), lon(lon), adj(std::move(adj)), visited(false) {}
    };

    class Path {
        std::list<Edge> path;
        double distance;
    public:
        explicit Path(std::list<Edge> path = {}, double distance = 0.0) :
            path(std::move(path)), distance(distance) {}
        const std::list<Edge>& getPath() const { return path; }
        double getDistance() const { return distance; }
        size_t getPathSize() const { return path.size(); }
        bool isValid() const { return distance != INFINITY; }
        void addToPath(Edge edge) {
            path.push_back(edge);
            distance += edge.dist;
        }
        void removeLast() {
            distance -= path.back().dist;
            path.pop_back();
        }
        bool operator<(const Path& pathObj) const {
            return distance < pathObj.distance;
        }
    };
private:
    std::vector<Node> nodes;
    long edgeCount;

    void initializeEdges(const CSV& edgesCSV);
    void initializeNodes(const CSV& nodesCSV);
    void initializeNetwork(const CSV& networkCSV);
    void clearData();
    void addNode(const Node &node);
    void addEdge(const Edge &edge);
    std::list<Edge> getAdj(int nodeId);
    bool nodeExists(int nodeId);
    Edge getEdge(int nodeId1, int nodeId2);
    Node& getNode(int nodeId);
    void clearVisits();
    bool isVisited(int nodeId);
    void visit(int nodeId);
    void unvisit(int nodeId);
    void backtrackingHelper(int currNodeId, Path currentPath, Path& bestPath);
public:
    /**
     * @brief Default constructor.
     *
     * Creates a new empty city network manager.
     */
    CityNetwork();
    /**
     * @brief Constructs a new CityNetwork object and initializes it with data on the datasetPath.
     * @param datasetPath The path to the directory containing the CSV files.
     */
    explicit CityNetwork(const std::string& datasetPath, bool isDirectory);
    /**
     * @brief Initializes the CityNetwork object with data from a CSV file.
     * @param datasetPath The path to the directory containing the CSV files.
     */
    void initializeData(const std::string& datasetPath, bool isDirectory);

    Path backtracking();

    Path triangularApproxHeuristic();

    /* TODO: path otherHeuristics(); */

    friend std::ostream& operator<<(std::ostream& os, const CityNetwork& cityNet);
};

std::ostream &operator<<(std::ostream &os, const CityNetwork::Path &cityPath);

#endif //CITYNETWORK_CITYNETWORK_H
