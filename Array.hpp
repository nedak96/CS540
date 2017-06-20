#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "MyInt.hpp"
#include <vector>
#include <utility>
#include <iostream>
#include <chrono>

namespace cs540{
	class Array{
		public:
			Array(std::initializer_list<int> list){
				vec.insert(vec.end(), list.begin(), list.end());
			}
			Array(const Array& arr){
				for(int i=0; i<arr.vec.size(); i++){
					vec.push_back(MyInt());
					vec[i] = arr.vec[i];
				}
			}
			Array(Array &&arr){
				vec = std::move(arr.vec);
			}
			Array& operator=(const Array& arr){
				vec.clear();
				for(int i=0; i<arr.vec.size(); i++){
					vec.push_back(MyInt());
					vec[i] = arr.vec[i];
				}
			}
			Array& operator=(Array &&arr){
				vec.clear();
				vec = std::move(arr.vec);
			}
			~Array(){}
			static void move_performance_test(){
				Array a1{0,1,2,3,4,5,6,7,8,9,1,2,3,4,5,4,3,2,1,2,3,4,5,6,7};
				auto start = std::chrono::system_clock::now();
				Array a2{a1};
				auto send1 = std::chrono::system_clock::now();
				Array a3{std::move(a1)};
				auto send2 = std::chrono::system_clock::now();
				Array a4 = a3;
				auto send3 = std::chrono::system_clock::now();
				Array a5 = std::move(a3);
				auto end = std::chrono::system_clock::now();
				if((send1-start).count()<(send2-send1).count() || (send3-send2).count()<(end-send3).count()) std::cerr<<"Error: Not faster"<<std::endl;
			}
			friend std::ostream& operator<<(std::ostream& os, const Array& arr){
				for(int i=0; i<arr.vec.size(); i++){
					os<<arr.vec[i];
					if(i != arr.vec.size()-1) os<<", ";
				}
				return os;
			}
		private:
			std::vector<MyInt> vec;
	};
}
#endif
