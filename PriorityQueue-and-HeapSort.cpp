#define _CRT_Secure_NO_WARNINGS
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include<algorithm>
using namespace std;
#define S 1000000

struct Task
{
	int deadline;   //чим менше число, тим ближче дедлайн і вищий пріорітет
	bool operator ==(const Task &other)
	const
	{
		return this->deadline == other.deadline;
	}
	bool operator <(const Task& other)
	const
	{
		return this->deadline > other.deadline;
	}
};


struct PriorityQueue 
{
	int N = 1;
	Task* DA = new Task[N];
	int Size = 0;

	~PriorityQueue() {
		delete[] DA;
	}

	void resize() {
		Task* DA2 = new Task[N * 2];
		for (int i = 0; i < Size; i++)
			DA2[i] = DA[i];
		delete[] DA;
		DA = DA2;
		N *= 2;
	}

	int getParent(int i)
	{
		if (i == 0)
		{
			return 0;
		}
		else
		{
			return (i - 1) / 2;
		}
	}

	int getLeftChild(int i)
	{
		return 2 * i + 1;
	}

	int getRightChild(int i)
	{
		return 2 * i + 2;
	}

	void siftUp(int i)
	{
		int j = getParent(i);
		while (DA[j] < DA[i])
		{
			Task temp = DA[i];
			DA[i] = DA[j];
			DA[j] = temp;
			i = j;
			j = getParent(i);
		}
	}

	void siftDown(int i)
	{
		while (getLeftChild(i) < Size)
		{
			int l = getLeftChild(i);
			int r = getRightChild(i);
			int j = l;
			if (r < Size && DA[l] < DA[r])
			{
				j = r;
			}
			if (DA[i] < DA[j])
			{
				Task temp = DA[j];
				DA[j] = DA[i];
				DA[i] = temp;
				i = j;
			}
			else
			{
				return;
			}
		}
	}

	void push(Task task) {
		DA[Size] = task;
		siftUp(Size);
		Size++;
		if (Size == N)
		{
			resize();
		}
	}

	Task top()
	{
		if (Size != 0)
		{
			return DA[0];
		}
		else
		{
			cout << "This queue is empty" << endl;
		}
	}

	Task pop()
	{
		if (Size != 0)
		{
			Task deleted = DA[0];
			Size--;
			DA[0] = DA[Size];
			siftDown(0);
			return deleted;
		}
		else
		{
			cout << "This queue is empty" << endl;
		}
	}

	int size()
	{
		return Size;
	}

	bool empty()
	{
		return !Size;
	}

	
};


void siftdown(int i, int arr[], int size)
{
	while (2 * i + 1 < size)
	{
		int l = 2 * i + 1;
		int r = 2 * i + 2;
		int j = l;
		if (r < size && arr[l] < arr[r])
		{
			j = r;
		}
		if (arr[i] < arr[j])
		{
			int temp = arr[j];
			arr[j] = arr[i];
			arr[i] = temp;
			i = j;
		}
		else
		{
			return;
		}
	}
}

void HeapSort(int arr[], int size)
	{
		int h = log2(size);
		int last = 0;
		for (int i = 0; i < h; i++)
		{
			last += pow(2, i);
		}
		for (last; last>=0; last--)
		{
			siftdown(last, arr, size);
		}
		while(size != 1)
		{
			size--;
			int root = arr[0];
			arr[0] = arr[size];
			arr[size] = root;
			siftdown(0, arr, size);
		}
	}



template <typename T>
float testPriorityQueueSpeed(T&& priorityQueue)
{
	const int iters = 100000;
	clock_t timeStart = clock();
	for (int i = 0; i < iters; i++)
	{
		int insertDataAmount = rand() % 6 + 5;
		for (int j = 0; j < insertDataAmount; j++)
		{
			priorityQueue.push(Task());
		}
		priorityQueue.top();
		priorityQueue.pop();
	}
	clock_t timeEnd = clock();
	float time = (float(timeEnd - timeStart)) / CLOCKS_PER_SEC;
	return time;
}
bool testPriorityQueue()
{
	srand(time(NULL));
	const int iters = 20000;
	PriorityQueue myPriorQueue;
	priority_queue<Task> stlPriorQueue;
	bool isDataEqual = true;
	for (int i = 0; i < iters; i++)
	{
		int insertDataAmount = rand() % 6 + 5;
		for (int j = 0; j < insertDataAmount; j++)
		{
			Task randData = Task();
			myPriorQueue.push(randData);
			stlPriorQueue.push(randData);
		} if (!(myPriorQueue.top() == stlPriorQueue.top()))
		{
			isDataEqual = false;
			cerr << "Comparing failed on iteration " << i << endl << endl;
			break;
		}
		int removeDataAmount = rand() % insertDataAmount;
		for (int j = 0; j < removeDataAmount; j++)
		{
			myPriorQueue.pop();
			stlPriorQueue.pop();
		}
	}
	int myQueueSize = myPriorQueue.size();
	int stlQueueSize = stlPriorQueue.size();
	float stlTime =
		testPriorityQueueSpeed<priority_queue<Task>>(priority_queue<Task>());
	float myTime = testPriorityQueueSpeed<PriorityQueue>(PriorityQueue());
	cout << "My PriorityQueue:" << endl;
	cout << "Time: " << myTime << ", size: " << myQueueSize << endl;
	cout << "STL priority_queue:" << endl;
	cout << "Time: " << stlTime << ", size: " << stlQueueSize << endl << endl;
	if (isDataEqual && myQueueSize == stlQueueSize)
	{
		cout << "The lab is completed" << endl << endl;
		return true;
	}
	cerr << ":(" << endl << endl;
	return false;
}


int main()
{
	testPriorityQueue();
	int* arr = new int[S];
	for (int i = 0; i < S; i++)
	{
		arr[i] = rand();
	}
	clock_t my_start = clock();
    HeapSort(arr, S);
	clock_t my_end = clock();

	clock_t stl_start = clock();
	sort(arr, arr + S - 1);
	clock_t stl_end = clock();

	float myTime = (float(my_end - my_start)) / CLOCKS_PER_SEC;
	float stlTime = (float(stl_end - stl_start)) / CLOCKS_PER_SEC;

	cout << "My HeapSort:" << endl;
	cout << "Time: " << myTime << endl;
	cout << "STL Sort:" << endl;
	cout << "Time: " << stlTime << endl;
	delete[] arr;
}


