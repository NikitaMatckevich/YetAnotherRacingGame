#pragma once
#include <TGE/TGE.h>

namespace Racing {

  namespace Textures {
    enum class SingleImage : std::uint8_t {
      Only,
      Counter
    };
  }

  class SingleImage : public TGE::Texturable<Textures::SingleImage, static_cast<size_t>(Textures::SingleImage::Counter)> {

  public:

    SingleImage(int x0, int y0, int width, int height);

    virtual void ProcessEvent(const SDL_KeyboardEvent& e) override final;
    virtual void Update(Uint32 frameTime) override final;

  };

} // end namespace Racing