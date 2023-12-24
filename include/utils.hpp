#ifndef CHESS_INCLUDE_UTILS_HPP
#define CHESS_INCLUDE_UTILS_HPP

#include <memory>

namespace utils {

template <class T>
class Singleton {
public:
	Singleton(const Singleton&)							   = delete;
	Singleton&				   operator=(const Singleton&) = delete;

	static std::shared_ptr<T>& get() {
		if (!_instance) _instance.reset(new T());

		return _instance;
	}

	virtual ~Singleton() = default;

protected:
	Singleton() = default;

private:
	static std::shared_ptr<T> _instance;
};

template <class T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

}  // namespace utils

#endif	// CHESS_INCLUDE_UTILS_HPP
