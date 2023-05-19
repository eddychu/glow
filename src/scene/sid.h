#pragma once
#include <cstdint>

/**
* SceneObject is a unique identifier for a scene resource.
*/
struct SceneObject {


    static uint32_t next_id;
        protected:
    uint32_t id;
    SceneObject() : id(next_id++) {}
    bool operator==(const SceneObject &other) const { return id == other.id; }
    bool operator!=(const SceneObject &other) const { return id != other.id; }
    bool operator<(const SceneObject &other) const { return id < other.id; }
    bool operator>(const SceneObject &other) const { return id > other.id; }
    bool operator<=(const SceneObject &other) const { return id <= other.id; }
    bool operator>=(const SceneObject &other) const { return id >= other.id; }

    public:
    uint32_t get_id() const { return id; }
};

inline bool operator==(const SceneObject &SceneObject, uint32_t id) { return SceneObject.get_id() == id; }