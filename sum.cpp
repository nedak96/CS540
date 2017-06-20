#include <iostream>
#include <array>


// 70 points

/* BEGIN YOUR CODE */
double* gArr;
template<size_t N> class Index{
	public:
		int x = N;
		double operator+(double d){
			return gArr[x]+d;
		}
};

template<typename T, size_t U, typename V> double sum(std::array<T, U> arr, V cur){
	gArr = new double[arr.size()];
	for(int i=0; i<arr.size(); i++){
		gArr[i]=arr[i];
	}
	double d = cur+0;
	delete[] gArr;
	return d;
}

template<typename T, size_t U, typename V, typename... Types> double sum(std::array<T,U> arr, V cur, Types... args){
	gArr = new double[arr.size()];
	for(int i=0; i<arr.size(); i++){
		gArr[i]=arr[i];
	}
	double total = cur+0;
	delete[] gArr;
	total += sum(arr, args...);
	return total;
}

int
main() {
    std::array<int, 5> a{{1, 2, 3, 4, 5}};
    std::cout << sum(a, 1, 2) << std::endl;
    std::cout << sum(a, 1, 2, Index<2>()) << std::endl;
    std::cout << sum(a, Index<3>(), 1, 2, Index<2>()) << std::endl;
}
