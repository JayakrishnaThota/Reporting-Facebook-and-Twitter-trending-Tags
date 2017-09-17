/*Declaration of header files useful for this project*/
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits> 
#include <map> 
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <sstream>  
#include <string>
#include <utility> 
#include <vector> 

using namespace std;                     //Using standard namespace

stack <string> Stack_String;             //Stack for pushing and popping strings for hashtags
stack <int> Stack_Count;                 //Stack for pushing and popping integers for count values

typedef istringstream iss;               //Typedef's for giving short alias to complicated data types
typedef pair< int ,int > pii;
typedef long long ll;
typedef ostringstream oss;
typedef vector <int> vi;

/* Declaration of Macros used in this project */
#define tr(ctr, it) for(typeof(ctr.begin()) it = ctr.begin(); it != ctr.end(); it++)
#define rep(i,n) for(int i=0;i<n;i++)
#define cpresent(ctr, element) (find(all(ctr),element) != ctr.end()) 
#define fu(i,a,n) for(int i=a;i<=n;i++)
#define fd(i,n,a) for(int i=n;i>=a;i--)
#define all(a)  a.begin(),a.end()
#define pb push_back
#define mp make_pair
#define ln length()
#define dg(n,s); printf("%s %d",s,n)  
#define gi(n) scanf("%d",&n)
#define gl(n) cin >> n
#define ff first
#define sz(a) int((a).size()) 
#define ss second
#define imax numeric_limits<int>::max()
#define imin numeric_limits<int>::min()
#define ps printf(" ")
#define pn printf("\n")
#define lmax numeric_limits<ll>::max()
#define lmin numeric_limits<ll>::min()
#define pi(n) printf("%d",n)
#define pl(n) cout << n
#define present(ctr, element) (ctr.find(element) != ctr.end()) 

/* Structure of the Fibonacci node */
struct F_Node
{
	int count;                                     //Count value of the given hashtag
    int degree;                                    //Number of nodes in the next level
	bool Child_Cut;                                //Childcut which represents the childcut variable in the fibonacci heap
	string hashTag;                                //Variable to store the given hashtag value
    F_Node *Parent_Node;                           //Pointer to the given fibonacci node's parent  
	F_Node *Right_Sibling;                         //Pointer to the given fibonacci node's Right Sibling to maintain the doubly linked list
	F_Node *Left_Sibling;                          //Pointer to the given fibonacci node's Left Sibling to maintain the doubly linked list
	F_Node *Child_Node;                            //Pointer to the given fibonacci node's Child  
    F_Node(int hashCount, string hshTag)           //Cosntructor to initialise the node's values
	{
       	count = hashCount;                         //variable count is initialised to the given argument hashCount
		degree = 0;                                //Degree of the node is set to zero initially
        Child_Cut = false;		                   //Child Cut value is set to false
		hashTag = hshTag;                          //variable hashTag is initialised to the provided argument hshTag
		Parent_Node = NULL;                        //Parent pointer is set to Null
		Child_Node = NULL;                         //Child pointer is set to Null
		Right_Sibling = this;                      //Pointer is pointing to the node itself
		Left_Sibling = this;                       //Pointer is pointing to the node itself
	}
};

class F_Class                                           //Class in which all Max Fibonacci heap's operation are declared and defined  
{
	/* Default access specifier is private for class */
	    int Node_Count;                                 // Variable to represent the node's count
		F_Node *Max_Ptr;                                //Pointer which points to the node with maximum key
		map<string, F_Node*>hashTable;                  //STL declaration of Hash Table which stores the hashtag and pointer to the heap
		
