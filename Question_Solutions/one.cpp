/*c++ code solution to question 1
I am in city “A”, can I fly to city “B” with less than x connections? Give me the
route with the smallest number of connections or tell me there is no such a route
PSEUDO CODE:
int connection_counter;
iterate through tree
first check if less than x connections or if route is even possible
then compare the paths that do qualify and find the one with min connections:
for each node
  node = keep track of nodes passed through
  connection_counter++;
if connection_counter++ > x(idk about equal to or not)
  return false
else
  return true(with route so all nodes visited)

*/

