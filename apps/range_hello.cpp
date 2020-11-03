#include <range_interface.hpp>
#include <range_mine.hpp>

#include <iostream>
#include <set>
#include <vector>
int main() {
  using namespace std;
  using namespace handmade;
  // cout << "Hello\n";
  // cout << "Put point number\n";
  int num;
  cin >> num;
  interface_range<half_avl_tree<int>> range;
  for (int i = 0; i < num; i++) {
    int val;
    cin >> val;
    range.insert(val);
  }
  // cout << "put number of min/max\n";
  cin >> num;
  vector<int> results(num);
  for (int i = 0; i < num; ++i) {
    int min;
    int max;
    cin >> min >> max;
    results[i] = range.getDistance(min, max);
  }
  for (const auto &val : results) {
    cout << val << " ";
  }
  cout << "\n";
  return 0;
}
