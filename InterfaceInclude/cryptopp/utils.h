#ifndef __UTILS_H__
#define __UTILS_H__

#include <Windows.h>
#include <vector>

namespace utils
{
    // convert
    template<class ToType, class FromType>
    struct convert
    {
    };

    template <>
    struct convert<unsigned char, unsigned long>
    {
        static unsigned char from(unsigned long value)
        {
            if (value > UCHAR_MAX)
                throw std::exception("Cannot cast");

            return (unsigned char)(value);
        }
    };

    template <int iCanBeNonZero>
    class CStaticAssert
    {
    public:
    };

    template <>
    class CStaticAssert<0>
    {
    public:
        CStaticAssert()
        {
        }
        virtual void Error()=0;
    };

    class CLocalAllocGuard
    {
        void * h_;
        CLocalAllocGuard(const CLocalAllocGuard&);
        CLocalAllocGuard& operator=(const CLocalAllocGuard&);
    public:
        explicit CLocalAllocGuard(void * h)
            :h_(h){}
        ~CLocalAllocGuard()
        {
            if(h_) LocalFree(h_); 
        }
        void * release()
        {
            void * hRes = h_;
            h_ = 0;
            return hRes;
        }
    };

    template<class ContainerType>
    typename ContainerType::pointer GetBeginOf(ContainerType & container)
    {
        if (container.empty())
            return 0;

        return &container.at(0);
    }
    template<class ContainerType>
    typename ContainerType::const_pointer GetBeginOf(const ContainerType & container)
    {
        if (container.empty())
            return 0;

        return &container.at(0);
    }

    class CHandleGuard
    {
        HANDLE h_;
        CHandleGuard(CHandleGuard&);
        CHandleGuard& operator=(CHandleGuard&);
    public:
        explicit CHandleGuard(HANDLE h=0)
            :h_(h){}
        ~CHandleGuard(void)
        {
            if(h_)CloseHandle(h_);
        }
        HANDLE get() const {return h_;}
        HANDLE release()
        {
            HANDLE tmp = h_;
            h_ = 0;
            return tmp;
        }
        void reset(HANDLE h)
        {
            if(h_)CloseHandle(h_);
            h_ = h;
        }
    };

    inline std::string FormatError(DWORD status, DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US) )
    {
        LPVOID lpMsgBuf;
        if (DWORD dwSize = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            status,
            dwLanguageId, // Default language
            (LPSTR) &lpMsgBuf,
            0,
            NULL ))
        {
            utils::CLocalAllocGuard guard( lpMsgBuf );
            if(dwSize < 2) return "";
            return std::string((char*)lpMsgBuf,(char*)lpMsgBuf+dwSize - 2);
        }
        else
        {
            return "Can't format message from error code";
        }
    }


    template<class wcharVectorType>
    inline void char2wstr(const char * str, size_t strSize, wcharVectorType * res, UINT cpId = CP_ACP)
    {
        if(!strSize)
            return;
        res->resize(strSize + 1);//it is enough in 99.99%
        while(1)
        {
            int size = MultiByteToWideChar(cpId, 0, str, (int)strSize, &res->front(), (int)res->size());
            if(size)
            {
                res->resize(size + 1);
                return;
            }
            DWORD error = ::GetLastError();
            if(error != ERROR_INSUFFICIENT_BUFFER)
                throw std::exception(utils::FormatError(error).c_str());
            size = MultiByteToWideChar(cpId, 0, str, (int)strSize, 0, 0);
            if(!size)
                throw std::exception(utils::FormatError(::GetLastError()).c_str());
            res->resize(size + 1);
        }
    }

    std::wstring string2wstring(const std::string &from);

    inline void char2wstr_unthrowable(const char * str, size_t strSize, std::vector<wchar_t> * res, UINT cpId = CP_ACP)
    {
        try
        {
            char2wstr(str, strSize, res, cpId);
        }
        catch(const std::exception& ){}//for capability
    }

	const char hex_chars[17] = "0123456789ABCDEF";

	template<class Type>
	std::string ToHexString(Type * pObject, int iCount = 1)
	{
		std::string str;
		str.reserve(sizeof(Type) * 2 * iCount);
		char* pEnd = (char*)pObject + sizeof(Type)*iCount;
		for (char * pData = (char*)pObject; pData != pEnd; ++pData)
		{
			str.push_back(hex_chars[((*pData) & 0xF0) >> 4]);
			str.push_back(hex_chars[(*pData) & 0x0F]);
		}
		return str;
	}

    inline
        void FromHexString(const std::string & str,
        char * pBegin,
        size_t size)
    {
        volatile utils::CStaticAssert<sizeof(unsigned char)==sizeof(char)> char_assert; &char_assert;
        size_t iCurrent = 0;
        size_t stringSize = str.size();
        if (!stringSize)
            return;

		if (stringSize % 2)
			throw std::exception("FromHexString.IncorrectInputSize");

        char * pEnd = pBegin + size;
        while(1)
        {
            if (iCurrent >= stringSize-1)
                break;

            if (pBegin >= pEnd)
                throw std::exception("BufferTooSmall");

            char buff[] = {str[iCurrent], str[iCurrent+1], 0};

            char * pEnd = 0;
            unsigned char ch = utils::convert<unsigned char, unsigned long>::from(strtoul(buff, &pEnd, 16));
            if ((buff + (sizeof(buff)/sizeof(buff[0])) -1) != pEnd)
                throw std::exception("InvalidBuffer");

            (unsigned char&)(*pBegin) = ch;
            ++pBegin;
            iCurrent+=2;
        }
    }

    inline
        void FromHexString(const std::string & str, std::vector<char> * pVec)
    {
        pVec->clear();
        if (str.empty())
            return;
		pVec->resize(str.size() / 2);
        FromHexString(str, utils::GetBeginOf(*pVec), pVec->size());
    }
}

#endif