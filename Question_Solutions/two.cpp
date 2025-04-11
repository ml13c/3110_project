/*c++ solution to question 2
Give me the route with the smallest number of connections from city “A” to city
“D” through city “B” and “C”. (the order of “B” and “C” is not important). Or
tell me there is no such a route
PSEUDOCODE:
first check if a path to d through b and c is even possible.(im guessing it doesnt matter if it goes as):
a g d f k c b d 
so long as it contains nodes c and b within the path to d.
then compare eligible paths and return smallest if any
given index of city a,b,c,d as &a,&b,&c,&d we can begin to search
int city_counter


*/