	public:                                            //Making all the member functions public to access these from outside the class 
		F_Class()                                      //Constructor of F_Class to initialise the values of max ptr and count of the tag
		{
			Max_Ptr = NULL;
			Node_Count = 0;
		}
		/* Declaration of Member Functions of the Fibonacci Class */
            void Create_Fnode(int hash_cnt, string hshTag);
		    void Cmbine();
			void Cascading_Cut(F_Node *node);
            F_Node * remove_Fnode(F_Node * node);
			void increaseKey(int new_count, string h_Tag); 
            string getMax();
            int getMaxCount();
            int remove_Max();
};
          /* Method to Create Node */
          void F_Class::Create_Fnode(int hash_cnt, string hshTag)
	 	   {
			if(hashTable.find(hshTag) == hashTable.end())       //Conditional Operator to Check if the tag is in the hashtable or not 
			{                                                   // Instructions to follow if the hashtag is not in the hash table
				F_Node *newNode = new F_Node(hash_cnt, hshTag); //Create a new node for the given hashtag and count  
				if(!Max_Ptr)                                    // Conditional check to see if there is max pointer is pointing to Null or not 
				{                                               // Instructions to follow if the max pointer is null
					Max_Ptr = newNode;                          //Assign the maximum pointer to the new node if the max pointer is null
				}	
				else                                            // Instructions to follow if the max pointer is not null
				{
					if(!Max_Ptr && !newNode)                    //Assign the max pointer to null if there is no new node and max pointer pointing to anyone
					{
						Max_Ptr= NULL;
					}
					else if(!Max_Ptr)                            
					{
						Max_Ptr= newNode;
					}
					F_Node *Node_Swap;
					Node_Swap = Max_Ptr->Right_Sibling;              // Pointer changes to update the new node and max pointer
					Max_Ptr->Right_Sibling = newNode->Right_Sibling;
					newNode->Right_Sibling = Node_Swap;
					Max_Ptr->Right_Sibling->Left_Sibling = Max_Ptr;
					newNode->Right_Sibling->Left_Sibling = newNode;
					Max_Ptr= (Max_Ptr->count > newNode->count)? Max_Ptr: newNode;
				}
				hashTable[hshTag]= newNode;
				Node_Count++;
			}
			else                                                      // Instructions to follow if the hashtag is not in the hash table   
			{
				increaseKey(hash_cnt, hshTag);                        // Perform Increase Key operation on the given hashtag and increase by hshcount
			}
		}
		
		/* Method for pair wise combining */
          void F_Class::Cmbine()
		  {	
			if (Max_Ptr->Right_Sibling == Max_Ptr)                  //After remove max, if older max's right sibling is max, then return 
				{
				return;
       			}
			vector<F_Node *> toCheck;                               //Vector declaration 
			F_Node *curr = Max_Ptr;
			F_Node *start = curr;
			toCheck.push_back(curr);
			curr = curr->Right_Sibling;
			while(curr != start)
			{
				toCheck.push_back(curr);
				curr = curr->Right_Sibling;
			}

			vector<F_Node *> degrees (int(log2(Node_Count)) + 1);
			for (int i = 0; i < toCheck.size(); i++)
			{
				curr = toCheck[i];
				if (curr->degree >= degrees.size())
				{
					degrees.resize(curr->degree+1);	
				} 
				if (!degrees[curr->degree])
				{
					degrees[curr->degree] = curr;
					if (curr->count >= Max_Ptr->count)
					{
						Max_Ptr = curr;	
					} 
				}
				else
					{
					while (degrees[curr->degree])
					{					
						F_Node *min, *max;
						if (degrees[curr->degree]->count > curr->count)
						{
							min = degrees[curr->degree];
							max = curr;
						} 
						else 
						{
							min = curr;
							max = degrees[curr->degree];
						}
						F_Node * rtn = max->Right_Sibling;
						max->Right_Sibling->Left_Sibling = max->Left_Sibling;
						max->Left_Sibling->Right_Sibling = max->Right_Sibling;
						max->Right_Sibling = max;
						max->Left_Sibling = max;

						if(!min->Child_Node && !max)
						{
							min->Child_Node= NULL;
						}
						else if(!min->Child_Node)
						{
							min->Child_Node= max;
						}
						F_Node *Node_Swap;

						Node_Swap = min->Child_Node->Right_Sibling;
						min->Child_Node->Right_Sibling = max->Right_Sibling;
						max->Right_Sibling = Node_Swap;

						min->Child_Node->Right_Sibling->Left_Sibling = min->Child_Node;
						max->Right_Sibling->Left_Sibling = max;

						min->Child_Node= (min->Child_Node->count > max->count)? min->Child_Node: max;
						max->Parent_Node = min;
						min->degree++;
						
						curr= min;
						degrees[curr->degree - 1] = NULL;
						if(curr->count >= Max_Ptr->count)
						{
							 Max_Ptr = curr;
						}
						if (curr->degree >= degrees.size())
						{
							degrees.resize(curr->degree+1);	
						} 
					}
					
					degrees[curr->degree] = curr;
				}
			}
		}
		
		/*Method to update the childcuts of the fibonacci heap*/
		void F_Class::Cascading_Cut(F_Node *node)
		{
           	if (!node->Parent_Node)                              //If node doesn't have a parent
			{
				return;                                          //Return
			}
			node->Child_Cut =false;                              //Assign boolean value of child cut to be false
			F_Node * rtn = node->Right_Sibling;                  //Assign rtn pointer to node's right sibling 
			F_Node *Node_Swap;                                   //Declare an additional pointer to Fibonacci node
			if (node->Parent_Node->Child_Node ==node)            //Check to see if node has a right sibling or not
			{
				if (node->Right_Sibling !=node)
				{
					node->Parent_Node->Child_Node =node->Right_Sibling;	
				} 
				else
				{
					node->Parent_Node->Child_Node =NULL;		
				} 
			}
			node->Right_Sibling->Left_Sibling = node->Left_Sibling;         //Pointer Updates
			node->Left_Sibling->Right_Sibling = node->Right_Sibling;
			node->Right_Sibling = node;
			node->Left_Sibling = node;
			if(!Max_Ptr && !node)
			{
				Max_Ptr= NULL;
			}
			else if(!Max_Ptr)
			{
				Max_Ptr= node;
			}
			Node_Swap = Max_Ptr->Right_Sibling;
			Max_Ptr->Right_Sibling = node->Right_Sibling;
			node->Right_Sibling = Node_Swap;
			Max_Ptr->Right_Sibling->Left_Sibling = Max_Ptr;
			node->Right_Sibling->Left_Sibling = node;
			Max_Ptr=(Max_Ptr->count > node->count)? Max_Ptr: node;
			node->Parent_Node->degree--;
			if (node->Parent_Node->Child_Cut)                                  //Perform cascading cut as long as the encountered node childcut is true
			{
				Cascading_Cut(node->Parent_Node);	
			}
			else                                                               //Stop at first node which has child cut as False
			{
				node->Parent_Node->Child_Cut=true;                             //Assign the new child cut of this node as true 
			} 
			node->Parent_Node=NULL;
		}
		
		/*Method to remove a node from the Fibonacci heap*/
		F_Node * F_Class::remove_Fnode(F_Node * node)
		{
			F_Node * rtn = node->Right_Sibling;                          //Assign rtn pointer to the argument node's right sibling
			if (rtn == node)                                             //Condition to check if the pointer is pointing to itself incase of a single element  
			{
				return NULL;                                             //Return Null
			}
			node->Left_Sibling->Right_Sibling = node->Right_Sibling;     // Pointer changes to update the structure of heap after deletion
			node->Right_Sibling->Left_Sibling = node->Left_Sibling;
			node->Left_Sibling = node;
			node->Right_Sibling = node;
			return rtn;                                                  //return the updated pointer
		}
		
		/*Method to remove the maximum node from the max fibonacci heap */
          int F_Class::remove_Max()
		  {
			if(!Max_Ptr)                                                 //Check to see if the max ptr is null or not
			   {
				cout<<"The heap is Empty"<<endl;                         //If null, print the statement that the heap is empty
				return 0;                                                //return zero
			   } 
			F_Node *delNode = Max_Ptr;                               //assign rem_Node pointer to Max_ptr       
			Node_Count--;                                             //Decrease Node count value by one
			hashTable.erase(Max_Ptr->hashTag);                       //Remove the tag from hashtable
 			Max_Ptr = remove_Fnode(Max_Ptr);                         //Call remove function
			if(delNode -> Child_Node)                               //Check to see if the node has a child node
			{
				F_Node *tnode  = delNode->Child_Node;              //Pointer updates for the above condition  
				F_Node *start = tnode;
					tnode->Parent_Node = NULL;
					tnode = tnode->Right_Sibling;
				while(tnode != start)
				{
					tnode->Parent_Node = NULL;
					tnode = tnode->Right_Sibling;
				}
				if(!delNode->Child_Node && !Max_Ptr)              //If no child node, assign max pointer to null 
				{
					Max_Ptr= NULL;
				}
				else if(!Max_Ptr)
				{  
					Max_Ptr= delNode->Child_Node;                 //Else, make child node as max node
				}
				F_Node *Node_Swap;                                //Pointer Updates
				Node_Swap = delNode->Child_Node->Right_Sibling;
				delNode->Child_Node->Right_Sibling = Max_Ptr->Right_Sibling;
				Max_Ptr->Right_Sibling = Node_Swap;
				delNode->Child_Node->Right_Sibling->Left_Sibling = delNode->Child_Node;
				Max_Ptr->Right_Sibling->Left_Sibling = Max_Ptr;
				
				Max_Ptr= (delNode->Child_Node->count > Max_Ptr->count)? delNode->Child_Node: Max_Ptr;
			}
			if (!Max_Ptr)                                           //Check to see is null or not
				 return delNode->count;                             //Return the original max node's count
			Cmbine();		                                        //Combine Pair Wise function call
			return delNode->count;                                  //Return the original max node's count
		}

		/*Method to return the maximum pointer's hashtag*/
        string F_Class::getMax()
		{
			if(Max_Ptr)                           //Condition to check if Maxptr is pointing to a node or Null
			{
				return Max_Ptr->hashTag;          //Return the maxptr pointing node's hashtag
			}
			return "The heap is Empty";           //If maxptr is null, return the message that the heap is empty	
		}

		/*Method to return the maximum count value */
		int F_Class::getMaxCount()                
		{
			if(Max_Ptr)                           //Condition to check if Maxptr is pointing to a node or Null
			{
				return Max_Ptr->count;            //Return the maxptr pointing node's count
			}
			return 0;                             //If maxptr is null, return zero which is the maximum count in case of an empty heap
		}

