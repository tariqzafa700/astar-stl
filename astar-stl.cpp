#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <memory>
#include <iterator>

using namespace std;

class Node {
  public:
    int x, y, f, g, h;
    shared_ptr<Node> parent;
    Node(int _x, int _y) : x(_x), y(_y), f(0), g(0), h(0) { }
    Node(int _x, int _y, int _f, int _g, int _h) : x(_x), y(_y), f(_f), g(_g), h(_h) { }
    friend bool operator==(const Node& a, const Node& b) {
      return a.x == b.x && a.y == b.y;
    }
    Node& operator=(const Node& a) {
      if (&a == this) {
        return *this;
      }
      this->x = a.x;
      this->y = a.y;
      this->f = a.f;
      this->g = a.g;
      this->h = a.h;
      this->parent = a.parent;
      return *this;
    }
    friend ostream& operator <<(ostream& out, const Node& node) {
      out << node.x << ", " << node.y << " f,g,h=" << node.f << ", " << node.g << ", " << node.h << endl;
      return out;
    }
};

auto comparer = [](const Node& a, const Node& b) {
  return a.f >= b.f;
};

int findHeuristicDistance(const Node& a, const Node& b) {
    return abs(a.x - b.x) + abs(a.y -b.y);
} 

bool enqueueNeighbour(const vector<vector<int>>& matrix, int sizeX, int sizeY, Node&& childNode, const Node& currNode, Node& endNode,
                      vector<Node>& openList, vector<Node>& closedList) {
  int cx = childNode.x, cy = childNode.y;
  
  if (cx >= 0 && cx < sizeX && cy >= 0 && cy < sizeY && matrix[cx][cy] != 0) {
    auto cItr = find(closedList.begin(), closedList.end(), childNode);
    if (cItr == closedList.end()) {
      auto oItr = find(openList.begin(), openList.end(), childNode);
      childNode.h = findHeuristicDistance(childNode, endNode);
      childNode.f = childNode.g + childNode.h;
      childNode.parent = make_shared<Node>(currNode);
      if (oItr == openList.end()) {
        openList.push_back(childNode);
        push_heap(openList.begin(), openList.end(), comparer);
      } else {
        if (oItr->f > childNode.f) {
          oItr->f = childNode.f;
          oItr->g = childNode.g;
          oItr->h = childNode.h;
          oItr->parent = make_shared<Node>(currNode);
        }
        make_heap(openList.begin(), openList.end(), comparer);
      }

      if (childNode == endNode) {
        endNode = childNode;
        return true;
      }
    }
  }
  return false;
}

vector<Node> astarDistance(const vector<vector<int>>& matrix, int sizeX, int sizeY, int startX, int startY, int endX, int endY) {
  vector<Node> openList, closedList;

  vector<pair<int,int>> neighbours{{0,1}, {1,0}, {-1,0}, {0,-1}};
  Node startNode(startX, startY);
  Node endNode(endX, endY);
  bool ended = false;

  openList.push_back(startNode);
  push_heap(openList.begin(), openList.end(), comparer);
  while (1) {
    pop_heap(openList.begin(), openList.end(), comparer);
    Node currNode = openList.back();
    openList.pop_back();
    
    for (auto& neighbour : neighbours) {
      ended = enqueueNeighbour(matrix, sizeX, sizeY, 
                       Node(currNode.x + neighbour.first, currNode.y + neighbour.second, 0, currNode.g + 1, 0), currNode, endNode, openList, closedList);
      if (ended)
        break;
    }
    
    if (openList.empty() || ended) {
      break;
    }
    closedList.push_back(currNode);
  }
  cout << "path:" << endl;
  vector<Node> path;
  if (ended && !openList.empty()) {
    shared_ptr<Node> iter = make_shared<Node>(endNode);
    while(iter) {
      path.push_back(*iter);
      iter = iter->parent;
    }
  }
  return path;
}

int main() {
  vector<vector<int>> matrix {
        
        { 1, 0, 1, 1, 1, 1, 0, 1, 1 }, 
        { 1, 1, 1, 0, 1, 1, 1, 0, 1 }, 
        { 1, 1, 1, 0, 1, 1, 0, 1, 1 }, 
        { 0, 0, 1, 0, 1, 1, 0, 1, 0 }, 
        { 1, 1, 1, 0, 1, 1, 1, 1, 1 }, 
        { 1, 0, 1, 1, 1, 1, 0, 1, 1 }, 
        { 1, 0, 0, 0, 0, 1, 0, 0, 0 }, 
        { 1, 0, 1, 1, 1, 1, 0, 1, 1 }, 
        { 1, 1, 1, 0, 0, 0, 1, 0, 0 } 

    };

    matrix = {
        
        { 1, 0, 1, 1, 1, 1, 0, 1, 1 }, 
        { 1, 1, 1, 0, 1, 1, 1, 0, 1 }, 
        { 1, 1, 1, 0, 1, 1, 0, 1, 1 }, 
        { 0, 0, 1, 0, 1, 1, 0, 0, 1 }, 
        { 1, 1, 1, 0, 1, 0, 1, 1, 1 }, 
        { 1, 0, 1, 1, 1, 1, 0, 1, 1 }, 
        { 1, 0, 0, 1, 0, 1, 1, 1, 0 }, 
        { 1, 0, 1, 1, 0, 1, 0, 1, 1 }, 
        { 1, 1, 1, 0, 0, 0, 1, 0, 0 } 

    };
    for (auto x:matrix) {
        for (auto y:x) {
            cout << y << " ";
        }
        cout << endl;
    }
  vector<Node> path = astarDistance(matrix, 9, 9, 8, 0, 0, 8);

  for (auto revId = path.rbegin(); revId != path.rend(); revId++) {
    cout << *revId;
  }
  cout << "min: " << path.front().f << endl;
  return 0;
}
