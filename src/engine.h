// #pragma once
// #include <chrono>
// #include <core/window.h>
// class Engine {
// public:
//   static Engine &get_instance() {
//     static Engine instance;
//     return instance;
//   }
//   Engine() = default;
//   Engine(const Engine &) = delete;
//   void initialize();
//   void shutdown();
//   void run();
//   bool is_quit() const { return m_is_quit; }
//   int get_fps() const { return m_fps; }

// protected:
//   void logical_tick(float delta_time);
//   bool renderer_tick(float delta_time);
//   void calculate_fps(float delta_time);
//   float calculate_delta_time();

// protected:
//   bool m_is_quit{false};
//   std::chrono::steady_clock::time_point m_last_tick_time_point{
//       std::chrono::steady_clock::now()};
//   float m_average_duration{0.f};
//   int m_frame_count{0};
//   int m_fps{0};
//   Window m_window;
// };