/* Method to increase the count by new_count */
		void F_Class::increaseKey(int new_count, string h_Tag)
		{
			F_Node *curr = hashTable[h_Tag];                   //Assign a pointer to the tag's hashtable location
			curr->count = curr->count + new_count;             //Increase the count value by new_count value
			hashTable.erase(h_Tag);
			hashTable[h_Tag] = curr;
			
			if (curr->Parent_Node && (curr->Parent_Node->count < curr->count))
			{
					Cascading_Cut(curr);                       //Perform a Cascading Cut with curr as argument
			}
			if (curr->count > Max_Ptr->count)                  //Condition to check if the new increased value is the maximum value or not
			{
				Max_Ptr=curr;	                               //If the curr pointer's node count is higher, assign max ptr to curr
			} 
		}
		
/*Main method*/
int main(int argc, char* argv[])
{
	F_Class fibObj =  F_Class();       //Declaring an object of fibonacci class
	ifstream infile(argv[1]);          //Opening file stream for sample input file
	ofstream outfile;                  //File stream for output file       
	int count;                         //Variable for tag's frequency
	int type;
	int topN;                                     //Variable for 'n' top tags
	string line;
	string h_Tag;                               //String variable for hashtag
	outfile.open("output.txt", ios::app);       //command for Opening output file
	
	while (std::getline(infile, line))          //While loop for reading each line
	{
	    std::istringstream iss(line);
	    iss>>h_Tag;
	    if(h_Tag[0]=='#')                        //Conditional operator to check if the first character in the line is # 
		{
	    	iss>>count;                                                               //read the count value of the hashtag
	    	type=1;                                                                   //Specify this case as type 1
	    }
	    else if(h_Tag[0]!='#' && (h_Tag!="STOP" || h_Tag!="stop"))                      //Case when we encounter a number in the input line
		{
	    	 stringstream convert(h_Tag);
	    	 convert>>topN;
	    	 type=2;                                                                 //Specify this as type 2
	    }
	    else if(h_Tag=="STOP" || h_Tag=="stop")                                        //Conditional operator to see if we encounter stop in the line
		{       
	    	break;                                                                   //Break from the loop 
	    }
	    if(type==1)                                                                  //Condition for type 1
		{
		fibObj.Create_Fnode(count, h_Tag);                                              //Create a node with the given count and hashtag in the max fibonacci heap
	 	}
	 	else if(type==2)                                                             //Condition for type 2  
		{
	 		while(topN--)                                                            //loop for printing tags to output file 
			{
	 			if(topN!=0)                                                          //Not looking for the next tag
				{
	 				outfile<<fibObj.getMax().substr(1,fibObj.getMax().length())<<",";//<<fibObj.getMaxCount()<<","; //If not, write the tag and then look at the next tag
	 			}
	 			else
				{
	 				outfile<<fibObj.getMax().substr(1,fibObj.getMax().length());//	<<fibObj.getMaxCount(); //Not looking for the next tag
	 			}
	 			Stack_String.push(fibObj.getMax());                              //Push the max node's string into stack string 
	 			Stack_Count.push(fibObj.getMaxCount());                          //Push the max node's count value into stack count   
	 			fibObj.remove_Max();                                             //Remove the max node from the heap
	 		}                                                                   //removemax writing is ended here
	 		outfile<<endl;                                                      //write to output file

	 		while(Stack_String.size()>0)                                        //Condition to check if the stack size is positive
			{
	 			fibObj.Create_Fnode(Stack_Count.top(), Stack_String.top());     //create fibonacci nodes for hashtags written to output  
	 			Stack_String.pop();                                             //Push the hashtags
	 			Stack_Count.pop();                                              //Pop the hashtags 
	 		}
	 	}
	}
}
