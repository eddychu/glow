// #pragma once
// #include <string>
// #include <cstdint>
// extern uint64_t g_id;
// class Object {
// public:
//   virtual ~Object() = default;

//   const std::string &name() const { return m_name; }
//   void set_name(const std::string &name) { m_name = name; }
//   uint64_t id() const { return m_id; }

//   static uint64_t next_id() { return g_id++; }

// protected:
//   Object() : m_id(g_id++) {}
//   Object(const std::string &name) : m_name(name), m_id(g_id++) {}

//   std::string m_name;
//   uint64_t m_id;
// };