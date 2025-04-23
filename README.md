# 3110_project
In this Project we are tasked with designing and proving a solution to the following questions:

    1. I am in city “A”, can I fly to city  “B”  with less than x connections? Give me the 
    route with the smallest number of connections or tell me there is no such a route. 
    2. Give me the route with the smallest number of connections from city “A” to city 
    “D” through city “B” and “C”. (the order of  “B” and “C” is not important). Or 
    tell me there is no such a route 
    3. I want to start from city “A”, visit all the cities that can be reached and then come 
    back to “A” using as less connections as possible. Give me a route or tell me there 
    is no such a route. (note: once you come back to “A”, you do not go out 
    anymore). 
    4. I am in city “A”, my friend John is in a different city “B”, and my other friend 
    Ann is in yet another different city “C”. We want to find a city different from the 
    three cities we are in to meet so that the total number of connections among three 
    of us is minimized. Tell me the city we should fly to and the routes for us or tell 
    me there is no such a city. 

RUNNING AND COMPILING INSTRUCTIONS:
    An important note when compiling is to make sure you are using g++ or any up to date compiler should work. However, if you are running this on a cell machine make sure the climit library is included as this was an issue we discovered when creating this project. We have included this library but in the case of any future updates it is important to note that this was built and tested mainly on a windows operating system through vs code.
    Before attempting to run it is important to ensure that the path of the flights, in our case flight.txt, is in the same directory as the main file. If the file name is different refer to the main function and change the filename variable to match your file. If you think there are any possible issues, run the main file and press option 5 to print out the graph. This graph, if any graph is generated, will be useful to ensure that input is correct as it is case sensitive. If no graph is generated then there is an issue with the input file so make sure it is in a txt format and formatted in a From-To matter similar to our file(flight.txt). 
    When inputting city names it is important to have the names of the cities match exactly as in the flight.txt is setup. So for our txt file, our code is based on the format of (City, Country) where all spaces and characters are the same as in the file. Again, if you wish to see how the graph was build using the provided Cities and Countries refer to our built ui and press option 5 to print the graph.

Creating the Graph
    To create our graph we created a function. It takes in the filename that is in the same directory and then begins to parse through it. It parses and creates the paths by looking at 'From:' and 'To:' as key words. One issue that occured when parsing through the flight paths was not reading the node included in the same line as 'From:' or 'To:'. In order to prevent any issues it would be best to keep a city right after these ase we found a work around to it. Once the graph is created we can then use it to search for our airline routes. This graph can also be printed as implemented by the printGraph feature that prints the graph back into a readable format. This was particularly useful in providing test input and debugging as we came to learn how case sensitive the input can be.

Building the main function and UI:
    We wanted to create a straight to the point UI. To do this we implemented a simple switch case in our main functoin and requested input where needed depending on which task was asked to complete. As an extra option we included option 5 for us when debugging. Considering how the testing was not just going to consists of one flight.txt file we decided to keep it in order to make it easier for the user to input their own flight.txt file. Depending on the selection of 1-4 it answered a different question that we were tasked. 
Solution to Question 1:

    In question 1 we were asked if it was possible to fly between two cities with less than x connections between them. To do this we received input asking the two desired cities and the maximum amount of connections. After this we sent it to a function to find if it was possible to do this under x amount of connections. This function is called findShortestRoute and accepts these 3 parameters. Using our variable distance and a stack, path, we can keep a counter of the amount of connections to ensure it is less than x connections. This same stack is not just used to count, but for popping and printing our cities to provide us a route less than or equal to x connections. 
Solution to Question 2:

    Our next tasks is to provide a route that goes from A-Z that goes through cities, 'H' and 'I'. It doesnt matter what order it is so long as H and I are on route to Z from A. To achieve this first we must check if A-Z is even possible. If it is then we need to find a path that contains our two desired cities. To do this we used a queue and logic with the stack similar to question 1 to print out the path. To find the actual path however, we store our 
Solution to Question 3:

    The third task requires us to find as many nodes as we can visit in a single path and return back our starting position only once.
Solution to Question 4:

    Our last task involves finding the best city(if possible) for 3 people to meetup in. These routes should be the ones with the least amount of connections or distance travelled. In order to ensure that we would find the best path we needeed a way to check the paths for each starint point. To do this we implemented the bfs algorithm. From here we used this on each statring point and compared 

