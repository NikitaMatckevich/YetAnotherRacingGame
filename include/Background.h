#pragma once
#include <Car.h>
#include <Text.h>

namespace Racing {

  struct TrackSection {
    float curvature;
    float distance;
  };

  class Background : public TGE::Renderable {

    void DrawSky(int width, int height, SDL_Renderer* renderer);
    void DrawForest(int width, int height, SDL_Renderer* renderer);
    void DrawRoad(int width, int height, SDL_Renderer* renderer);

    std::vector<TrackSection> m_track{};

    size_t m_currentSection { 0 };
    float  m_passedInSection{ 0 };
    float m_relativeCurvature   { 0 };

    std::shared_ptr<Car> m_car;

    bool m_collision{ false };
    void SignalCollision();

  public:

    Background(int x0, int y0, int width, int height,
      std::shared_ptr<Car> car_ptr = nullptr,
      std::shared_ptr<Text> txt_ptr = nullptr);
 
    bool ProcessCollision();
    void AddRoadSection(float curvature, float distance);

    virtual void ProcessEvent(const SDL_KeyboardEvent& e) override final;
    virtual void Update(Uint32 frameTime) override final;
    virtual void Clear()  override final;
    virtual bool Render() override final;
  };

}