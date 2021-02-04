#include <Car.h>

namespace Racing {

  Car::Car(int x0, int y0, int width, int height)
    : Texturable{ x0, y0, width, height } {}

  void Car::ProcessEvent(const SDL_KeyboardEvent& e) {
    
    if (e.state == SDL_RELEASED) {
      m_acceleration = 0.05f;
      FetchTexture(Textures::Car::Front);
      return;
    }

    switch (e.keysym.sym) {
    case SDLK_UP:
      m_acceleration =  2.0f;
      break;
    case SDLK_DOWN:
      m_acceleration = -1.0f;
      break;
    case SDLK_LEFT:
      FetchTexture(Textures::Car::Left);
      GetRect().x -= 5 * GetSpeed();
      break;
    case SDLK_RIGHT:
      FetchTexture(Textures::Car::Right);
      GetRect().x += 5 * GetSpeed();
      break;
    default:
      break;
    }
 
  }

  void Car::Update(Uint32 frameTime) {

    double elapsedTime = 1.e-3 * frameTime;
    
    m_speed += m_acceleration * elapsedTime;
    if (m_speed > 1.0f)
      m_speed = 1.0f;
    else
    if (m_speed < 0.0f)
      m_speed = 0.0f;

    m_distance_frame = m_speed * elapsedTime;

    m_distance_overall += m_distance_frame;
  }

} // end namespace Racing