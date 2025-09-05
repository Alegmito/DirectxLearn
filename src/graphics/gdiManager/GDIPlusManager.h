#pragma once
#include <basetsd.h>
#include <atomic>

class GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();
private:
	static std::atomic<ULONG_PTR> token;
	static std::atomic<int> refCount;
};
