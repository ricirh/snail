#ifndef __SNAIL_SINGELTON_H__

#define __SNAIL_SINGELTON_H__

namespace snail
{
	template<class T, class X = void, int N = 0>
	class Singleton
	{
	public:
		static T* GetInstance()
		{
			static T v;
			return &v;
		}
	private:

	};

	template<class T, class X = void, int N = 0>
	class SingletonPtr
	{
	public:
		static std::shared_ptr<T> GetInstanc()
		{
			static std::shared_ptr<T> v(new T);
			return v;
		}
	};
}

#endif