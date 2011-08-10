#include "HashBenchmarker.h"
#include <cstdlib>
#include <sstream>
#include "GeneralHashFunctions.h"

extern "C"
{
	#include "lookup3.h"
	#include "fnv.h"
}

#define GlobalFuncHasher(HashFunc, name) \
	class name##_##hasher : public hasher<std::string> \
	{ \
	public: \
		inline std::size_t hash(const std::string& in) {return HashFunc(in);} \
	};

#define GlobalFuncDoBench(namepr, desc) benchmarkHasher(desc, &##namepr##_##hasher##(), testCases)

class default_hasher : public hasher<std::string>
{
protected:
	std::hash<std::string> mHasher;

public:
	inline std::size_t hash(const std::string& in)
	{
		return mHasher(in);
	}
};

class null_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash(const std::string& in)
	{
		return 0;
	}
};

class jenkins1_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash(const std::string& in)
	{
		unsigned int hash = 0;
		for(std::string::const_iterator it = in.begin(); it != in.end(); ++it)
		{
			hash += *it;
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}
};

class jenkins3_hasher : public hasher<std::string>
{
protected:
	std::size_t mLast;

public:
	jenkins3_hasher() : mLast(0)
	{
	}

	inline std::size_t hash(const std::string& in)
	{
		return mLast = std::size_t(hashlittle(in.c_str(), in.length(), mLast));
	}
};

class fnv_hasher : public hasher<std::string>
{
protected:
std::size_t mLast;

public:
	fnv_hasher() : mLast(FNV1_32_INIT)
	{
	}

	inline std::size_t hash(const std::string& in)
	{
		return mLast = fnv_32_str(in.c_str(), mLast);
	}
};

class trivial_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash(const std::string& in)
	{
		std::size_t hash;
		unsigned int i;
		for (hash=0, i=0; i<in.length(); ++i)
			hash = ((hash<<5)^(hash>>27))^in[i];
		
		return (hash % 16777619L);
	}
};

class java_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash (const std::string& in)
	{
		std::size_t h = 0;
		for (std::string::const_iterator it = in.begin(); it != in.end(); ++it)
			h = h*31+*it;
		return h;
	}
};

class newstd_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash (const std::string& in)
	{
		std::size_t hash = 2166136261U;
		std::size_t len = in.length();
		std::size_t step = len / 10 + 1;

		for (unsigned int i = 0; i < len; i += step)
		{
			hash = hash * 16777619U;
			hash ^= static_cast<std::size_t>(in[i]);
		}

		return hash;
	}
};

class fnvstr_asm_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash (const std::string& in_str)
	{
		const char* p = in_str.c_str();
		__asm 
		{
			mov ebx, p;
			mov eax, 2166136261U;

main_loop:
			movsx ecx, BYTE PTR[ebx];
			test cl, cl;
			jz out_loop;
			imul eax, eax, 16777619U;
			xor eax, ecx;
			inc ebx;
			jmp main_loop;

			out_loop:
		}
	}
};

class fnvstr_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash (const std::string& in_str)
	{
		const char* p = in_str.c_str();
		std::size_t hash = 2166136261U;

		char d;
		while (d = *p++)
			hash = hash * 16777619U ^ d;

		return hash;
	}
};

class modfnv_asm_hasher : public hasher<std::string>
{
public:
	inline std::size_t hash (const std::string& in_str)
	{
		const char* d = in_str.c_str();
		std::size_t len = in_str.length();
		__asm
		{
			mov eax, 0xCCCCCCCD;
			mov ecx, len;
			mul ecx;
			shr edx, 3;
			inc edx;

			mov eax, 2166136261;
			mov ebx, d;
			xor edi, edi;

			main_loop:
			imul eax, eax, 16777619U;
			movsx esi, BYTE PTR [ebx+edi];
			xor eax, esi;
			add edi, edx;
			cmp edi, ecx;
			jb main_loop;
		}
	}
};

class pseudorandom_hasher : public hasher<std::string>
{
public:

