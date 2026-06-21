#ifndef CONFIG_ROLLBACK_H
#define CONFIG_ROLLBACK_H

#include "Models.h"
#include <stack>
#include <unordered_map>
#include <string>

// ---------------------------------------------------------
// ConfigRollback
//
// Backed by one std::stack<ConfigSnapshot> PER DEVICE
// (std::unordered_map<deviceId, stack<ConfigSnapshot>>).
//
// Why a stack?
//   Configuration changes on network devices must be
//   reversible in "undo" (LIFO) order - if a new configuration
//   push causes degraded performance, the operator needs to
//   roll back to the immediately preceding configuration
//   first, then the one before that, and so on. A stack gives
//   O(1) push (saveConfig) and O(1) pop (rollback), exactly
//   matching this undo workflow.
// ---------------------------------------------------------
class ConfigRollback {
private:
    std::unordered_map<std::string, std::stack<ConfigSnapshot>> history;

public:
    // Push a new configuration snapshot for a device - O(1)
    void saveConfig(const ConfigSnapshot& snap);

    // Roll back to the previous configuration for a device - O(1)
    // Returns true and fills `out` with the configuration that was reverted FROM.
    bool rollback(const std::string& deviceId, ConfigSnapshot& out);

    // Peek at the most recent saved configuration for a device
    bool peekLatest(const std::string& deviceId, ConfigSnapshot& out) const;

    int historyCount(const std::string& deviceId) const;
};

#endif // CONFIG_ROLLBACK_H
