#pragma once
#include <array>
#include <functional>
#include <type_traits>

namespace TGE {

  template <class T, class Enum, size_t Size>
  class ArrayDict {

    static_assert(std::is_enum_v<Enum>, "Please, provide valid enum class to ArrayDict\n");
    std::array<T, Size> m_storage{};
    size_t m_last_accessed{0};

  public:

    ~ArrayDict() = default;

    constexpr size_t Size() const noexcept { return Size; }

    void FetchEntry(Enum e) noexcept { m_last_accessed = static_cast<size_t>(e); }
    T& GetEntry() noexcept { return m_storage[m_last_accessed]; }
    const T& GetEntry() const noexcept { return m_storage[m_last_accessed]; }
    
    void ApplyToAll(const std::function<void(T)>& f) {
      for (auto& element : m_storage) {
        f(element);
      }
    }
  };

} // end namespace TGE