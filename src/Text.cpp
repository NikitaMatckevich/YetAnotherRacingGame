#include <Text.h>

#include <string>
#include <stdexcept>

namespace Racing {

  Text::Text(int x0, int y0, int width, int height, std::shared_ptr<Car> car_ptr)
    : Texturable{ x0, y0, width, height }, m_car{ std::move(car_ptr) }
  {

    TTF_Init();

    for (size_t i = 0; i < m_cells.size(); i++) {
      m_cells[i].x = x0 + i * width / 5;
      m_cells[i].y = y0;
      m_cells[i].w = width / 6;
      m_cells[i].h = height;
    }

    for (size_t i = 2; i < m_cells.size(); i++) {
      m_cells[i].x += width / 5;
    }

  }

  bool Text::AddFont(std::string_view filename, int fontsize, SDL_Color fontcolor) {

    m_font = TTF_OpenFont(filename.data(), fontsize);
    if (!m_font) {
      SDL_Log("Enable to open font file\n");
      return false;
    }

    m_fontColor = fontcolor;
    return true;
  }

  void Text::ProcessEvent(const SDL_KeyboardEvent& e) {
  }

  void Text::Update(Uint32 frameTime) {
    std::string msg{};

    msg += std::to_string((std::uint64_t)m_car->GetDistanceOverall());
    msg += " km";
    AddTexture(Racing::Textures::Text::DistanceValue,
      TTF_RenderText_Solid(GetFont(), msg.c_str(), GetColor()));
    msg.clear();

    msg += std::to_string((std::uint8_t)(200 * m_car->GetSpeed()));
    msg += " km/h";
    AddTexture(Racing::Textures::Text::SpeedValue,
      TTF_RenderText_Solid(GetFont(), msg.c_str(), GetColor()));
  }

  bool Text::Render() {

    auto& engine = TGE::Engine::GetInstance();
    if (!engine.IsRunning()) {
      return false;
    }

    for (size_t i = 0; i < m_cells.size(); i++) {

      FetchTexture(static_cast<Textures::Text>(i));
      SDL_Texture* texture = GetTexture();
      if (!texture)
        throw std::runtime_error("Rendering null texture, please overload the default method or add a new texture\n");

      SDL_RenderCopy(engine.GetRenderer(), texture, NULL, &m_cells[i]);
    }

    return true;
  }

} // end namespace Racing