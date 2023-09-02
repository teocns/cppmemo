#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <any>
#include <sstream>



template<typename T>
class Memo {
public:
    Memo(T& object) : object_(object) {}
    
    template<typename Ret, typename... Args>
    std::string createKey(Ret (T::*func)(Args...), Args... args) {
        std::ostringstream keyStream;

        // Use address of member function as part of the key
        keyStream << reinterpret_cast<const void*>(*(const void**)(&func)) << "|";
        
        // Use delimiters to separate arguments
        auto append = [&](auto&& arg) {
            keyStream << typeid(arg).name() << ":" << arg << "|";
        };
        (append(args), ...);

        return keyStream.str();
    }
    
    template<typename Ret, typename... Args>
    Ret dispatch(Ret (T::*func)(Args...), Args... args) {
        std::unique_lock<std::mutex> lock(mutex_);
        std::string key = createKey(func, args...);
        std::cout << "Key: " + key << std::endl;
        if (cache_.find(key) != cache_.end()) {
            return std::any_cast<Ret>(cache_[key]);
        }
        
        Ret result = (object_.*func)(args...);
        cache_[key] = result;
        
        return result;
    }
    
private:
    T& object_;
    std::unordered_map<std::string, std::any> cache_;
    std::mutex mutex_;
};
