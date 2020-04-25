#pragma once
#include <memory>

namespace cxx
{
	//使用匿名函数或者仿函数作为参数，
	//创建一个实现 on scope exit 功能的对象
	template<class tFunctor> class scope_guard;
	template<class tFunctor> scope_guard<tFunctor> make_scope_guard(tFunctor&& callback);

	template<class tFunctor>
	class scope_guard
	{
    public:
        scope_guard(tFunctor&& callback)
            : callback(std::move(callback)) { }
        
        scope_guard(scope_guard&& other)
            : callback(std::move(other.callback))
            , dismissed(other.dismissed)
        {
            other.dismissed = true;
        }

		~scope_guard()
		{
			if (!is_dismissed())
			{
				callback();
			}
		}

		void dismiss() { dismissed = true; }

		bool is_dismissed() const { return dismissed; }

		operator bool const () { return !dismissed; }

	private:
		tFunctor callback;
		bool dismissed = false;
	};

	template<class tFunctor>
	scope_guard<tFunctor> make_scope_guard(tFunctor&& callback)
	{
		return scope_guard<tFunctor>(std::move(callback));
	}

	//使用 on_exit_helper() << [&] { }
	//的语法，让代码看起来更清晰
	struct scope_syntax
	{
		template<class tFunctor>
		scope_guard<tFunctor> operator<<(tFunctor&& callback)
		{
			return make_scope_guard<tFunctor>(std::forward<tFunctor>(callback));
		}
	};

	inline scope_syntax& on_exit_helper()
	{
		static scope_syntax instance;
		return instance;
	}

	//Unique Interface
	template<class tInterface>
	struct unique_interface_releaser
	{
		void operator()(tInterface* p)
		{
			safe_release<tInterface*>(p);
		}
	};

	template<class tInterface>
	using unique_i = std::unique_ptr<tInterface, unique_interface_releaser<tInterface>>;
}

#define CXX_JOIN_IMPL(string_a, string_b) string_a##string_b
#define CXX_JOIN(string_a, string_b) CXX_JOIN_IMPL(string_a, string_b)
#define ON_EXIT const auto CXX_JOIN(scope_guard_at_line,__LINE__) = cxx::on_exit_helper() << [&]()
