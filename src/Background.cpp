#include <Background.h>
#include <fstream>
#include <stdexcept>

namespace Racing {

  void Background::DrawSky(int width, int height, SDL_Renderer* renderer) {
    SDL_Color color{ 204, 245, 255, 100 }; // pale blue
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
  }

  void Background::DrawForest(int width, int height, SDL_Renderer* renderer) {

    float rCurvature = m_track[m_currentSection].curvature;
    SDL_Color color;

    for (int x = 0; x < width; x++) {

      int frontHillHeight = fabs(16.f * sinf(x * 0.01f + 0.9 * m_relativeCurvature) + 
                                 cosf(pow(x, 2) * 0.05f) +
                                 20.f);

      color = { 41, 169, 42, 255 }; // forest green
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      for (int y = height / 2; y > height / 2 - frontHillHeight; y--) {
        SDL_RenderDrawPoint(renderer, x, y);
      }

      color = { 11, 102, 35, 255 }; // dark green
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      int backHillHeight = frontHillHeight + 4 * cosf(0.05f * x) + 7;
      for (int y = height / 2 - frontHillHeight; y > height / 2 - backHillHeight; y--) {
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }

  void Background::DrawRoad(int width, int height, SDL_Renderer* renderer) {

    float rCurvature = m_track[m_currentSection].curvature;

    int car_x = m_car->GetRect().x + m_car->GetRect().w / 2;
    int car_y = m_car->GetRect().y + m_car->GetRect().h / 2;

    SDL_Color color;

    for (int y = height; y > height / 2; y--) {

      float fPerspective = (2 * y - height) * 1.f / height;
      float fRoadWidth = 0.05f + 0.4 * fPerspective;
      float fClipWidth = 0.3 * fRoadWidth;

      float fMiddle = 0.5f + m_relativeCurvature * pow(1.0f - fPerspective, 3);

      int LGrass  = (fMiddle - fRoadWidth - fClipWidth) * width;
      int RGrass  = (fMiddle + fRoadWidth + fClipWidth) * width;
      int LDanger = (fMiddle - fRoadWidth) * width;
      int RDanger = (fMiddle + fRoadWidth) * width;

      if (!m_collision && (y == car_y)) {
        if ((car_x <= LGrass) || (car_x >= RGrass)) {
          SignalCollision();
        }
      }

      float frequency = powf(1.f - fPerspective, 3);
      float phase = 70.f * m_car->GetDistanceOverall();
      bool isGrassDark = sinf(40.f  * frequency + phase) > 0.;
      bool isСlipDark  = sinf(100.f * frequency + phase) > 0.;

      for (int x = 0; x < width; x++) {
        if ((x <= LGrass) || (x >= RGrass)) {
          float gradient = ((float)y * 2 / height);
          int r = 105 * gradient + 40;
          int g = 15 * gradient + 200;
          int b = 15 * gradient;
          if (!isGrassDark) {
            r += 5; g += 5;
          }
          color = { Uint8(r), Uint8(g), Uint8(b), 100 }; // green/yellow gradient
        }
        else if ((x <= LDanger) || (x >= RDanger)) {
          if (isСlipDark) color = { 255, 0, 0, 100 };     // red
          else            color = { 255, 255, 255, 255 }; // white
        }
        else {
          color = { 155, 155, 155, 255 };   // grey
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }

  void Background::SignalCollision() {
    TGE::Engine::GetInstance().Stop();
    m_collision = true;
  }

  bool Background::ProcessCollision() {
    bool isRecord = false;
    std::fstream fs;
    fs.open("assets/Records.bin", fs.in | fs.out | fs.binary);
    if (!fs.is_open()) {
      SDL_Log("Cannot open record file\n");
    }
    else {
      std::uint64_t result = m_car->GetDistanceOverall();
      std::uint64_t record;
      fs.read(reinterpret_cast<char*>(&record), sizeof(record));
      if (result > record) {
        fs.seekp(0);
        fs.write(reinterpret_cast<char*>(&result), sizeof(result));
        isRecord = true;
      }
      if (fs.bad()) {
        SDL_Log("Bad record file\n");
      }
    }
    fs.close();
    return isRecord;
  }

  Background::Background(int x0, int y0, int width, int height,
    std::shared_ptr<Car> car_ptr,
    std::shared_ptr<Text> txt_ptr)
    : Renderable{ x0, y0, width, height }
    , m_car{ std::move(car_ptr) }
  {}

  void Background::AddRoadSection(float curvature, float distance) {
    if (distance <= 0.f)
      throw std::runtime_error("Negative or zero road distance value passed");
    TrackSection section;
    section.curvature = curvature;
    section.distance  = distance;
    m_track.push_back(section);
  }

  void Background::ProcessEvent(const SDL_KeyboardEvent& e) {
  }

  void Background::Update(Uint32 frameTime) {

    float speed = m_car->GetSpeed();
    m_passedInSection += m_car->GetDistanceFrame();

    float rDistance;
    while (rDistance = m_track[m_currentSection].distance, m_passedInSection > rDistance)
    {
      m_passedInSection -= rDistance;
      m_currentSection++;
      if (m_currentSection >= m_track.size()) {
        m_currentSection = 0;
      }
    }

    float rCurvature = m_track[m_currentSection].curvature;
    float diffTrackCarCurv = (rCurvature - m_relativeCurvature) * m_car->GetDistanceFrame();
    
    m_relativeCurvature += diffTrackCarCurv;
    m_car->GetRect().x -= 7.0f * pow(speed, 2) * m_relativeCurvature;
  }

  void Background::Clear() {
    m_car.reset();
  }

  bool Background::Render() {

    auto& engine = TGE::Engine::GetInstance();
    if (!engine.IsRunning()) {
      SDL_Log("Invalid engine instance\n");
      return false;
    }
      
    int width = GetRect().w;
    int height = GetRect().h;

    auto renderer = engine.GetRenderer();

    DrawSky(width, height, renderer);
    DrawForest(width, height, renderer);
    DrawRoad(width, height, renderer);

    return true;
  }

} // end namespace Racing