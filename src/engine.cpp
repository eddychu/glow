// #include "engine.h"
// #include <GLFW/glfw3.h>
// void Engine::initialize() {

//   m_window.initialize({
//       .width = 1280,
//       .height = 720,
//       .title = "Hello World",
//       .is_fullscreen = false,
//   });

//   m_window.register_on_key_func(
//       [&](int key, int scancode, int action, int mods) {
//         if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//           Engine::get_instance().m_is_quit = true;
//           m_window.set_should_close(true);
//         }
//       });
//   m_renderer.initialize();
// }

// void Engine::shutdown() {
//   //   m_scene.shutdown();
//   //   m_renderer.shutdown();
//   m_window.destroy();
// }

// void Engine::run() {
//   while (!m_window.should_close()) {
//     auto delta_time = calculate_delta_time();
//     logical_tick(delta_time);
//     renderer_tick(delta_time);
//     glfwSwapBuffers(m_window.window());
//     glfwPollEvents();
//     calculate_fps(delta_time);
//   }
// }

// void Engine::logical_tick(float delta_time) {
//   //   m_scene.tick(delta_time);
// }

// bool Engine::renderer_tick(float delta_time) {
//   //   m_renderer.tick(delta_time);
//   return true;
// }

// float Engine::calculate_delta_time() {
//   auto current_time_point = std::chrono::steady_clock::now();
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
//                       current_time_point - m_last_tick_time_point)
//                       .count();
//   m_last_tick_time_point = current_time_point;
//   return static_cast<float>(duration) / 1000000.f;
// }

// void Engine::calculate_fps(float delta_time) {
//   m_frame_count++;

//   if (m_frame_count == 1) {
//     m_average_duration = delta_time;
//   } else {
//     m_average_duration =
//         m_average_duration * (1 - 1.f / 100) + delta_time * (1.f / 100);
//   }

//   m_fps = static_cast<int>(1.f / m_average_duration);
// }
