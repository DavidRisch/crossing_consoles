#ifndef CROSSING_CONSOLES_IWEAPON_H
#define CROSSING_CONSOLES_IWEAPON_H

#include "IItem.h"

namespace game::world {

/**
 * \brief Possible weapon types.
 */
enum class WeaponType : char { GUN, SWORD, HIGHEST_ELEMENT = SWORD };

/**
 * \brief Interface for game weapons.
 */
class IWeapon : public IItem {
 public:
  virtual ~IWeapon() = default;
  virtual visual::ColoredCharMatrix GetItemBarSprite() = 0;
  virtual WeaponType GetWeaponType() = 0;
  virtual std::wstring GetName() = 0;

};  // namespace game::world
}  // namespace game::world
#endif  // CROSSING_CONSOLES_IWEAPON_H
