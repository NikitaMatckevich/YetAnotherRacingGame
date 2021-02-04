#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

namespace TGE {

  class Renderable {

    SDL_Rect m_position;

  protected:

    Renderable(int x0, int y0, int width, int height)
      : m_position{ x0, y0, width, height } {}

  public:

    virtual ~Renderable() = default;

    inline const SDL_Rect& GetRect() const noexcept { return m_position; }
    inline SDL_Rect& GetRect() noexcept { return m_position; }

    virtual void ProcessEvent(const SDL_KeyboardEvent& e) = 0;
    virtual void Update(Uint32 frameTime) = 0;
    virtual void Clear() = 0;
    virtual bool Render() = 0;

  };

} // end namespace TGE