#include <Background.h>
#include <Car.h>
#include <Text.h>
#include <SingleImage.h>

#include <fstream>
#include <sstream>

namespace Racing {

  std::shared_ptr<SingleImage> InitCloud(int screenWidth, int screenHeight) {
    int cloudWidth = screenWidth;
    int cloudHeight = 250;
    return std::make_shared<SingleImage>(0, 0, cloudWidth, cloudHeight);
  }

  std::shared_ptr<SingleImage> InitExplosion(int screenWidth, int screenHeight) {
    int boomWidth = 600;
    int boomHeight = 400;
    return std::make_shared<SingleImage>(
      (screenWidth - boomWidth) / 2,
      (screenHeight - boomHeight) / 2,
      boomWidth,
      boomHeight
    );
  }

  std::shared_ptr<Car> InitCar(int screenWidth, int screenHeight) {
    int carWidth = 260;
    int carHeight = 260;
    return std::make_shared<Car>(
      (screenWidth - carWidth) / 2,
      9 * screenHeight / 16,
      carWidth,
      carHeight
    );
  }

  std::shared_ptr<Text> InitStat(int screenWidth, int screenHeight, std::shared_ptr<Car> car) {
    int textWidth = screenWidth - 20;
    int textHeight = 60;
    return std::make_shared<Text>(10, 10, textWidth, textHeight, std::move(car));
  }

  std::shared_ptr<SingleImage> InitRecord(int screenWidth, int screenHeight) {
    int textWidth = screenWidth - 50;
    int textHeight = 100;
    return std::make_shared<SingleImage>(25, screenHeight - textHeight - 10, textWidth, textHeight);
  }

  std::shared_ptr<Background> InitBackground(int screenWidth, int screenHeight, std::shared_ptr<Car> car) {
    auto background = std::make_shared<Background>(0, 0, screenWidth, screenHeight, std::move(car));
    background->AddRoadSection(0.0f, 4.0f);
    background->AddRoadSection(1.0f, 3.0f);
    background->AddRoadSection(0.0f, 1.0f);
    background->AddRoadSection(-1.0f, 1.5f);
    background->AddRoadSection(0.0f, 2.5f);
    background->AddRoadSection(1.0f, 3.5f);
    background->AddRoadSection(-1.0f, 2.0f);
    background->AddRoadSection(0.5f, 5.0f);
    background->AddRoadSection(0.0f, 3.0f);
    background->AddRoadSection(0.7f, 2.5f);
    return std::move(background);
  }

  int Play() {

    int screenWidth = 800;
    int screenHeight = 600;

    auto& TGE = TGE::Engine::GetInstance();
    if (!TGE.Init("Super Racing Pro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight)) {
      return -1;
    }

    const char* s;

    auto cloud = InitCloud(screenWidth, screenHeight);
    s = "assets/textures/Clouds.png";
    if (!cloud->AddTexture(Textures::SingleImage::Only, IMG_Load(s))) {
      SDL_Log("Couldn't find file : %s", s);
      return -1;
    }

    auto explosion = InitExplosion(screenWidth, screenHeight);

    auto car = InitCar(screenWidth, screenHeight);
    s = "assets/textures/CarFront.png";
    if (!car->AddTexture(Textures::Car::Front, IMG_Load(s))) {
      SDL_Log("Couldn't find file : %s", s);
      return -1;
    }
    s = "assets/textures/CarLeft.png";
    if (!car->AddTexture(Textures::Car::Left, IMG_Load(s))) {
      SDL_Log("Couldn't find file : %s", s);
      return -1;
    }
    s = "assets/textures/CarRight.png";
    if (!car->AddTexture(Textures::Car::Right, IMG_Load(s))) {
      SDL_Log("Couldn't find file : %s", s);
      return -1;
    }

    auto stat = InitStat(screenWidth, screenHeight, car);
    int fontSize = 40;
    SDL_Color fontColor = { 0, 0, 0 }; // black letters
    s = "assets/fonts/Font1.ttf";
    if (!stat->AddFont(s, fontSize, fontColor)) {
      SDL_Log("Couldn't find file : %s", s);
      return -1;
    }
    s = "Distance : ";
    if (!stat->AddTexture(Textures::Text::DistanceKeyword,
      TTF_RenderText_Solid(stat->GetFont(), s, stat->GetColor())))
    {
      SDL_Log("Couldn't add texture : %s", s);
      return -1;
    }
    s = "Speed : ";
    if (!stat->AddTexture(Textures::Text::SpeedKeyword,
      TTF_RenderText_Solid(stat->GetFont(), s, stat->GetColor()))) {
      SDL_Log("Couldn't add texture : %s", s);
      return -1;
    }

    auto background = InitBackground(screenWidth, screenHeight, car);

    TGE.AddObject(background);
    TGE.AddObject(cloud);
    TGE.AddObject(car);
    TGE.AddObject(stat);

    int FPS = 18;
    const Uint32 frameDelay = 1000 / FPS;
    Uint32 frameStart, frameTime = 0;

    while (TGE.IsRunning()) {
      frameStart = SDL_GetTicks();
      TGE.ProcessEvents();
      TGE.Update(frameTime);
      TGE.Render();
      frameTime = SDL_GetTicks() - frameStart;
      if (frameDelay > frameTime) {
        SDL_Delay(frameDelay - frameTime);
      }
    }
    
    TGE.Restart();
    s = "assets/textures/Explosion.png";
    if (!explosion->AddTexture(Textures::SingleImage::Only, IMG_Load(s))) {
      SDL_Log("Couldn't find file : %s", s);
      return -1;
    }
    TGE.AddObject(std::move(explosion));
    TGE.Render();
    SDL_Delay(2000);

    bool isNewRecord = background->ProcessCollision();

    if (isNewRecord) {

      auto record = InitRecord(screenWidth, screenHeight);
      std::ostringstream os;
      os << "Congratulations!!! ";
      os << (std::uint64_t)car->GetDistanceOverall();
      os << " km is a new record!";
      if (!record->AddTexture(Textures::SingleImage::Only,
        TTF_RenderText_Solid(stat->GetFont(), os.str().data(), stat->GetColor())))
      {
        SDL_Log("Couldn't add texture : %s", os.str().data());
        return -1;
      }
      TGE.AddObject(std::move(record));
      TGE.Render();
      SDL_Delay(2000);  
    }

    TGE.Stop();
    SDL_Delay(2000);
    SDL_Log("Exit");
    TGE.Clean();
  }

} // end namespace Racing

void NullifyRecord() {
  std::fstream fs;
  fs.open("assets/Records.bin", fs.in | fs.out | fs.binary);
  if (!fs.is_open()) {
    SDL_Log("Cannot open record file\n");
  }
  else {
    std::uint64_t result = 0;
    fs.write(reinterpret_cast<char*>(&result), sizeof(result));
    if (fs.bad()) {
      SDL_Log("Bad record file\n");
    }
  }
  fs.close();
}

int main(int argc, char** argv) {
  try {
  
    //NullifyRecord();
    //return 0;

    Racing::Play();
    std::getchar();
    return 0;
    
  }
  catch (const std::exception& e) {
    SDL_Log("ERROR: %s", e.what());
  }
  return 0;
}