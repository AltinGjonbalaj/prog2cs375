#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

/*
	-Struct to hold all the values parsed in and other important values
	-bound holds the result calculated by getBound() for the node
	-weight and profit are parsed in from input.txt by main
	-level corresponds to the level in the tree the node would be
	-f is used for compareNode to sort the vector
	-parents holds a list of nodes, including itself, to rebuild the optimal solution
	-operator sets up the comparison for the priority queue

*/
struct Node{
	int bound;
	int weight;
	int profit;
	int level;

	float f;
	vector<Node> parents;
	bool operator <(const Node n) const{return (bound < n.bound);}
};
bool compareNode(Node i, Node j) {return (i.f > j.f);}


/*
	-returns the bound given a node's level, current weight, current profit, max weight, and the sorted vector lf inputs
	-list is the sorted list parsed in from input.txt
	-level corresponds to the level of the node in the tree
	-maxWeight corresponds to the maximum weight parsed into the function
	-currentWeight is the weight the node has accumulated
	-current profit is the profit the node accumulated until this function
	
	-this function first loops through until no more whole nodes can be added due to weight constraints
	-it then takes a float to represent the fraction of the next node it can add to the weight
	-it then returns the bound
*/
int getBound(vector<Node> list, int level, int maxWeight, int currentWeight, int currentProfit){
	if(currentWeight > maxWeight) return -1;
	int i;		//used to parse list
	for(i = level; i < list.size(); i++){
		if(currentWeight + list[i].weight < maxWeight){
			currentProfit += list[i].profit;
			currentWeight += list[i].weight;
		}
		else
			break;
	}

	if(i < list.size()){
		float f = (float)(maxWeight - currentWeight)/(float)list[i].weight;
		currentProfit += (int)(f * list[i].profit);
	}
	return currentProfit;
}


//THIS IS A METHOD I USED FOR DEBUGGING, UNIMPORTANT OTHERWISE
void printq(priority_queue<Node> q){
	priority_queue<Node> pq = q;
	while(!pq.empty()){
		cout << '\t' << pq.top().profit << endl;
		pq.pop();	
	}
	cout << '\n';
}


/*
	-creates 2 nodes, one for if the current item in list is added to the queue and one if the current item is not
	-adds both nodes to the priority queue if they are legal// not overweight
	-keeps track of leaves and nodes using a parameter passed in by reference
	-calls getBound for each node created in order to return a bound
	-creates a node list for each node that keeps track of its parents for the pupose of returning them
	-has a variable poppedParent that is meant to keep track of the node with the highest value as they are popped out of the queue
	-returns popped parent's internal vector which holds all of the nodes used to get the solution of the 0/1 knapsack

	-best is the priority queue used to maintain a max heap
	-list is the sorted list parsed in by input.txt
	-totalNodes corresponds to the total number of nodes created by the function// in the graph
	-totalLeaves corresponds to the total number of leaves created by the function// in the graph
	-maxWeight corresponds to the maximum weight parsed in from input.txt
*/
vector<Node> bestBranch(priority_queue<Node> best, vector<Node> list, int *totalNodes, int *totalLeaves, int maxWeight){
	Node poppedParent; //used to hold the optimal node as mentioned previously
	poppedParent.profit = -1;

	while(!best.empty()){
		if(best.top().bound > poppedParent.profit){	
	
		Node n1; // NODE THAT INCLUDES ITEM @ CURRENT LEVEL
		n1.level = best.top().level + 1;
		n1.weight = best.top().weight + list[n1.level - 1].weight; 
		n1.profit = best.top().profit + list[n1.level - 1].profit;
		if(n1.level - 1 < list.size()){
			n1.bound = getBound(list, n1.level, maxWeight, n1.weight, n1.profit);
			if(n1.bound == -1){ 
				n1.bound = n1.profit;
				*totalLeaves = *totalLeaves + 1;
				*totalNodes = *totalNodes + 1;
			}
		}
		n1.parents = best.top().parents;
		if(n1.level - 1 < list.size())
			n1.parents.push_back(list[n1.level - 1]);
	
	
		Node n2; // NODE THAT DOESNT INCLUDE ITEM @ CURRENT LEVEL
		n2.level = best.top().level + 1;
		n2.weight = best.top().weight; 
		n2.profit = best.top().profit;
		if(n2.level - 1 < list.size()){
			n2.bound = getBound(list, n2.level, maxWeight, n2.weight, n2.profit);
			if(n2.bound == -1){
				n2.bound = n2.profit;
			}
		}
		if(n2.level - 1 < list.size())
			n2.parents = best.top().parents;
	
	
		if(n1.weight < maxWeight && n1.level - 1 < list.size()){
			best.push(n1);
			*totalNodes = *totalNodes + 1;
		}
		if(n2.weight < maxWeight && n2.level - 1 < list.size()){
			best.push(n2);
			*totalNodes = *totalNodes + 1;
		}
		}

		if(best.top().bound < poppedParent.profit){
			*totalLeaves = *totalLeaves + 1;
		}
		if(best.top().profit >= poppedParent.profit && best.top().weight != 0)
			poppedParent = best.top();
		best.pop();
	}
	
	return poppedParent.parents;
}

