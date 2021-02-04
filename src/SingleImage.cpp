#include <SingleImage.h>

namespace Racing {

  SingleImage::SingleImage(int x0, int y0, int width, int height)
    : Texturable{ x0, y0, width, height } {}

  void SingleImage::ProcessEvent(const SDL_KeyboardEvent& e) {
  }

  void SingleImage::Update(Uint32 frameTime) {
  }

} // end namespace Racing