/*
 * ProjectTwo.cpp
 *
 *  Created on: Oct 13, 2025
 *      Author: Leanna Walker
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
// holds all the course data info
struct Course{
	string courseNumber;
	string courseTitle;
	vector<string> prereqList;
};
//tree node structure
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	// create a node that already has a course in it
	Node(Course courseData) {
		course = courseData;
		left = nullptr;
		right = nullptr;
	}
};
// class that will hold the courses to organize
class BinarySearchTree {
private:
	// pointer starts the root of the tree
	Node* root;

	//go through every node in he tree
	void goThroughTree(Node* currentNode) {
		// if the node is not null
		if (currentNode != nullptr) {
			// go to left children nodes first
			goThroughTree(currentNode->left);
			// print course info
			cout << currentNode->course.courseNumber << ", " << currentNode->course.courseTitle << endl;
			// go to the right children nodes next
			goThroughTree(currentNode->right);
		}
	}

public:
	BinarySearchTree() {
		//begin with no courses in the tree
		root = nullptr;
	}

	// adding a new course to the tree
	void Insert(Course course) {
		// if the tree is empty , then this will be the root
		if (root == nullptr) {
			root = new Node(course);
		}
		else {
			// begin searching for where to insert new course at the root node
			Node* currentNode = root;
			while (true) {
				// check to see if new course is smaller, if yes, go left
				if (course.courseNumber < currentNode->course.courseNumber) {
					if (currentNode->left == nullptr) {
						currentNode->left = new Node(course);
						break;
					} else {
						currentNode = currentNode->left;
					}
				} else {
					// check if new node is larger, if yes, go right
					if (currentNode->right == nullptr) {
						currentNode->right = new Node(course);
						break;
					} else {
						currentNode = currentNode->right;
					}
				}
			}
		}
	}

	// Search function to find the course the user input
	Course Search(string courseNumber) {
		// begin search at the root node
		Node* currentNode = root;
		while (currentNode != nullptr) {
			// checks to see if this is the node we're searching for
			if (courseNumber == currentNode->course.courseNumber) {
				// if the node is found, return that course
				return currentNode->course;
				// if the search course is smaller, move to left child node
			} else if (courseNumber < currentNode->course.courseNumber) {
				currentNode = currentNode->left;
			} else {
				// if search course is larger, move to the right child node
				currentNode = currentNode->right;
			}
		}
		// if the course is not found
		Course emptyCourse;
		return emptyCourse;
	}

	// prints all courses
	void InOrder() {
		// starts at root node, and looks through all nodes in recursive traversal order
		goThroughTree(root);
	}
};

// load the courses from the file
void loadCourses(BinarySearchTree& tree) {
	string courseFilename;
	// prompt user for filename
	cout << "Enter filename: ";
	getline(cin, courseFilename);
	// prints to user which file it's reading
	cout << "Reading file: " << courseFilename << endl;

	// attempt to open file
	ifstream file(courseFilename);

	if (file.is_open()) {
		string line;
		// read the file line by line
		while (getline(file, line)) {
			// find where the commas are
			size_t whereCommaIs = line.find(',');
			if (whereCommaIs != string::npos) {
				// get the courseNumber, which is everything before the comma
				string courseNumber = line.substr(0, whereCommaIs);
				// get rest of the line after the comma
				string restOfLine = line.substr(whereCommaIs + 1);
				// make a new course
				Course newCourse;
				newCourse.courseNumber = courseNumber;

				// breaks apart the line using commas
				stringstream commaSplitter(restOfLine);
				string piece;
				// make a list to store the pieces
				vector<string> pieceList;

				while (getline(commaSplitter, piece, ',')) {
					pieceList.push_back(piece);
				}

				if (pieceList.size() > 0) {
					// make the first piece the course title
					newCourse.courseTitle = pieceList[0];

					// start loop after the title, and through the rest of the pieces
					for (size_t i = 1; i < pieceList.size(); i++) {
						// check if the current piece is empty or not
						if (!pieceList[i].empty()) {
							// adds pieces to prereq list if they are not empty
							newCourse.prereqList.push_back(pieceList[i]);
						}
					}
				}
				// insert new course into the tree
				tree.Insert(newCourse);
			}
		}
		file.close();
		cout << endl;
	} else {
		cout << "Error: Could not open file." << endl;
		cout << endl;
	}
}

// print the course list
void printCourseList(BinarySearchTree& tree) {
	cout << "Here is a sample schedule:" << endl;
	cout << endl;
	tree.InOrder();
	cout << endl;
}

int main() {
	// program displays welcome message to user
	cout << "Welcome to the course planner." << endl;
	// Variable that will hold a number for the user input
	int choice = 0;

	BinarySearchTree abcTree;
	// while loop for main menus
	while (choice != 9) {
		// main menu options displayed to user
		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit" << endl;
		cout << "What would you like to do? ";
		// user input
		cin >> choice;
		cin.ignore();
		//if user selects an invalid menu option
		if (choice != 1 && choice != 2 && choice != 3 && choice != 9) {
			// displays error message to user
			cout << choice << " is not a valid option." << endl;
			cout << endl;
		}
		// handles user menu choices
		if (choice == 1) {
			loadCourses(abcTree);
		}
		else if (choice == 2) {
			printCourseList(abcTree);
		}
		else if (choice == 3) {
			string courseNumber;
			// prompt user for course info
			cout << "What course do you want to know about? " << endl;
			cin >> courseNumber;
			cin.ignore();

			// make the search not case sensitive
			for (char &c : courseNumber) {
				c = toupper(c);
			}

			// use binary search tree to look for course
			Course foundCourse = abcTree.Search(courseNumber);
			if (foundCourse.courseNumber != "") {
				cout << foundCourse.courseNumber << ", " << foundCourse.courseTitle << endl;
				// check to see if the prereq list is empty
				if (!foundCourse.prereqList.empty()) {
					cout << "Prerequisites: ";
					for (size_t i = 0; i < foundCourse.prereqList.size(); i++) {
						// print prereq course number
						cout << foundCourse.prereqList[i];
						// look to see if this is the last prereq in the list
						if (i != foundCourse.prereqList.size() - 1) {
							// if its not the last, print comma to separate them
							cout << ", ";
						}
					}
					cout << endl;
				}
			}
			cout << endl;
		}
		else if (choice == 9) {
			cout << "Thank you for using the course planner!" << endl;
		}
	}
	return 0;
}



