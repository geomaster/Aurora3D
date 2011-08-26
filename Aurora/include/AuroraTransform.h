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
		inline Transform() : Position(Real( 0.0 )), Orientation(Quaternion::Identity), Scale(Real( 1.0 ))
		{
		}

		inline Transform(const Vector3D& _Position, const Quaternion& _Orientation, const Vector3D& _Scale) : Position(_Position), Orientation(_Orientation), Scale(_Scale)
		{
		}

		Vector3D Position;
		Quaternion Orientation;
		Vector3D Scale;

		Matrix4 toMatrix(bool IsQuaternionUnit = true) const
		{
			Matrix4 mat;
			mat = (IsQuaternionUnit? Orientation.toRotationMatrix4Unit() : Orientation.toRotationMatrix4());
			mat *= Matrix4::fromTranslation(Position);
			mat *= Matrix4::fromScale(Scale);
			return mat;
		}

		inline Transform concatenate(const Transform& other) const
		{
			Transform concat;
			concat.Orientation = Orientation * other.Orientation;
			concat.Scale = Scale * other.Scale;

			// Transform the displacement vector
			concat.Position = Orientation * (Scale * other.Position) + Position; 

			return concat;
		}

		inline void append(const Transform &other)
		{
			Position = other.Orientation * (other.Scale * Position) + other.Position;
			Orientation *= other.Orientation;
			Scale *= other.Scale;
		}
	};
}

#endif // __AURORA_TRANSFORM_H__
