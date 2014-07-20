#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

template<class T>
class Buffer: public Lock
{
    public:
		inline T & operator[](const int iIndex)
		{
		    return vBuffer[iIndex];
		}
        inline void PushBack(T ele)
        {
            vBuffer.push_back(ele);
        }
        inline void Erase(int iIndex)
        {
            vBuffer.erase(vBuffer.begin() + iIndex);
        }
        inline T PopFront()
        {
            typename std::vector<T>::iterator itr = vBuffer.begin();
            T ele = (*itr);
            vBuffer.erase(itr);
            return ele;
        }
        inline bool IsEmpty()
        {
            return vBuffer.begin() == vBuffer.end();
        }
        inline size_t Size()
        {
            return vBuffer.size();
        }
        inline void Dump()
        {
            std::stringstream stream;
            typename std::vector<T>::iterator itr = vBuffer.begin();
            for(;itr != vBuffer.end(); ++itr)
            {
                char ch[32] = {0};
                snprintf(ch, 32, "0x%x", (int)*itr);
                stream<<std::endl<<ch;
            }
            std::cerr<<std::endl<<"#### Buffer Dump ####";
            std::cerr<<stream.str();
            std::cerr<<std::endl<<"#####################";
        }
    private:
        std::vector<T> vBuffer;
};


#endif

