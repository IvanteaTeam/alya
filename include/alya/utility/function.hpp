#pragma once
#include<functional>

namespace alya
{

    namespace details
    {

        template<typename F, typename E = void>
        class function_impl;

        template<typename F>
        class function_impl<F, std::enable_if_t<std::is_copy_constructible_v<F>>>
        {
        private:
            F f;
        public:
            template<typename F>
            function_impl(F&&f) : f(std::forward<F>(f)) {}
            function_impl(function_impl&&) = default;
            function_impl(function_impl const&) = default;
            function_impl& operator=(function_impl&&) = default;
            function_impl& operator=(function_impl const&) = default;
            template<typename...Args>
            constexpr auto operator()(Args&&...args)
            {
                return f(std::forward<Args>(args)...);
            }
        };

        template<typename F>
        class function_impl<F, std::enable_if_t<!std::is_copy_constructible_v<F> && std::is_move_constructible_v<F>>>
        {
        private:
            F f;
        public:
            template<typename F>
            function_impl(F&&f) : f(std::forward<F>(f)) {}
            function_impl(function_impl&&) = default;
            function_impl(function_impl const&f) : function_impl(const_cast<F&&>(f.f)) {}
            function_impl& operator=(function_impl&&) = default;
            function_impl& operator=(function_impl const&) = delete;

            template<typename...Args>
            constexpr auto operator()(Args&&...args)
            {
                return f(std::forward<Args>(args)...);
            }

        };


    }

    template<typename T>
    class function : public std::function<T>
    {
    public:

        function() noexcept = default;
        function(std::nullptr_t) noexcept : std::function(nullptr) {}

        template<typename F>
        function(F&& f) : std::function<T>(details::function_impl<F>{std::forward<F>(f)}) {}

        function(function&&) = default;
        function& operator=(function&&) = default;
        function& operator=(std::nullptr_t) { 
            std::function::operator=(nullptr); 
            return *this; 
        }
        
        template<typename F>
        function& operator=(F&& f)
        {
            std::function::operator=(details::function_impl<F>{std::forward<F>(f)});
            return *this;
        }

        using std::function<T>::operator();
    };

}
