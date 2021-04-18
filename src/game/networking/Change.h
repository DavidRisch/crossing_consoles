#ifndef CROSSING_CONSOLES_CHANGE_H
#define CROSSING_CONSOLES_CHANGE_H

#include <cstdint>
#include <vector>

namespace game::networking {

enum class ChangeType : uint8_t {
  // client -> server
  SET_NAME = 0,
  SET_COLOR,
  MOVE_UP,
  MOVE_RIGHT,
  MOVE_DOWN,
  MOVE_LEFT,
  USE_ITEM,

  // server -> client
  SET_OWN_ID,
  SET_WORLD,
  UPDATE_WORLD,

  HIGHEST_ELEMENT = UPDATE_WORLD
};

/**
 * \brief Used to communicate some kind of change between server and client.
 */
class Change {
 public:
  /// Used for a new `Change`, which will be sent to the other side.
  explicit Change(ChangeType change_type);
  /// Used to receive a `Change` from the other side.
  explicit Change(std::vector<uint8_t> bytes);

  [[nodiscard]] ChangeType GetChangeType() const;

  [[nodiscard]] std::vector<uint8_t>::iterator GetContentIterator();

  /**
   * \brief Returns true iff the Change type matches MOVE_* .
   */
  bool IsMovement() const;

  std::vector<uint8_t> payload;

 private:
  ChangeType change_type;
};

}  // namespace game::networking

#endif  // CROSSING_CONSOLES_CHANGE_H
