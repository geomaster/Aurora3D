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
#ifndef __AURORA_CONTAINER_H__
#define __AURORA_CONTAINER_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"

namespace Aurora
{
	template<typename T>
	class ContainerForwardIterator : virtual public Alloc
	{
	public:
		virtual ContainerForwardIterator* increment() = 0;
		virtual void goBegin() = 0;
		virtual void goEnd() = 0;

		virtual T& get() = 0;

		virtual bool hasMoreElements() const = 0;
	};

	template<typename T>
	class ContainerConstantForwardIterator : virtual public Alloc
	{
	public:
		virtual ContainerConstantForwardIterator* increment() = 0;
		virtual void goBegin() = 0;
		virtual void goEnd() = 0;

		virtual const T& get() const = 0;

		virtual bool hasMoreElements() const = 0;
	};

	template<typename T>
	class ContainerReverseIterator : virtual public Alloc
	{
	public:
		virtual ContainerReverseIterator* increment() = 0;
		virtual void goBegin() = 0;
		virtual void goEnd() = 0;

		virtual T& get() = 0;

		virtual bool hasMoreElements() const = 0;
	};

	template<typename T>
	class ContainerConstantReverseIterator : virtual public Alloc
	{
	public:
		virtual ContainerConstantReverseIterator* increment() = 0;
		virtual void goBegin() = 0;
		virtual void goEnd() = 0;

		virtual const T& get() const = 0;

		virtual bool hasMoreElements() const = 0;
	};

/*
	template<typename T>
	class Container
	{
		typedef ContainerForwardIterator<T> ForwardIterator, Iterator;
		typedef ContainerConstantForwardIterator<T> ConstanForwardIterator, ConstantIterator;
		typedef ContainerReverseIterator<T> ReverseIterator;
		typedef ContainerConstantReverseIterator<T> ConstantReverseIterator;

		virtual Iterator* getBegin() = 0;
		virtual Iterator* getEnd() = 0;
		virtual ConstantIterator* getBegin() const = 0;
		virtual ConstantIterator* getEnd() const = 0;

		virtual ReverseIterator* getReverseBegin() = 0;
		virtual ReverseIterator* getReverseEnd() = 0;
		virtual ConstantReverseIterator* getReverseBegin() const = 0;
		virtual ConstantReverseIterator* getReverseEnd() const = 0;

		virtual T& getFront() = 0;
		virtual const T& getFront() const = 0;
		virtual T& getBack() = 0;
		virtual const T& getBack() const = 0;
	};
*/

}

#endif // __AURORA_CONTAINER_H__
