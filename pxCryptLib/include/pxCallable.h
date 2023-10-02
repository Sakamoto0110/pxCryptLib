#pragma once


#include <type_traits>

template<typename T, typename...Tx>
class ICallable {
public:
	virtual T operator()(Tx...args) const = 0;
	virtual bool operator==(ICallable*) const = 0;
};

template<typename T, typename...Tx>
class Callable {
public:
	static constexpr size_t ARGS_COUNT = sizeof...(Tx);
	inline static const char* RETURNTYPE = typeid(T).name();
	struct {
		const char* operator[](size_t index) const noexcept {
			return index < 0 || index >= ARGS_COUNT ? "" : _args[index];
		}
	private:
		const char* _args[sizeof...(Tx)] = { (typeid(Tx).name()) ... };
	}inline static ARGS;




	T Invoke(Tx...args) const {
		if constexpr (std::is_void<T>()) {
			BeforeInvokeHook();
			(*this)(args...);
			AfterInvokeHook();
			return;
		}
		else {
			BeforeInvokeHook();
			T t = (*this)(args...);
			AfterInvokeHook();
			return t;
		}
	}



protected:

	virtual void BeforeInvokeHook() {
		/* Override this if needed */
	}
	virtual void AfterInvokeHook() {
		/* Override this if needed */
	}
};


template<typename T, typename...Tx>
class Function : public Callable<T, Tx...>, ICallable<T, Tx...> {
public:
	Function(T(*_fn)(Tx...)) :fn(_fn) {

	}
	T operator()(Tx...args) const override {
		return fn(args...);
	}
	bool operator==(ICallable<T, Tx...>* other) const override {
		return fn == ((Function*)other)->fn;
	}

	void BeforeInvokeHook() override {

	}

private:
	T(*fn)(Tx...);
};

template<typename...Tx>
class Action : public ICallable<void, Tx...>, Callable<void, Tx...> {
public:
	Action(void(*_fn)(Tx...)) :fn(_fn) {

	}
	void operator()(Tx...args) const override {
		fn(args...);
	}
	bool operator==(ICallable<void, Tx...>* other) const override {
		return fn == ((Action*)other)->fn;
	}

private:
	void(*fn)(Tx...);
};

