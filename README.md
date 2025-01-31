# Eight Puzzle Solver

## Introduction

The Eight Puzzle is a type of sliding tile puzzle that involves a 3x3 grid of squares that are numbered and has one square that is empty. Initially, these numbers are scrambled in random order on the grid. The player's goal is to order these numbers using the fewest moves possible.

For this project, I implemented an Eight Puzzle solver using C++. Using this programming langauge helped me learn about its advanced capabilties as well as increase my mastery of the lanaguage. Within this project, I implemneted three unique algorithms that are capable of solving this type of puzzle. These include Uniform Cost Search, A* with a Misplaced Tile Heuristic, and A* with a Manhattan Distance Heuristic. The overall performance of these algorithms is evaluated and shown below.

Throughout this project, I consulted the following resources:

1. Dr. Eamonn Keogh's Lecture slides on Blind Search and Heursitic Search

    - [a. 2__Blind Search_part1.pptx](https://www.dropbox.com/sh/rltooq0t3khobuj/AAA3MYkZc8gb1RLa3tNSnsrga?dl=0&preview=2__Blind+Search_part1.pptx)
	- [b. 2__Blind Search_part2.pptx](https://www.dropbox.com/sh/rltooq0t3khobuj/AAA3MYkZc8gb1RLa3tNSnsrga?dl=0&preview=2__Blind+Search_part2.pptx)
   	- [c. 3__Heuristic Search.pptx](https://www.dropbox.com/sh/rltooq0t3khobuj/AAA3MYkZc8gb1RLa3tNSnsrga?dl=0&preview=3__Heuristic+Search.pptx)
	- [d. Eight-Puzzle_briefing_and_review_of_search.pptx](https://www.dropbox.com/sh/rltooq0t3khobuj/AAA3MYkZc8gb1RLa3tNSnsrga?dl=0&preview=Eight-Puzzle_briefing_and_review_of_search.pptx)

2. C++11 Documentation 

    - [a. cplusplus.com](https://cplusplus.com/doc/)

3. The following are libraries and predefined subroutines I included in my program to help with node manipulation:
    - The Queue Library to access the priority_queue data type
    - The Copy Constructor Syntax in order to perform Deep Copy on nodes
    - The operator() Overload Syntax in order to compare f(n) costs between nodes to correctly input them into the priority_queue
    - The operator= Overload Syntax to assign member variables from one node to another


## The Search Algorithms

### Uniform Cost Search

Uniform Cost Search, also known simply as UCS, is a type of Blind Search Algorithm. As explained by Dr. Keogh in his Blind Search PowerPoint slides, this algorithm expands the cheapest node. This is done by enqueueing the nodes in order of cumulative cost. For this algorithm, the g(n) is simply the cost to get to state n from the initial state. Uniform Cost Search is complete and optimal only if the “path cost is a nondecreasing function of depth.” As explained in the Project 1 requirements, UCS is just A* with h(n) hardcoded to zero. Its Time and Space Complexities are both O(bd), where b is the average branching factor and d is the depth of the tree. 


