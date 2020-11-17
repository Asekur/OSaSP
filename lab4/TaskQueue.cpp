#include "TaskQueue.h"
#include <mutex>

std::mutex g_lock;

TaskQueue::TaskQueue()
{
}


int TaskQueue::addTask(TTask task)
{
	//блокировка мьютексом
	g_lock.lock();
	//добавление задания в очередь
	tasksQueue->push(task);
	g_lock.unlock();

	return 1;
}

TTask TaskQueue::popTask()
{
	//извлечение задания из очереди
	TTask result;

	g_lock.lock();
	if (tasksQueue->empty()) {
		result = NULL;
	}
	else {
		//первый элемент из очереди
		result = tasksQueue->front();
		tasksQueue->pop();
	}
	g_lock.unlock();

	return result;
}
