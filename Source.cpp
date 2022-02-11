#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

vector<int> jugsCapacity;
vector<int> jugStates;
vector<vector<int>> closed;
int capacity;
int steps = 0;
int currCap;

struct StateSpaces
{
	vector<int> stateSpace;
	double h;

	StateSpaces(vector<int> ss, double hv) :stateSpace(ss), h(hv) {}
};

struct CompareStateSpaces {
	bool operator()(StateSpaces const& ss1, StateSpaces const& ss2)
	{
		return ss1.h > ss2.h;
	}
};

double hFunc(int cap, int jugCap, int goal = capacity)
{
	return abs((jugCap + cap) - goal);
}

vector<int> Action(vector<int> States, int jugIndex, int pouredJugIndex, int mode)
{
	//fill the jug -> mode=0
	if (mode == 0)
	{
		States[jugIndex] = jugsCapacity[jugIndex];
	}

	//pour to another jug -> mode=2
	if (mode == 1)
	{
		int overflow = 0;
		States[pouredJugIndex] += States[jugIndex];
		if (States[pouredJugIndex] > jugsCapacity[pouredJugIndex])
		{
			overflow = States[pouredJugIndex] - jugsCapacity[pouredJugIndex];
			States[pouredJugIndex] = jugsCapacity[pouredJugIndex];
		}
		States[jugIndex] = overflow;
	}

	//pour to infinite jug -> mode=1
	if (mode == 2)
	{
		currCap += States[jugIndex];
		States[jugIndex] = 0;
	}

	return States;
}

bool Is_In(vector<int> s)
{
	bool found = false;
	for (int i = 0; i < closed.size(); i++)
	{
		if (s == closed[i])
		{
			found = true;
			break;
		}
	}
	return found;
}

int main()
{
	for (int i = 0; i < 5; i++)
	{
		int a;
		cin >> a;
		jugsCapacity.push_back(a);
		jugStates.push_back(0);
	}

	cin >> capacity;

	priority_queue<StateSpaces, vector<StateSpaces>, CompareStateSpaces> pq;
	pq.push(StateSpaces(jugStates, 1));

	currCap = 0;

	while (true)
	{
		if (pq.empty())
		{
			cout << "Could not find" << endl;
			break;
		}

		StateSpaces ss = pq.top();
		pq.pop();

		if (currCap == capacity)
		{
			cout << "Goal reached" << endl;
			break;
		}

		for (int i = 0; i < 3; i++) //to go through all of the possible modes
		{
			if (i == 0) //mode 0
			{
				for (int j = 0; j < jugsCapacity.size(); j++)
				{
					if (ss.stateSpace[j] != jugsCapacity[j])
					{
						vector<int> v = Action(ss.stateSpace, j, j, i);
						double hv = hFunc(currCap, v[j], capacity);
						pq.push(StateSpaces(v, hv));
					}
				}
			}
			if (i == 1) //mode = 1
			{
				for (int j = 0; j < jugsCapacity.size(); j++)
				{
					for (int k = 0; k < jugsCapacity.size(); k++)
					{
						if (k != j && ss.stateSpace[j] != 0)
						{
							vector<int> v = Action(ss.stateSpace, j, k, i);
							double hv = hFunc(currCap, v[k], capacity);
							pq.push(StateSpaces(v, hv));
						}
					}
				}
			}
			if (i == 2) //mode = 2
			{
				for (int j = 0; j < jugsCapacity.size(); j++)
				{
					if (ss.stateSpace[j] != 0)
					{
						vector<int> v = Action(ss.stateSpace, j, j, i);
						double hv = hFunc(currCap, v[j], capacity);
						pq.push(StateSpaces(v, hv));
					}
				}
			}
		}
	}
}