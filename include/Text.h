#pragma once
#include <Car.h>

namespace Racing {

  namespace Textures {
    enum class Text : std::uint8_t {
      DistanceKeyword,
      DistanceValue,
      SpeedKeyword,
      SpeedValue,
      Counter
    };
  }

  class Text : public TGE::Texturable<Textures::Text, static_cast<size_t>(Textures::Text::Counter)> {

    TTF_Font* m_font{ nullptr };
    SDL_Color m_fontColor{};
    std::array<SDL_Rect, static_cast<size_t>(Textures::Text::Counter)> m_cells{};

    std::shared_ptr<Car> m_car;

  public:

    Text(int x0, int y0, int width, int height, std::shared_ptr<Car> car_ptr = nullptr);

    bool AddFont(std::string_view filename, int fontsize, SDL_Color fontcolor);
    inline TTF_Font* GetFont() const noexcept { return m_font; }
    inline const SDL_Color& GetColor() const noexcept { return m_fontColor; }
    inline SDL_Color& GetColor() noexcept { return m_fontColor; }

    virtual void ProcessEvent(const SDL_KeyboardEvent& e) override final;
    virtual void Update(Uint32 frameTime) override final;
    virtual bool Render() override final;

  };

} // end namespace Racing