/*
 	- parses input into vector<Node> called list
        - sorts list
        - creates an initial dummy node and pushes it into a priority queue called best
        - calles bestBranch to return a Node vector that contains the solution
	- writes the solution into output.txt
	
			
*/

int main(int argc, char* argv[]){
	if(argc > 3)
		cout << "Wrong amount of args" << endl;

	ofstream writeFile;	//THE FILE TO WRITE TO (OUTPUT)
	writeFile.open(argv[2]);

	ifstream readFile;	//THE FILE TO READ FROM (INPUT)
	readFile.open(argv[1]);
	int maxWeight;		//HOLDS MAXIMUM WEIGHT, PARSED FROM INPUL
	int totalItems;		//HOLDS TOTAL ITEMS, PARSED FROM INPUT
	string s;
	
	vector<Node> list;	//LIST USED TO HOLD PARSED DATA
	while(getline(readFile, s)){
		string delim = ",";	//DELIMITER USED FOR PARSING
		string weight = s.substr(0, s.find(delim));	//WEIGHT OF EACH INPUT
		string profit = s.substr(s.find(delim) + 1);	//PROFIT OF EACH INPUT
		
		Node n;			//NODE TO PUSH INTO LIST
		n.weight = stoi(weight);
		n.profit = stoi(profit);
		n.f = (float)n.profit/(float)n.weight;
	
		list.push_back(n);
	}	
	
	maxWeight = list[0].profit;
	totalItems = list[0].weight;
	list.erase(list.begin());
	sort(list.begin(), list.end(), compareNode);
	priority_queue<Node> best;
	Node start;		//STARTING NODE USED TO KICK OFF THE BESTBRANCH FUNCTION
	start.weight = 0; start.profit = 0; start.level = 0; 
	start.bound = getBound(list, start.level, maxWeight, 0, 0);
	best.push(start);
	
	int totalNodes = 1; int totalLeaves = 1;
	vector<Node> finalList = bestBranch(best, list, &totalNodes, &totalLeaves, maxWeight);	//CALLS BEST BRANCH TO GET A LIST OF NODES IN OPTIMAL SOLUTION

	string ret2;		//HOLDS EACH OPTIMAL SOLUTION LIST OF NODES TO WRITE IN OUTPUT FILE
	int totalProfit = 0;
	for(auto each : finalList){
		totalProfit += each.profit;
		ret2 += to_string(each.weight);
		ret2 += ',';
		ret2 += to_string(each.profit);
		ret2 += "\n";
	}
	
	string ret1 = to_string(totalItems) + ',' + to_string(totalProfit) + ',' + to_string(finalList.size()) + '\n';		//HOLDS FIRST LIKE OF OUTPUT FILE
	string ret3 = to_string(totalNodes) + ',' + to_string(totalLeaves) + '\n';	//HOLDS SECOND LINE OF OUTPUT FILE
	
	writeFile << ret1 << ret3 << ret2;

	readFile.close(); writeFile.close();
	return 0;
}
