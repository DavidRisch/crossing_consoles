#ifndef CROSSING_CONSOLES_I_SERIALIZABLE_H
#define CROSSING_CONSOLES_I_SERIALIZABLE_H

#include <list>

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
   * \brief Extract a list length encoded by `SerializeList()` from the iterator.
   */
  static size_t DeserializeListLength(std::vector<uint8_t>::iterator& from) {
    return *from++;  // TODO: use two bytes
  }
};

#endif  // CROSSING_CONSOLES_I_SERIALIZABLE_H
