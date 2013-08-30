#ifndef DX11SANDBOX_SINGLETON_H
#define DX11SANDBOX_SINGLETON_H

namespace Dx11Sandbox
{
#define SINGLETON(TYPE) friend class Singleton<TYPE>;


	template<class T>
	class Singleton
	{
	public:

		static T* singleton();
		static void destroyInstance();
	protected:
		Singleton();
	private:
		static T* m_instance;
	};

	template<class T>
	T* Singleton<T>::m_instance = 0;

	template <class T>
	Singleton<T>::Singleton(){}

	template <class T>
	T* Singleton<T>::singleton()
	{
		if(m_instance == 0){
			m_instance = new T;
		}
		return m_instance;
	}

	template <class T>
	void Singleton<T>::destroyInstance()
	{
		SAFE_DELETE(m_instance);
	}

};

#endif
