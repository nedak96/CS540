#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

#include <cstddef>
#include <typeinfo>
#include <mutex>

namespace cs540{
	std::mutex m;
	class DeleterB {
		public:
	 		virtual ~DeleterB() {}
	};
	template <typename T> class Deleter : public DeleterB {
		public:
			Deleter(T* p) : obj(p) {}
			T* obj;
			~Deleter(){
				delete obj;
			}
	};
	template<typename T> class SharedPtr{
		public:
			typedef T element_type;
			SharedPtr() : px(nullptr), pn(nullptr), d(nullptr){ }
			template<typename U> explicit SharedPtr(U* obj){
				if(obj != nullptr){
					pn = new long(1);
					px = obj;
					d = new Deleter<U>(obj);
				}
				else{
					pn = nullptr;
					px = nullptr;
					d = nullptr;
				}
			}
			SharedPtr(const SharedPtr &p){
				std::lock_guard<std::mutex> l(m);
				pn = p.pn;
				px = p.px;
				d = p.d;
				if (pn != nullptr) (*(pn))++;
			}
			template <typename U> SharedPtr(const SharedPtr<U> &p){
				std::lock_guard<std::mutex> l(m);
				pn = p.pn;
				px = p.px;
				d = p.d;
				if (p.pn != nullptr) (*(pn))++;
			}
			SharedPtr(SharedPtr &&p){
				std::lock_guard<std::mutex> l(m);
				pn = p.pn;
				px = p.px;
				d = p.d;
				p.pn = nullptr;
				p.px = nullptr;
				p.d = nullptr;
			}
			template <typename U> SharedPtr(SharedPtr<U> &&p){
				std::lock_guard<std::mutex> l(m);
				pn = p.pn;
				px = p.px;
				d = p.d;
				p.pn = nullptr;
				p.px = nullptr;
				p.d = nullptr;
			}
			SharedPtr &operator=(const SharedPtr &p){
				std::lock_guard<std::mutex> l(m);
				if(this != &p){
					if(pn != nullptr){
						(*pn)--;
						if(*pn == 0){
							delete d;
							delete pn;
						}
					}
					d = p.d;
					pn = p.pn;
					if(pn != nullptr) (*(pn))++;
					px = p.px;
				}
				return *this;
			}
			template <typename U> SharedPtr<T> &operator=(const SharedPtr<U> &p){
				std::lock_guard<std::mutex> l(m);
				if(pn != nullptr){
					(*pn)--;
					if(*pn == 0){
						delete d;
						delete pn;
					}
				}
				d = p.d;
				pn = p.pn;
				if(pn != nullptr) (*(pn))++;
				px = p.px;
				return *this;
			}
			SharedPtr &operator=(SharedPtr &&p){
				std::lock_guard<std::mutex> l(m);
				px = p.px;
				pn = p.pn;
				d = p.d;
				p.pn = nullptr;
				p.px = nullptr;
				p.d = nullptr;
				return *this;
			}
			template <typename U> SharedPtr &operator=(SharedPtr<U> &&p){
				std::lock_guard<std::mutex> l(m);
				px = p.px;
				pn = p.pn;
				d = p.d;;
				p.pn = nullptr;
				p.px = nullptr;
				p.d = nullptr;
				return *this;
			}
			~SharedPtr(){
				std::lock_guard<std::mutex> l(m);
				if(pn != nullptr){
					(*pn)--;
					if(*pn == 0){
						delete d;
						delete pn;
					}
					pn = nullptr;
					d = nullptr;
					px = nullptr;
				}
			}
			template <typename U> SharedPtr(const SharedPtr<U> &p, T* obj){
				std::lock_guard<std::mutex> l(m);
				if(obj != nullptr){
					pn = p.pn;
					(*(pn))++;
					px = obj;
					d = p.d;
				}
				else{
					pn = nullptr;
					px = nullptr;
					d = nullptr;
				}
			}
			void reset(){
				std::lock_guard<std::mutex> l(m);
				if(pn != nullptr){
					(*pn)--;
					if(*pn == 0){
						delete d;
						delete pn;
					}
					pn = nullptr;
					d = nullptr;
					px = nullptr;
				}
			}
			template <typename U> void reset(U *obj){
				std::lock_guard<std::mutex> l(m);
				if(pn != nullptr){
					(*pn)--;
					if(*pn == 0){
						delete d;
						delete pn;
					}
				}
				if(obj != nullptr){
					d = new Deleter<U>(obj);
					px = obj;
					pn = new long(1);
				}
				else{
					d = nullptr;
					px = nullptr;
					pn = nullptr;
				}
			}
			T* get() const{
				std::lock_guard<std::mutex> l(m);
				return px;
			}
			T& operator*() const{
				std::lock_guard<std::mutex> l(m);
				return *px;
			}
			T* operator->() const{
				std::lock_guard<std::mutex> l(m);
				return px;
			}
			explicit operator bool() const{
				std::lock_guard<std::mutex> l(m);
				if(pn == nullptr) return false;
				return true;
			}
		private:
			template<typename U>friend class SharedPtr;
			T* px;
			long* pn;
    			DeleterB* d;
	};

	template <typename T1, typename T2> bool operator==(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2){
		return(p1.get()==p2.get());
	}
	template <typename T> bool operator==(const SharedPtr<T> &p, std::nullptr_t){
		return(p.get()==nullptr);
	}
	template <typename T> bool operator==(std::nullptr_t, const SharedPtr<T> &p){
		return(p.get()==nullptr);
	}
	template <typename T1, typename T2>bool operator!=(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2){
		return(p1.get()!=p2.get());
	}
	template <typename T> bool operator!=(const SharedPtr<T> &p, std::nullptr_t){
		return(p.get()!=nullptr);
	}
	template <typename T> bool operator!=(std::nullptr_t, const SharedPtr<T> &p){
		return(p.get()!=nullptr);
	}
	template <typename T, typename U> SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp){
		return SharedPtr<T>(sp, static_cast<typename SharedPtr<T>::element_type*>(sp.get()));
	}
	template <typename T, typename U> SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp){
	    	return SharedPtr<T>(sp, dynamic_cast<typename SharedPtr<T>::element_type*>(sp.get()));
	}
}
#endif