	inline std::size_t hash (const std::string& in)
	{
		size_t seed = 0;
		for (std::string::const_iterator it = in.begin(); it != in.end(); ++it)
			seed += *it;

		srand(seed);
		return rand();
	}
};

GlobalFuncHasher(RSHash, rs);
GlobalFuncHasher(JSHash, js);
GlobalFuncHasher(PJWHash, pjw);
GlobalFuncHasher(ELFHash, elf);
GlobalFuncHasher(BKDRHash, bkdr);
GlobalFuncHasher(SDBMHash, sdbm);
GlobalFuncHasher(DJBHash, djb);
GlobalFuncHasher(DEKHash, dek);
GlobalFuncHasher(BPHash, bp);
GlobalFuncHasher(FNVHash, fnv2);
GlobalFuncHasher(APHash, ap);

TestCase randomTestCase(char MinChar, char MaxChar, int LengthMin, int LengthMax, int NumKeys)
{
	TestCase t;
	t.keys.resize(NumKeys);
	for (int l = 0; l < NumKeys; ++l)
	{
		int length = LengthMin + (rand() % (LengthMax - LengthMin));
		std::string k;
		k.resize(length);
		for (int j = 0; j < length; ++j)
		{
			char c = MinChar + (rand() % (MaxChar - MinChar));
			k[j] = c;
		}
		t.keys[l] = k;
	}
	return t;
}

void addTestCases(std::vector<TestCase> *cases)
{
	// random
	for (int i = 1; i <= 10; ++i)
	{
		int lengthMax = i * 100;
		int lengthMin = (i-1) * 100;
		std::ostringstream nameStr;
		nameStr << "Random symbols, length 100-1000, 200 keys (";
		nameStr << i;
		nameStr << ")";

		TestCase p = randomTestCase(0, 127, lengthMin, lengthMax, 200);
		p.Name = nameStr.str();
		cases->push_back(p);
	}

	// random alnum
	for (int i = 1; i <= 10; ++i)
	{
		int lengthMax = i * 100;
		int lengthMin = (i-1) * 100;
		std::ostringstream nameStr;
		nameStr << "Random alphabetics, length 100-1000, 200 keys (";
		nameStr << i;
		nameStr << ")";

		TestCase p = randomTestCase(65, 123, lengthMin, lengthMax, 200);
		p.Name = nameStr.str();
		cases->push_back(p);
	}
	/*
	// random alnum
	for (int i = 1; i <= 10; ++i)
	{
		int lengthMax = i * 10;
		int lengthMin = (i-1) * 10;
		std::ostringstream nameStr;
		nameStr << "Random alphabetics, length 10-100, 10 keys (";
		nameStr << i;
		nameStr << ")";

		TestCase p = randomTestCase(65, 123, lengthMin, lengthMax, 10);
		p.Name = nameStr.str();
		cases->push_back(p);
	}*/
	
	TestCase normal;
	normal.keys.push_back("John");
	normal.keys.push_back("Will");
	normal.keys.push_back("Brian");
	normal.keys.push_back("James");
	normal.keys.push_back("Michelle");
	normal.keys.push_back("Joanne");
	normal.keys.push_back("Donald");
	normal.keys.push_back("Amy");
	normal.keys.push_back("Mister");
	normal.keys.push_back("Matthew");
	normal.keys.push_back("Emily");
	normal.keys.push_back("Sarah");
	normal.keys.push_back("Aaron");
	normal.keys.push_back("Ollie");
	normal.keys.push_back("Rachel");
	normal.Name = "Common english names";

	TestCase realworld;
	realworld.keys.push_back("DefaultSceneNode");
	realworld.keys.push_back("DefaultSceneNode");
	realworld.keys.push_back("Skybox1");
	realworld.keys.push_back("Skybox2");
	realworld.keys.push_back("Skydome1");
	realworld.keys.push_back("PlayerAura");
	realworld.keys.push_back("ForceFieldEmitter");
	realworld.keys.push_back("AmbientMusicEmitter");
	realworld.keys.push_back("PhysicalRepresentation");
	realworld.keys.push_back("OcclusionCullingMethod");
	realworld.keys.push_back("SewerPipe001");
	realworld.keys.push_back("SewerPipe002");
	realworld.Name = "Real-world names in a 3D engine";


	cases->push_back(normal);
	cases->push_back(realworld);
}

