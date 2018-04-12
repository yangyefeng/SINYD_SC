#ifndef _Singleton_H_
#define _Singleton_H_

#include "Mutex.h"
#include <memory>

template<class T>
class CSingleton
{
public:
	static T* GetInstance()
	{
		if (NULL == m_pInstance.get())
		{
			m_cs.Lock();
			if (NULL == m_pInstance.get())
			{
				CSingleton<T>::m_pInstance.reset(new T());
			}
			m_cs.Unlock();
		}

		return m_pInstance.get();
	}
// 	static void ReleaseInstance()
// 	{
// 		m_cs.Lock();
// 		if (!!m_pInstance)
// 		{
// 			delete CSingleton<T>::m_pInstance;
// 			CSingleton<T>::m_pInstance = 0;
// 		}
// 		m_cs.Unlock();
// 	}

private:
	CSingleton(CSingleton const&);
	CSingleton& operator=(CSingleton const&);

private:
	static std::auto_ptr<T> m_pInstance;
	static Mutex m_cs;

	CSingleton() {}
	~CSingleton() {}
};

template<class T>
std::auto_ptr<T> CSingleton<T>::m_pInstance;
template<class T>
Mutex CSingleton<T>::m_cs;


#define DECLARE_SINGLETON_CLASS(type) friend class std::auto_ptr<type>;\
									friend class CSingleton<type>;



#endif	//_Singleton_H_
