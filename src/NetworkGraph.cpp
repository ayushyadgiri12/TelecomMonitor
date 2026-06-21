#include "NetworkGraph.h"
#include <queue>
#include <stack>
#include <unordered_set>
#include <limits>
#include <algorithm>
#include <iostream>

void NetworkGraph::addLink(const std::string& deviceA, const std::string& deviceB, double latencyMs) {
    adjList[deviceA].emplace_back(deviceB, latencyMs);
    adjList[deviceB].emplace_back(deviceA, latencyMs); // bidirectional
}

std::vector<std::string> NetworkGraph::bfs(const std::string& start) const {
    std::vector<std::string> order;
    if (adjList.find(start) == adjList.end()) return order;

    std::unordered_set<std::string> visited;
    std::queue<std::string> q;

    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        std::string cur = q.front();
        q.pop();
        order.push_back(cur);

        auto it = adjList.find(cur);
        if (it == adjList.end()) continue;

        for (const Link& link : it->second) {
            if (visited.find(link.to) == visited.end()) {
                visited.insert(link.to);
                q.push(link.to);
            }
        }
    }
    return order;
}

std::vector<std::string> NetworkGraph::dfs(const std::string& start) const {
    std::vector<std::string> order;
    if (adjList.find(start) == adjList.end()) return order;

    std::unordered_set<std::string> visited;
    std::stack<std::string> s;
    s.push(start);

    while (!s.empty()) {
        std::string cur = s.top();
        s.pop();

        if (visited.find(cur) != visited.end()) continue;
        visited.insert(cur);
        order.push_back(cur);

        auto it = adjList.find(cur);
        if (it == adjList.end()) continue;

        // Push neighbours in reverse so traversal order is intuitive
        for (auto rit = it->second.rbegin(); rit != it->second.rend(); ++rit) {
            if (visited.find(rit->to) == visited.end()) {
                s.push(rit->to);
            }
        }
    }
    return order;
}

std::vector<std::string> NetworkGraph::shortestLatencyPath(const std::string& source,
                                                             const std::string& destination,
                                                             double& totalLatency) const {
    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> prev;

    for (const auto& entry : adjList) dist[entry.first] = std::numeric_limits<double>::infinity();
    dist[source] = 0.0;

    using PQEntry = std::pair<double, std::string>;
    std::priority_queue<PQEntry, std::vector<PQEntry>, std::greater<PQEntry>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        if (u == destination) break;

        auto it = adjList.find(u);
        if (it == adjList.end()) continue;

        for (const Link& link : it->second) {
            if (dist.find(link.to) == dist.end()) dist[link.to] = std::numeric_limits<double>::infinity();
            double nd = d + link.latencyMs;
            if (nd < dist[link.to]) {
                dist[link.to] = nd;
                prev[link.to] = u;
                pq.push({nd, link.to});
            }
        }
    }

    totalLatency = 0.0;
    std::vector<std::string> path;
    if (dist.find(destination) == dist.end() ||
        dist[destination] == std::numeric_limits<double>::infinity()) {
        return path; // unreachable
    }

    totalLatency = dist[destination];

    for (std::string at = destination; ; at = prev[at]) {
        path.push_back(at);
        if (at == source) break;
        if (prev.find(at) == prev.end()) { path.clear(); return path; }
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void NetworkGraph::displayTopology() const {
    std::cout << "\n--- Network Topology (adjacency list) ---\n";
    for (const auto& entry : adjList) {
        std::cout << entry.first << ": ";
        for (const auto& link : entry.second) {
            std::cout << "-> [" << link.to << ", " << link.latencyMs << "ms]  ";
        }
        std::cout << "\n";
    }
}
