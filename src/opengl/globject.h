#pragma once
#include <cstdint>
#include <limits>
#include <glad/glad.h>

/**
* GLObject is a unique identifier for a opengl resource.
* the id is initialized to the maximum value of uint32_t which will be invalid
* In any concrete GLObject subclasses, the id will be initialized to the original scene object 
* id.
*/

struct GLObject {
    protected:
    uint32_t id{std::numeric_limits<uint32_t>::max()};
    GLObject() = default;
    virtual ~GLObject() = default;

    public:
    uint32_t get_id() const { return id; }
};