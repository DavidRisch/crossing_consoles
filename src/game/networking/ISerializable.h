#ifndef CROSSING_CONSOLES_I_SERIALIZABLE_H
#define CROSSING_CONSOLES_I_SERIALIZABLE_H

#include <list>
#include <unordered_map>

namespace game::networking {

/**
 * \brief Interface for objects which can be serialize to bytes.
 * \details Any object `T` implementing this interface should a static method with the signature
 * `static T Deserialize(std::vector<uint8_t>::iterator &input_iterator)`.
 */
class ISerializable {
 public:
  /**
   * \brief Place all relevant data into the vector.
   */
  virtual void Serialize(std::vector<uint8_t>& output_vector) const = 0;

  /**
   * \brief Place the list size and serialized versions of all contents into the vector.
   */
  template <typename T>
  static void SerializeList(std::vector<uint8_t>& output_vector, std::list<T> objects) {
    output_vector.push_back(objects.size());  // TODO: use two bytes
    for (const auto& object : objects) {
      object->Serialize(output_vector);
    }
  }

  /**
   * \brief Place the map size and serialized versions of all contents with keys into the vector.
   */
  template <typename K, typename V>
  static void SerializeMap(std::vector<uint8_t>& output_vector,
                           std::unordered_map<K, V, typename K::HashFunction> objects) {
    output_vector.push_back(objects.size());  // TODO: use two bytes
    for (const auto& pair : objects) {
      pair.first.Serialize(output_vector);
      pair.second.Serialize(output_vector);
    }
  }

  /**
   * \brief Extract a container(list, map, etc.) length encoded by `Serialize[...]()` from the iterator.
   */
  static size_t DeserializeContainerLength(std::vector<uint8_t>::iterator& from) {
    return *from++;  // TODO: use two bytes
  }
};

}  // namespace game::networking

#endif  // CROSSING_CONSOLES_I_SERIALIZABLE_H
