/*
    This file is part of Aurora Game Engine.

    Aurora Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Aurora Game Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Aurora Game Engine.  If not, see <http://www.gnu.org/licenses/>.

	Copyright (C) David Davidovic (Geomaster) 2011.

*/
#ifndef __AURORA_BASE_H__
#define __AURORA_BASE_H__
#include "../AuroraPrereqs.h"
#include "../AuroraIterator.h"

namespace Aurora
{
	namespace STL
	{
		template<typename T, typename IteratorType, class Iterator>
		class GenericIteratorWrapper : public IteratorType
		{
		protected:
			Iterator mIter, mBegin, mEnd;

		public:
			GenericIteratorWrapper<T, IteratorType, Iterator>(const Iterator& it, const Iterator& begin, const Iterator& end) : mIter(it), mBegin(begin), mEnd(end) { ; }

			IteratorType* increment()
			{
				++mIter;
				return this;
			}

			void goBegin() { mIter = mBegin; }
			void goEnd() { mIter = mEnd; }

			T& get()
			{
				return *mIter;
			}

			bool hasMoreElements() const
			{
				return mIter != mEnd;
			}
		};

		template<typename T, typename IteratorType, class Iterator>
		class GenericConstantIteratorWrapper : public IteratorType
		{
		protected:
			Iterator mIter, mBegin, mEnd;

		public:
			GenericConstantIteratorWrapper<T, IteratorType, Iterator>(const Iterator& it, const Iterator& begin, const Iterator& end) : mIter(it), mBegin(begin), mEnd(end) { ; }

			IteratorType* increment()
			{
				++mIter;
				return this;
			}

			void goBegin() { mIter = mBegin; }
			void goEnd() { mIter = mEnd; }

			const T& get() const
			{
				return *mIter;
			}

			bool hasMoreElements() const
			{
				return mIter != mEnd;
			}

		};

		template<typename T, typename IteratorType, class Iterator>
		class GenericPairValueIteratorWrapper : public IteratorType
		{
		protected:
			Iterator mIter, mBegin, mEnd;

		public:
			GenericPairValueIteratorWrapper<T, IteratorType, Iterator>(const Iterator& it, const Iterator& begin, const Iterator& end) : mIter(it), mBegin(begin), mEnd(end) { ; }

			IteratorType* increment()
			{
				++mIter;
				return this;
			}

			void goBegin() { mIter = mBegin; }
			void goEnd() { mIter = mEnd; }

			T& get()
			{
				return mIter->second;
			}

			bool hasMoreElements() const
			{
				return mIter != mEnd;
			}
		};

		template<typename T, typename IteratorType, class Iterator>
		class GenericPairValueConstantIteratorWrapper : public IteratorType
		{
		protected:
			Iterator mIter, mBegin, mEnd;

		public:
			GenericPairValueConstantIteratorWrapper<T, IteratorType, Iterator>(const Iterator& it, const Iterator& begin, const Iterator& end) : mIter(it), mBegin(begin), mEnd(end) { ; }

			IteratorType* increment()
			{
				++mIter;
				return this;
			}

			void goBegin() { mIter = mBegin; }
			void goEnd() { mIter = mEnd; }

			const T& get() const
			{
				return mIter->second;
			}

			bool hasMoreElements() const
			{
				return mIter != mEnd;
			}

		};

		template<typename T, class Iterator>
		struct IteratorWrappers
		{
			typedef GenericIteratorWrapper<T, ContainerForwardIterator<T>, Iterator> Forward;
			typedef GenericConstantIteratorWrapper<T, ContainerConstantForwardIterator<T>, Iterator> ConstantForward;

			typedef GenericIteratorWrapper<T, ContainerReverseIterator<T>, Iterator> Reverse;
			typedef GenericConstantIteratorWrapper<T, ContainerConstantReverseIterator<T>, Iterator> ConstantReverse;

			typedef GenericPairValueIteratorWrapper<T, ContainerForwardIterator<T>, Iterator> ValueForward;
			typedef GenericPairValueConstantIteratorWrapper<T, ContainerConstantForwardIterator<T>, Iterator> ValueConstantForward;

			typedef GenericPairValueIteratorWrapper<T, ContainerReverseIterator<T>, Iterator> ValueReverse;
			typedef GenericPairValueConstantIteratorWrapper<T, ContainerConstantReverseIterator<T>, Iterator> ValueConstantReverse;

		};

/*
		template<typename T, typename STLContainer>
		class ContainerWrapper : public Aurora::Container<T>
		{
		protected:
			typedef typename IteratorWrappers<T, typename T::iterator>::Forward _FwdIterator;
			typedef typename IteratorWrappers<T, typename T::iterator>::ConstantForward _ConstFwdIterator;
			typedef typename IteratorWrappers<T, typename T::iterator>::Reverse _RevIterator;
			typedef typename IteratorWrappers<T, typename T::iterator>::ConstantReverse _ConstRevIterator;

			STLContainer* mContainer;
			_FwdIterator mFwdBIt;
			_ConstFwdIterator mCFwdBIt;
			_RevIterator mRevBIt;
			_ConstRevIterator mCRevBIt;
			_FwdIterator mFwdEIt;
			_ConstFwdIterator mCFwdEIt;
			_RevIterator mRevEIt;
			_ConstRevIterator mCRevEIt;

		public:
			ContainerWrapper<T, STLContainer>(STLContainer* c) : mContainer(c)
			{
				mFwdBIt = c->begin();
				mCFwdBIt = c->begin();
				mRevBIt = c->rbegin();
				mCRevBIt = c->rbegin();

				mFwdEIt = c->end();
				mCFwdEIt = c->end();
				mRevEIt = c->rend();
				mCRevEIt = c->rend();
			}

			typename Container<T>::Iterator* getBegin()
			{
				return &mFwdBIt;
			}

			typename Container<T>::Iterator* getEnd()
			{
				return &mFwdEIt;
			}

			typename Container<T>::ConstantIterator*getBegin() const
			{
				return &mCFwdBIt;
			}

			typename Container<T>::ConstantIterator getEnd() const
			{
				return &mCFwdEIt;
			}

			typename Container<T>::ReverseIterator getReverseBegin()
			{
				return &mRevBIt;
			}

			typename Container<T>::ReverseIterator getReverseEnd()
			{
				return &mRevEIt;
			}

			typename Container<T>::ConstantReverseIterator getReverseBegin() const
			{
				return &mCRevBIt;
			}

			typename Container<T>::ConstantReverseIterator getReverseEnd() const
			{
				return &mCRevEIt;
			}

			T& getFront()
			{
				return mContainer->front();
			}

			T& getBack()
			{
				return mContainer->back();
			}

			const T& getFront() const
			{
				return mContainer->front();
			}

			const T& getBack() const
			{
				return mContainer->back();
			}
		};
*/
	}
}

#endif // __AURORA_BASE_H__
