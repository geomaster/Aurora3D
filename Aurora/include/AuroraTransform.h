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
#ifndef __AURORA_TRANSFORM_H__
#define __AURORA_TRANSFORM_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include "AuroraVector3D.h"
#include "AuroraQuaternion.h"

namespace Aurora
{
	enum TransformSpace
	{
		ETS_Parent,
		ETS_Local,
		ETS_World
	};

	struct Transform : virtual public SSEAlloc
	{
		inline Transform() : Translation(Real( 0.0 )), Rotation(Quaternion::Identity), Scale(Real( 1.0 ))
		{
		}

		inline Transform(const Vector3D& _Translation, const Quaternion& _Rotation, const Vector3D& _Scale) : Translation(_Translation), Rotation(_Rotation), Scale(_Scale)
		{
		}

		Vector3D Translation;
		Quaternion Rotation;
		Vector3D Scale;

		Matrix4 toMatrix(bool IsQuaternionUnit = true) const
		{
			Matrix4 mat;
			mat = (IsQuaternionUnit? Rotation.toRotationMatrix4Unit() : Rotation.toRotationMatrix4());
			mat *= Matrix4::fromTranslation(Translation);
			mat *= Matrix4::fromScale(Scale);
			return mat;
		}

		inline Transform concatenate(const Transform& other) const
		{
			return Transform(Translation + other.Translation, Rotation * other.Rotation, Scale * other.Scale);
		}

		inline void append(const Transform &other)
		{
			Translation += other.Translation;
			Rotation *= other.Rotation;
			Scale *= other.Scale;
		}
	};
}

#endif // __AURORA_TRANSFORM_H__
