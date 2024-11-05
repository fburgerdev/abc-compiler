# Generics

1. **generic<**param1**:** _kind1_**,** _…_**,** _paramN_**:** _kindN_**>** _definition_

> A generic parameter is either a _type_, or a _primitive_
# Classes


generic<T: type, N: u64>
class array {
  init(value: const &T)
	  : data{value}
	
  func at(n: u64) mut -> mut &T {
	  return _data[n];
  }
  func at(n: u64) const -> const &T {
	  return _data[n];
  }
  private:
  \_\_seq<T, N> data;
}

func main() -> i32 {
  let arr := array<i32, 10>(1);
  for i in range(0, 10) {
	  arr.at(i) = i;
  }
  return 0;
}


# Functions


generic<T: type>
func swap(x1: &T, x2: &T) {
  let temp :T= move(x1);
  x1 = move(x2);
  x2 = move(temp);
}

func main() -> i32 {
  let str1 := "world";
  let str2 := "hello";
  swap(str1, str2);
  std::print(str1, " ", str2, "!");
  return 0;
}


# Variables


generic<T: type>
var staticList := std::list<T>();

func main() -> i32 {
  for i in range(0, 10) {
    staticList<i32>.push(i);
  }
  for ch in "hello world!" {
    staticList<char>.push(ch);
  }
  std::print(staticList<i32>.size()); // 10
  std::print(staticList<char>.size()); // 12
  return 0;
}
