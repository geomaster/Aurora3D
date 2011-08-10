#ifndef __AURORA_TRANSFORM_H__
#define __AURORA_TRANSFORM_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include "AuroraVector3D.h"
#include "AuroraQuaternion.h"

namespace Aurora
{
	enum TransformRelativeTo
	{
		ETRT_Parent,
		ETRT_Local,
		ETRT_World
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
			Matrix4 mat = (IsQuaternionUnit? Rotation.toRotationMatrix4Unit() : Rotation.toRotationMatrix4());
			mat *= Matrix4::fromScale(Scale.getX(), Scale.getY(), Scale.getZ());

			mat.setEntry(0, 3, Translation.getX());
			mat.setEntry(1, 3, Translation.getY());
			mat.setEntry(2, 3, Translation.getZ());

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
