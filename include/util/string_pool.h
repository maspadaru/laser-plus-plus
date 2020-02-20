#ifndef LASER_UTIL_STRING_POOL_H
#define LASER_UTIL_STRING_POOL_H

#include <string>
#include <vector>

namespace laser::util {

class StringPool {
  private:
      // If I decide to count references, the pool datastructure should 
      // be a heap instead of a vector.  
    std::vector<std::string> pool;  
    StringPool() = default; 

  public:
    StringPool(StringPool const&) = delete;

    void operator=(StringPool const&) = delete;

    static StringPool &get_instance();

    /** Adds the string if not already present in pool
     * returns the ID of the string in the pool.
     * */
    size_t add_string(std::string const &str);

    std::string const &get_string(size_t id) const;

};

} // namespace laser::util

#endif // LASER_UTIL_STRING_POOL_H
