# cppmemo
A C++ template class Memo that provides memoization functionality for any class



# Usage


```

#include <string>
#include <iostream>
#include <random>

int getRandom(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    return dis(gen);
}

class Dummy{
    public:
      std::string doSome(std::string in){
          return in + std::to_string(getRandom());
      }
      
      int doSomeInt(int in, int in2){
          return in + getRandom() + in2;
      }

      int doSomeOnYourOwn(){
          return getRandom();
      }
};


using namespace std;

int main() {
  Dummy fp;
  Memo<Dummy> memo(fp);

  // Test 1: Memoization works for Dummy::doSome
  auto result = memo.dispatch(&Dummy::doSome, std::string("input_string"));
  auto result2 = memo.dispatch(&Dummy::doSome, std::string("input_string"));
  assert(result == result2);

  // Test 2: Memoization works for Dummy::doSomeInt
  auto result3 = memo.dispatch(&Dummy::doSomeInt, 1,2);
  auto result4 = memo.dispatch(&Dummy::doSomeInt, 1,2);
  assert(result3 == result4);

  // Test 3: Memoization works for Dummy::doSomeOnYourOwn
  auto result5 = memo.dispatch(&Dummy::doSomeOnYourOwn);
  auto result6 = memo.dispatch(&Dummy::doSomeOnYourOwn);
  assert(result5 == result6);

  // Test 4: Memoization does not work for different arguments
  auto result7 = memo.dispatch(&Dummy::doSome, std::string("input_string"));
  auto result8 = memo.dispatch(&Dummy::doSome, std::string("input_string2"));
  assert(result7 != result8);

  cout << "All tests passed!" << endl;

  return 0;
}
```
