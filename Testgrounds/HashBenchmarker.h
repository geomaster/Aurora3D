#pragma once
#include <vector>
#include <Windows.h>

template <class T>
class hasher
{
public:
	virtual std::size_t hash(const T& in) = 0;
};

struct TestCase
{
	std::string Name;
	std::vector<std::string> keys;
};

struct TestCaseIterationResult
{
	unsigned long Collisions;
	double CollisionFactor;
	double RunningTime;
};

struct TestCaseResult
{
	TestCase Subject;
	std::vector< TestCaseIterationResult > Iterations;
};

template <class T>
struct HashBenchmarkDesc
{
	hasher<T>* Subject;
	std::vector<TestCase> TestCases;
	unsigned long IterationsPerTestCase;
};

struct HashBenchmarkResult
{
	std::vector< TestCaseResult > Results;

	double AverageCollisionFactor, BestCollisionFactor, PeakCollisionFactor;
	double AverageRunningTime, BestRunningTime, PeakRunningTime;

	TestCase BestCF, PeakCF, BestRT, PeakRT;
};

template <class T>
class HashBenchmarker
{
protected:
	LARGE_INTEGER mStart, mFreq;
	bool mCounting;

	void startCounting()
	{
		QueryPerformanceFrequency(&mFreq);
		QueryPerformanceCounter(&mStart);
		mCounting = true;
	}

	double peekTimer()
	{
		if (!mCounting) return 0.0;

		LARGE_INTEGER k;
		QueryPerformanceCounter(&k);
		return (double)(k.QuadPart - mStart.QuadPart) / (double)mFreq.QuadPart;
	}

	double stopCounting()
	{
		if (!mCounting) return 0.0;
		double r = peekTimer();
		mCounting = false;

		return r;
	}


	void iterateTestCase(hasher<T> * Subject, const TestCase& Case, TestCaseIterationResult* Res)
	{
		std::vector<bool> alreadySeen;
		alreadySeen.resize(Case.keys.size());


		unsigned long Collisions = 0, sz = Case.keys.size();
		startCounting();
		for (std::vector<T>::const_iterator it = Case.keys.begin(); it != Case.keys.end(); ++it)
		{
			std::size_t hash = Subject->hash(*it) % sz;
			if (alreadySeen[hash])
				++Collisions; 
			else
				alreadySeen[hash] = true;
		}
		double Time = stopCounting();
		Res->RunningTime = Time;
		Res->Collisions = Collisions;
		Res->CollisionFactor =  double(Collisions) / double(sz);
	}

public:

	HashBenchmarkResult performBenchmark(HashBenchmarkDesc<T> Desc)
	{
		HashBenchmarkResult FullResult;
		FullResult.Results.resize(Desc.TestCases.size());

		unsigned long nTestCases = Desc.TestCases.size();
		double ndTestCases = nTestCases;
		int p = 0;

		double avgCollisionFactor = 0.0, BestCollisionFactor = 0.0, PeakCollisionFactor = 0.0;
		double avgRunningTime = 0.0;
		double BestRunningTime = 0, PeakRunningTime = 0;

		double IterationContribution = 1.0 / (ndTestCases * double(Desc.IterationsPerTestCase));
		bool f = true;
		for (std::vector<TestCase>::const_iterator it = Desc.TestCases.begin(); it != Desc.TestCases.end(); ++it, ++p)
		{
			TestCaseResult res;
			res.Subject = *it;
			res.Iterations.resize(Desc.IterationsPerTestCase);
			for (unsigned int i = 0; i < Desc.IterationsPerTestCase; ++i)
			{
				TestCaseIterationResult iter;
				iterateTestCase(Desc.Subject, *it, &iter);
				res.Iterations[i] = iter;

				avgCollisionFactor += iter.CollisionFactor * IterationContribution;
				if (iter.CollisionFactor > PeakCollisionFactor)
					PeakCollisionFactor = iter.CollisionFactor, FullResult.PeakCF = *it;
				if (f) BestCollisionFactor = iter.CollisionFactor, FullResult.BestCF = *it;
				else
					if (iter.CollisionFactor < BestCollisionFactor)
					BestCollisionFactor = iter.CollisionFactor, FullResult.BestCF = *it;

				avgRunningTime += double(iter.RunningTime) * IterationContribution;
				if (iter.RunningTime > PeakRunningTime)
					PeakRunningTime = iter.RunningTime, FullResult.PeakRT = *it;
				if (f) BestRunningTime = iter.RunningTime, FullResult.BestRT = *it;
				else
					if (iter.RunningTime < BestRunningTime)
						BestRunningTime = iter.RunningTime, FullResult.BestRT = *it;

				f=false;
			}

			FullResult.Results[p] = res;
		}

		FullResult.AverageRunningTime = avgRunningTime;
		FullResult.AverageCollisionFactor = avgCollisionFactor;
		FullResult.BestCollisionFactor = BestCollisionFactor;
		FullResult.BestRunningTime = BestRunningTime;
		FullResult.PeakCollisionFactor = PeakCollisionFactor;
		FullResult.PeakRunningTime = PeakRunningTime;

		return FullResult;
	}
};