void benchmarkHasher(std::string Name, hasher<std::string> *h, std::vector<TestCase> &cases)
{
	HashBenchmarkDesc<std::string> DefaultHash;
	DefaultHash.Subject = h;
	DefaultHash.IterationsPerTestCase = 100;
	DefaultHash.TestCases = cases;

	HashBenchmarker<std::string> b;
	HashBenchmarkResult r;
	printf("Benchmarking %s...\n", Name.c_str());
	r = b.performBenchmark(DefaultHash);
	printf("Benchmark finished.\n");

	printf("\n"
		   "	Collision factor\n"
		   "		Avg:  %.8f\n"
		   "		Peak: %.8f on\n"
		   "			%s\n"
		   "		Best: %.8f on\n"
		   "			%s\n"
		   "\n"
		   "	Running time\n"
		   "		Avg:  %.8f\n"
		   "		Peak: %.8f on\n"
		   "			%s\n"
		   "		Best: %.8f on\n"
		   "			%s\n\n",
		   r.AverageCollisionFactor, r.PeakCollisionFactor, r.PeakCF.Name.c_str(), r.BestCollisionFactor, r.BestCF.Name.c_str(),
		   r.AverageRunningTime, r.PeakRunningTime, r.PeakRT.Name.c_str(), r.BestRunningTime, r.BestRT.Name.c_str()
		   );
}

int main()
{
	srand(time(NULL));

	fnvstr_asm_hasher h1;
	fnvstr_hasher h2;
	std::string k = "RANDOM !!!!!!!!!! aadv  dddaaaa!!wsqqqqqqaaargh! lolololol 12454787777\x4c";
	std::size_t a = h2.hash(k);
	std::size_t b = h1.hash(k);

	printf("%u %u", a, b);

	std::vector<TestCase> testCases;
	printf("Building test cases..\n");
	addTestCases(&testCases);
	printf("Finished.\n");
	/*benchmarkHasher("null hasher, for reference", &null_hasher(), testCases);
	benchmarkHasher("trivial hash", &trivial_hasher(), testCases);
	benchmarkHasher("C pseudo-random hash (impl. specific)", &pseudorandom_hasher(), testCases);
	benchmarkHasher("std::hash", &default_hasher(), testCases);
	benchmarkHasher("Java hashCode()", &java_hasher(), testCases);
	benchmarkHasher("Jenkins one-at-a-time hash", &jenkins1_hasher(), testCases);
	benchmarkHasher("Jenkins lookup3 hash", &jenkins3_hasher(), testCases);
	benchmarkHasher("Fowller-Noll-Vo hash", &fnv_hasher(), testCases);*//*
	benchmarkHasher("Stochastic FNV", &newstd_hasher(), testCases);
	benchmarkHasher("Stochastic FNV (asm)", &modfnv_asm_hasher(), testCases);
	*/

	benchmarkHasher("FNV string", &fnvstr_hasher(), testCases);
	benchmarkHasher("FNV string (asm)", &fnvstr_asm_hasher(), testCases);

	/*printf("Benchmarking hash implementations given by Arash Partow on partow.net\n");
	GlobalFuncDoBench(rs, "Robert Sedwicks hash");
	GlobalFuncDoBench(js, "Justin Sobel bitwise hash");
	GlobalFuncDoBench(pjw, "Peter J. Weinberger hash");
	GlobalFuncDoBench(elf, "ELF hash");
	GlobalFuncDoBench(bkdr, "Kernighan-Ritchie hash");
	GlobalFuncDoBench(sdbm, "SDBM project hash");
	GlobalFuncDoBench(djb, "Bernstein hash");
	GlobalFuncDoBench(dek, "Donald Knuth hash");
	GlobalFuncDoBench(bp, "BP hash");
	GlobalFuncDoBench(fnv2, "Arash Partow's implementation of FNV hash");
	GlobalFuncDoBench(ap, "Arash Partow hash");*/

	system("pause >nul");
	return 0;
}