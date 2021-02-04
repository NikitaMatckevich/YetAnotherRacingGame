#pragma once
#include <vector>
#include <memory>
#include <TGE_Renderable.h>

namespace TGE {

  class Engine final {

    std::vector<std::shared_ptr<Renderable>> m_objects{};

    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
    bool m_running{ false };

  public:

    static inline Engine& GetInstance() {
      static Engine instance{};
      return instance;
    }

    bool Init(const char* title, int x0, int y0, int widht, int height, bool fullscreen = false);
    void Stop();
    void Restart();

    inline bool IsRunning() const { return m_running; }
    inline SDL_Window* GetWindow() const { return m_window; }
    inline SDL_Renderer* GetRenderer() const { return m_renderer; }

    void AddObject(std::shared_ptr<Renderable> ptr);

    void ProcessEvents();
    void Update(Uint32 frameTime);
    void Render();
    void Clean();
  };

} // end namespace TGE