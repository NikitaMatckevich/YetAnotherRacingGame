#pragma once
#include <TGE/TGE.h>

namespace Racing {

  namespace Textures {
    enum class Car : std::uint8_t {
      Front,
      Left,
      Right,
      Counter
    };
  }

  class Car : public TGE::Texturable<Textures::Car, static_cast<size_t>(Textures::Car::Counter)> {

    float m_acceleration{ 0 };
    float m_speed{ 0 };
    float m_distance_overall{ 0 };
    float m_distance_frame{ 0 };

  public:

    Car(int x0, int y0, int width, int height);

    inline float  GetAcceleration() const noexcept { return m_acceleration; }
    inline float& GetAcceleration() noexcept { return m_acceleration; }

    inline float  GetSpeed() const noexcept { return m_speed; }
    inline float& GetSpeed() noexcept { return m_speed; }

    inline float  GetDistanceOverall() const noexcept { return m_distance_overall; }
    inline float& GetDistanceOverall() noexcept { return m_distance_overall; }

    inline float  GetDistanceFrame() const noexcept { return m_distance_frame; }
    inline float& GetDistanceFrame() noexcept { return m_distance_frame; }

    virtual void ProcessEvent(const SDL_KeyboardEvent& e) override final;
    virtual void Update(Uint32 frameTime) override final;
  
  };

} // end namespace Racing