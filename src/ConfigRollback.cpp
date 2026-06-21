#include "ConfigRollback.h"

void ConfigRollback::saveConfig(const ConfigSnapshot& snap) {
    history[snap.deviceId].push(snap);
}

bool ConfigRollback::rollback(const std::string& deviceId, ConfigSnapshot& out) {
    auto it = history.find(deviceId);
    if (it == history.end() || it->second.empty()) return false;

    out = it->second.top(); // the configuration we are rolling back FROM
    it->second.pop();
    return true;
}

bool ConfigRollback::peekLatest(const std::string& deviceId, ConfigSnapshot& out) const {
    auto it = history.find(deviceId);
    if (it == history.end() || it->second.empty()) return false;
    out = it->second.top();
    return true;
}

int ConfigRollback::historyCount(const std::string& deviceId) const {
    auto it = history.find(deviceId);
    if (it == history.end()) return 0;
    return (int)it->second.size();
}
