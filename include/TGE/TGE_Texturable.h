#pragma once
#include <string_view>
#include <stdexcept>
#include <TGE_ArrayDict.h>
#include <TGE_Renderable.h>
#include <TGE_Engine.h>

namespace TGE {

  template <class TextureNames, size_t TextureNumber>
  class Texturable : public Renderable {

    TGE::ArrayDict<SDL_Texture*, TextureNames, TextureNumber> m_textures{};

  protected:

    Texturable(int x0, int y0, int width, int height)
      : Renderable{ x0, y0, width, height } {}

  public:

    bool AddTexture(TextureNames texturename, SDL_Surface* surface) {

      auto& engine = Engine::GetInstance();
      if (!engine.IsRunning()) {
        return false;
      }

      m_textures.FetchEntry(texturename);
      m_textures.GetEntry() = SDL_CreateTextureFromSurface(engine.GetRenderer(), surface);
      SDL_FreeSurface(surface);
    }

    void FetchTexture(TextureNames name) {
      m_textures.FetchEntry(name);
    }

    SDL_Texture* GetTexture() const {
      return m_textures.GetEntry();
    }

    void DeleteTexture() {
      SDL_DestroyTexture(m_textures.GetEntry());
    }

    virtual void Clear() override {
      m_textures.ApplyToAll([](SDL_Texture* texture) {
        if (texture) SDL_DestroyTexture(texture);
        });
    }

    virtual bool Render() override {

      auto& engine = Engine::GetInstance();
      if (!engine.IsRunning()) {
        return false;
      }

      auto texture = m_textures.GetEntry();
      if (!texture)
        throw std::runtime_error("Rendering null texture, please overload the default method or add a new texture\n");

      SDL_RenderCopy(engine.GetRenderer(), texture, NULL, &GetRect());
      return true;
    }

  };
  
} // end namespace TGE