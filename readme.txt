“I, Altin Gjonbalaj, have done this assignment completely on my own. I have not copied it, nor
have I given my solution to anyone else. I understand that if I am involved in plagiarism or
cheating I will have to sign an official form that I have cheated and that this form will be stored in
my official university record. I also understand that I will receive a grade of 0 for the involved
assignment for my first offense and that I will receive a grade of “F” for the course for any
additional offense.”i

COMPILATION INSTRUCTION
	This program uses a makefile
	make clean 	//to rm the execuable
	make main 	//to create the execuable
	make run 	//to run the execuable
Make run calls ./BestFirstSearch input.txt output.txt, this means that each time you would like to run a new test, you need to replace the items inside the input.txt file

DATA STRUCTURES USED
	std::vector for taking in the initial input and sorting it
	std::priority_queue to store created nodes and pop off the one with the highest bound until there are none left
	Node struct to hold data values for weight, profit, profit/weight, level, and a vector of parent nodes that I use to write the output nodes, it also has an operator in order to compare nodes for use in the priority queue

COMPUTATION TIME
	Time complexity for this program should be on average nlog(n), the use of priority queue to ignore infeasible solutions and iterate through most promising solutions first makes it very efficient. 
	
CLASSES AND THEIR INTERACTIONS
	int main 
		 -> parses input into vector<Node> called list
		 -> sorts list
		 -> creates an initial dummy node and pushes it into a priority queue called best
		 -> calles bestBranch to return a Node vector that contains the solution
		 -> writes the solution into output.txt
	
	vector<Node> bestBranch 
		 -> creates 2 nodes, one for if the current item in list is added to the queue and one if the current item is not
		 -> adds both nodes to the priority queue if they are legal// not overweight
		 -> keeps track of leaves and nodes using a parameter passed in by reference
		 -> calls getBound for each node created in order to return a bound
		 -> creates a node list for each node that keeps track of its parents for the pupose of returning them
		 -> has a variable poppedParent that is meant to keep track of the node with the highest value as they are popped out of the queue
		 -> returns popped parent's internal vector which holds all of the nodes used to get the solution of the 0/1 knapsack
	
	int getBound
		 -> takes in info about the node and the maxWeight and level
		 -> returns solution to greedy partial knapsack problem
