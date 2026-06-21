#ifndef NETWORK_GRAPH_H
#define NETWORK_GRAPH_H

#include "Models.h"
#include <unordered_map>
#include <vector>
#include <string>

// A weighted link between two network devices
struct Link {
    std::string to;
    double latencyMs; // edge weight - link latency in milliseconds

    Link(std::string t, double l) : to(t), latencyMs(l) {}
};

// ---------------------------------------------------------
// NetworkGraph
//
// Backed by an adjacency list: unordered_map<deviceId, vector<Link>>
// representing the physical/logical topology of the telecom
// network (towers, routers, switches and the links between them).
//
// Why a graph + adjacency list?
//   The problem requires analyzing "network connectivity" -
//   a graph is the natural model of any network topology, and
//   an adjacency list is memory-efficient for the sparse
//   topologies typical of telecom backhaul networks (each
//   node connects to only a handful of neighbours).
//
// Algorithms provided (BFS, DFS & Dijkstra's Algorithm
// requirement):
//   - BFS: finds all devices reachable from a given device,
//     and the FEWEST-HOPS path - useful for checking whether
//     an outage has isolated part of the network.
//   - DFS: explores the full connected component from a
//     device, useful for discovering all devices in the same
//     network segment for diagnostics.
//   - Dijkstra: finds the path between two devices with the
//     LOWEST TOTAL LATENCY - used for service-quality routing
//     and identifying the best path for rerouting traffic
//     around a failed link.
// ---------------------------------------------------------
class NetworkGraph {
private:
    std::unordered_map<std::string, std::vector<Link>> adjList;

public:
    // Add a bidirectional link between two devices - O(1)
    void addLink(const std::string& deviceA, const std::string& deviceB, double latencyMs);

    // BFS traversal from a starting device - returns devices in
    // the order visited (fewest-hops / "outage isolation" view) - O(V+E)
    std::vector<std::string> bfs(const std::string& start) const;

    // DFS traversal from a starting device - returns devices in
    // the order visited (full connected-component view) - O(V+E)
    std::vector<std::string> dfs(const std::string& start) const;

    // Dijkstra's algorithm: lowest-total-latency path between two
    // devices. Returns the ordered list of device IDs on the path
    // (empty if unreachable) and fills totalLatency - O((V+E) log V)
    std::vector<std::string> shortestLatencyPath(const std::string& source,
                                                   const std::string& destination,
                                                   double& totalLatency) const;

    void displayTopology() const;
};

#endif // NETWORK_GRAPH_H
