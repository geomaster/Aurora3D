#include "WinTimer.h"
#include <AuroraVector2D.h>
#include <AuroraVector3D.h>
#include <AuroraVector4D.h>
#include <AuroraMatrix4.h>
#include <AuroraMatrix3.h>

using namespace Aurora;

void consistencyCheck(bool Expression, String Name)
{
	if (Expression)
		printf("%s: OK.\n", Name.c_str());
	else
		printf("ERROR! %s failure!\n", Name.c_str());
}

void testVec2D()
{

	printf("Commencing consistency testing for Vector2D...\n");

	/*********************************************************/
	printf("Testing constructors...\n");
	Vector2D subj1(10.f, 20.f);
	consistencyCheck(subj1.getX() == 10.f && subj1.getY() == 20.f, "Vector2D two-argument constructor");

	Vector2D subj2(5.f);
	consistencyCheck(subj2.getX() == 5.f && subj2.getY() == 5.f, "Vector2D scalar constructor");

	Real aux1[2] = { 1.f, 2.f };
	Vector2D subj3(aux1);
	consistencyCheck(subj3.getX() == 1.f && subj3.getY() == 2.f, "Vector2D from ptr constructor");

	Vector2D copyFrom1;
	copyFrom1.setX(4.f);
	copyFrom1.setY(2.f);
	Vector2D subj4(copyFrom1);
	consistencyCheck(subj4.getX() == 4.f && subj4.getY() == 2.f, "Vector2D copy constructor");

	printf("Constructor test done.\n");

	/*********************************************************/
	printf("Testing member function behavior...\n");

	Vector2D subj5(3.f, 4.f);
	Real len5 = subj5.getLength();
	consistencyCheck(Math::equal(len5, 5.f), "Vector2D length computation");

	Vector2D subj6(0.f, 0.f);
	Real len6 = subj6.getLength();
	consistencyCheck(Math::equal(len6, 0.f), "Vector2D zero length");

	subj5.setX(1.f);
	subj5.setY(1.f);
	Real len5_2 = subj5.getLength();
	consistencyCheck(Math::equal(len5_2, Aurora::Sqr2), "Vector2D correct length caching");

	Vector2D subj7(2.f, 5.f);
	Real lenSq7 = subj7.getLengthSquared();
	consistencyCheck(Math::equal(lenSq7, 29.f), "Vector2D squared length computation");

	Vector2D subj8(0.f, 0.f);
	Real lenSq8 = subj8.getLengthSquared();
	consistencyCheck(Math::equal(lenSq8, 0.f), "Vector2D zero squared length");

	subj8.setX(9.f);
	subj8.setY(6.f);
	Real lenSq8_2 = subj8.getLengthSquared();
	consistencyCheck(Math::equal(lenSq8_2, 117.f), "Vector2D correct squared length caching");

	Vector2D subj9(1.f, 1.f);
	subj9.normalize();
	Real len9_man = Math::sqrt(subj9.getX() * subj9.getX() + subj9.getY() * subj9.getY());
	Real len9 = subj9.getLength();
	consistencyCheck(Math::equal(len9_man, 1.f), "Vector2D normalization length");
	consistencyCheck(Math::equal(len9, 1.f), "Vector2D reported normalization length");

	subj9.setX(4.f);
	Real len9_2 = subj9.getLength();
	consistencyCheck(Math::notEqual(len9_2, len9), "Vector2D reported denormalized length");

	Vector2D subj10(subj9.getNormalized());
	Real len10_man = Math::sqrt(subj10.getX() * subj10.getX() + subj10.getY() * subj10.getY());
	Real len10 = subj10.getLength();
	consistencyCheck(Math::equal(len10_man, 1.f), "Vector2D getNormalized() length");
	consistencyCheck(Math::equal(len10, 1.f), "Vector2D reported getNormalized() length");

	Vector2D subj11(1.f, 1.f), subj12(-1.f, -1.f);
	Real dot11_12 = subj11.dot(subj12), dot12_11 = subj12.dot(subj11);
	consistencyCheck(Math::equal(dot11_12, dot12_11), "Vector2D dot product commutativity");
	Real len = -1.f * subj11.getLength() * subj12.getLength();
	consistencyCheck(Math::equal(dot11_12, -1.f * subj11.getLength() * subj12.getLength()), "Vector2D perpendicular vector dot");

	Vector2D subj13(0.f, 1.f), subj14(Aurora::Sqr2 / 2.f, Aurora::Sqr2 / 2.f);
	Angle angle13_14 = subj13.getAngleTo(subj14), angle14_13 = subj14.getAngleTo(subj13);
	consistencyCheck(Math::equal(Degree(angle13_14), Degree(angle14_13)), "Vector2D getAngleTo() identity");
	consistencyCheck(Math::equal(Degree(angle13_14), 45.f), "Vector2D getAngleTo()");

	Vector2D subj15(0.f, 0.f), subj16(3.f, 4.f);
	Real distance15_16 = subj15.getDistanceTo(subj16), distance16_15 = subj16.getDistanceTo(subj15);
	consistencyCheck(Math::equal(distance15_16, distance16_15), "Vector2D distance commutativity");
	consistencyCheck(Math::equal(distance15_16, 5.f), "Vector2D distance");

	Real distanceSq15_16 = subj15.getSquaredDistanceTo(subj16), distanceSq16_15 = subj16.getSquaredDistanceTo(subj15);
	consistencyCheck(Math::equal(distanceSq15_16, distanceSq16_15), "Vector2D squared distance commutativity");
	consistencyCheck(Math::equal(distanceSq15_16, 25.f), "Vector2D squared distance");

	printf("Member function test done.\n");
	/*********************************************************/
	printf("Testing operators...\n");
	Vector2D subj17(5.f, 6.f), subj18(2.f, -8.f);
	Vector2D subj19(subj17+subj18), subj18equiv_1(subj17+subj18-subj17);

	consistencyCheck(Math::equal(subj19.getX(), 7.f) && Math::equal(subj19.getY(), -2.f), "Vector2D addition");
	consistencyCheck(Math::equal(subj18equiv_1.getX(), subj18.getX()) && Math::equal(subj18equiv_1.getY(), subj18.getY()), "Vector2D addition identity");

	Vector2D subj20(subj17-subj18), subj17equiv_1(subj17-subj18+subj18);
	consistencyCheck(Math::equal(subj20.getX(), 3.f) && Math::equal(subj20.getY(), 14.f), "Vector2D subtraction");
	consistencyCheck(Math::equal(subj17equiv_1.getX(), subj17.getX()) && Math::equal(subj17equiv_1.getY(), subj17.getY()), "Vector2D subtraction identity");

	Vector2D subj21(-subj17), subj22(-subj21);
	consistencyCheck(Math::equal(subj21.getX(), -5.f) && Math::equal(subj21.getY(), -6.f), "Vector2D negation");
	consistencyCheck(Math::equal(subj22.getX(), subj17.getX()) && Math::equal(subj22.getY(), subj17.getY()), "Vector2D negation identity");

	Vector2D subj23(subj17 * subj18), subj24(subj17 * subj18 / subj18);
	consistencyCheck(Math::equal(subj23.getX(), 10.f) && Math::equal(subj23.getY(), -48.f), "Vector2D multiplication");
	consistencyCheck(Math::equal(subj24.getX(), subj17.getX()) && Math::equal(subj24.getY(), subj17.getY()), "Vector2D multiplication identity");

	Vector2D subj25(subj17 * 5.f), subj26(subj17 * 5.f / 5.f);
	consistencyCheck(Math::equal(subj25.getX(), 25.f) && Math::equal(subj25.getY(), 30.f), "Vector2D scalar multiplication");
	consistencyCheck(Math::equal(subj26.getX(), subj17.getX()) && Math::equal(subj26.getY(), subj17.getY()), "Vector2D scalar multiplication identity");
	
	consistencyCheck(subj25 == subj25, "Vector2D self-equality");
	Vector2D subj27(5.f, 10.f), subj28(5.f, 10.f);
	consistencyCheck(subj27 == subj28, "Vector2D equality");


	Vector2D subj29(subj17), subj30(subj17), subj31(subj17), subj32(subj17), subj33(subj17), subj34(subj17);
	subj29 += subj18;
	subj30 -= subj18;
	subj31 *= subj18;
	subj32 *= 5.f;
	subj33 /= subj18;
	subj34 /= 5.f;

	Vector2D subj35(0.f, 0.f);
	subj35 = subj5;

	consistencyCheck(subj35.getX() == subj5.getX() && subj5.getY() == subj35.getY(), "Vector2D assignment");
	consistencyCheck(subj29 == subj17 + subj18, "Vector2D addition and assignment");
	consistencyCheck(subj30 == subj17 - subj18, "Vector2D subtraction and assignment");
	consistencyCheck(subj31 == subj17 * subj18, "Vector2D multiplication and assignment");
	consistencyCheck(subj32 == subj17 * 5.f, "Vector2D scalar multiplication and assignment");
	consistencyCheck(subj33 == subj17 / subj18, "Vector2D division and assignment");
	consistencyCheck(subj34 == subj17 / 5.f, "Vector2D scalar division and assignment");

	printf("Operator test done.\n");
	/*********************************************************/
	printf("Consistency test for Vector2D completed.\n\n");
}

void testVec3D()
{
	printf("Commencing consistency testing for Vector3D...\n");

	/*********************************************************/
	printf("Testing constructors...\n");
	Vector3D subj1(10.f, 20.f, 30.f);
	consistencyCheck(subj1.getX() == 10.f && subj1.getY() == 20.f && subj1.getZ() == 30.f, "Vector3D two-argument constructor");

	Vector3D subj2(5.f);
	consistencyCheck(subj2.getX() == 5.f && subj2.getY() == 5.f && subj2.getZ() == 5.f, "Vector3D scalar constructor");

	Real aux1[3] = { 1.f, 2.f, 3.f };
	Vector3D subj3(aux1);
	consistencyCheck(subj3.getX() == 1.f && subj3.getY() == 2.f && subj3.getZ() == 3.f, "Vector3D from ptr constructor");

	Vector3D copyFrom1;
	copyFrom1.setX(4.f);
	copyFrom1.setY(2.f);
	copyFrom1.setZ(1.f);
	Vector3D subj4(copyFrom1);
	consistencyCheck(subj4.getX() == 4.f && subj4.getY() == 2.f && subj4.getZ() == 1.f, "Vector3D copy constructor");

	printf("Constructor test done.\n");

	/*********************************************************/
	printf("Testing member function behavior...\n");

	Vector3D subj5(3.f, 4.f, 5.f);
	Real len5 = subj5.getLength();
	consistencyCheck(Math::equal(len5, 5.f * Sqr2), "Vector3D length computation");

	Vector3D subj6(0.f, 0.f, 0.f);
	Real len6 = subj6.getLength();
	consistencyCheck(Math::equal(len6, 0.f), "Vector3D zero length");

	subj5.setX(1.f);
	subj5.setY(1.f);
	subj5.setZ(1.f);
	Real len5_2 = subj5.getLength();
	consistencyCheck(Math::equal(len5_2, 2.f), "Vector3D correct length caching");

	Vector3D subj7(2.f, 5.f, 3.f);
	Real lenSq7 = subj7.getLengthSquared();
	consistencyCheck(Math::equal(lenSq7, 38.f), "Vector3D squared length computation");

	Vector3D subj8(0.f, 0.f, 0.f);
	Real lenSq8 = subj8.getLengthSquared();
	consistencyCheck(Math::equal(lenSq8, 0.f), "Vector3D zero squared length");

	subj8.setX(9.f);
	subj8.setY(6.f);
	subj8.setZ(2.f);
	Real lenSq8_2 = subj8.getLengthSquared();
	consistencyCheck(Math::equal(lenSq8_2, 121.f), "Vector3D correct squared length caching");

	Vector3D subj9(1.f, 1.f, 1.f);
	subj9.normalize();
	Real len9_man = Math::sqrt(subj9.getX() * subj9.getX() + subj9.getY() * subj9.getY() + subj9.getZ() * subj9.getZ());
	Real len9 = subj9.getLength();
	consistencyCheck(Math::equal(len9_man, 1.f), "Vector3D normalization length");
	consistencyCheck(Math::equal(len9, 1.f), "Vector3D reported normalization length");

	subj9.setX(4.f);
	Real len9_2 = subj9.getLength();
	consistencyCheck(Math::notEqual(len9_2, len9), "Vector3D reported denormalized length");

	Vector3D subj10(subj9.getNormalized());
	Real len10_man = Math::sqrt(subj10.getX() * subj10.getX() + subj10.getY() * subj10.getY() + subj10.getZ() * subj10.getZ());
	Real len10 = subj10.getLength();
	consistencyCheck(Math::equal(len10_man, 1.f), "Vector3D getNormalized() length");
	consistencyCheck(Math::equal(len10, 1.f), "Vector3D reported getNormalized() length");

	Vector3D subj11(1.f, 1.f, 1.f), subj12(-1.f, -1.f, -1.f);
	Real dot11_12 = subj11.dot(subj12), dot12_11 = subj12.dot(subj11);
	consistencyCheck(Math::equal(dot11_12, dot12_11), "Vector3D dot product commutativity");
	Real len = -1.f * subj11.getLength() * subj12.getLength();
	consistencyCheck(Math::equal(dot11_12, -1.f * subj11.getLength() * subj12.getLength()), "Vector3D perpendicular vector dot");

	Vector3D subj13(0.f, 1.f, 0.f), subj14(Aurora::Sqr2 / 2.f, Aurora::Sqr2 / 2.f, 0.f);
	Angle angle13_14 = subj13.getAngleTo(subj14), angle14_13 = subj14.getAngleTo(subj13);
	consistencyCheck(Math::equal(Degree(angle13_14), Degree(angle14_13)), "Vector3D getAngleTo() identity");
	consistencyCheck(Math::equal(Degree(angle13_14), 45.f), "Vector3D getAngleTo()");

	Vector3D subj15(0.f, 0.f, 0.f), subj16(3.f, 4.f, 5.f);
	Real distance15_16 = subj15.getDistanceTo(subj16), distance16_15 = subj16.getDistanceTo(subj15);
	consistencyCheck(Math::equal(distance15_16, distance16_15), "Vector3D distance commutativity");
	consistencyCheck(Math::equal(distance15_16, 5 * Sqr2), "Vector3D distance");

	Real distanceSq15_16 = subj15.getSquaredDistanceTo(subj16), distanceSq16_15 = subj16.getSquaredDistanceTo(subj15);
	consistencyCheck(Math::equal(distanceSq15_16, distanceSq16_15), "Vector3D squared distance commutativity");
	consistencyCheck(Math::equal(distanceSq15_16, 50.f), "Vector3D squared distance");

	printf("Member function test done.\n");
	/*********************************************************/
	printf("Testing operators...\n");
	Vector3D subj17(5.f, 6.f, 7.f), subj18(2.f, -8.f, -7.f);
	Vector3D subj19(subj17+subj18), subj18equiv_1(subj17+subj18-subj17);

	consistencyCheck(Math::equal(subj19.getX(), 7.f) && Math::equal(subj19.getY(), -2.f) && Math::equal(subj19.getZ(), 0.f), "Vector3D addition");
	consistencyCheck(Math::equal(subj18equiv_1.getX(), subj18.getX()) && Math::equal(subj18equiv_1.getY(), subj18.getY()), "Vector3D addition identity");

	Vector3D subj20(subj17-subj18), subj17equiv_1(subj17-subj18+subj18);
	consistencyCheck(Math::equal(subj20.getX(), 3.f) && Math::equal(subj20.getY(), 14.f) && Math::equal(subj20.getZ(), 14.f), "Vector3D subtraction");
	consistencyCheck(Math::equal(subj17equiv_1.getX(), subj17.getX()) && Math::equal(subj17equiv_1.getY(), subj17.getY()), "Vector3D subtraction identity");

	Vector3D subj21(-subj17), subj22(-subj21);
	consistencyCheck(Math::equal(subj21.getX(), -5.f) && Math::equal(subj21.getY(), -6.f) && Math::equal(subj21.getZ(), -7.f), "Vector3D negation");
	consistencyCheck(Math::equal(subj22.getX(), subj17.getX()) && Math::equal(subj22.getY(), subj17.getY()), "Vector3D negation identity");

	Vector3D subj23(subj17 * subj18), subj24(subj17 * subj18 / subj18);
	consistencyCheck(Math::equal(subj23.getX(), 10.f) && Math::equal(subj23.getY(), -48.f) && Math::equal(subj23.getZ(), -49.f), "Vector3D multiplication");
	consistencyCheck(Math::equal(subj24.getX(), subj17.getX()) && Math::equal(subj24.getY(), subj17.getY()), "Vector3D multiplication identity");

	Vector3D subj25(subj17 * 5.f), subj26(subj17 * 5.f / 5.f);
	consistencyCheck(Math::equal(subj25.getX(), 25.f) && Math::equal(subj25.getY(), 30.f) && Math::equal(subj25.getZ(), 35.f), "Vector3D scalar multiplication");
	consistencyCheck(Math::equal(subj26.getX(), subj17.getX()) && Math::equal(subj26.getY(), subj17.getY()), "Vector3D scalar multiplication identity");
	
	consistencyCheck(subj25 == subj25, "Vector3D self-equality");
	Vector3D subj27(5.f, 10.f, 2.f), subj28(5.f, 10.f, 2.f);
	consistencyCheck(subj27 == subj28, "Vector3D equality");


	Vector3D subj29(subj17), subj30(subj17), subj31(subj17), subj32(subj17), subj33(subj17), subj34(subj17);
	subj29 += subj18;
	subj30 -= subj18;
	subj31 *= subj18;
	subj32 *= 5.f;
	subj33 /= subj18;
	subj34 /= 5.f;

	Vector3D subj35(0.f, 0.f, 0.f);
	subj35 = subj5;

	consistencyCheck(subj35.getX() == subj5.getX() && subj5.getY() == subj35.getY(), "Vector3D assignment");
	consistencyCheck(subj29 == subj17 + subj18, "Vector3D addition and assignment");
	consistencyCheck(subj30 == subj17 - subj18, "Vector3D subtraction and assignment");
	consistencyCheck(subj31 == subj17 * subj18, "Vector3D multiplication and assignment");
	consistencyCheck(subj32 == subj17 * 5.f, "Vector3D scalar multiplication and assignment");
	consistencyCheck(subj33 == subj17 / subj18, "Vector3D division and assignment");
	consistencyCheck(subj34 == subj17 / 5.f, "Vector3D scalar division and assignment");

	printf("Operator test done.\n");
	/*********************************************************/
	printf("Consistency test for Vector3D completed.\n\n");
}


void testVec4D()
{
	printf("Commencing consistency testing for Vector4D...\n");

	/*********************************************************/
	printf("Testing constructors...\n");
	Vector4D subj1(10.f, 20.f, 30.f, 40.f);
	consistencyCheck(subj1.getX() == 10.f && subj1.getY() == 20.f && subj1.getZ() == 30.f && subj1.getW() == 40.f, "Vector4D two-argument constructor");

	Vector4D subj2(5.f);
	consistencyCheck(subj2.getX() == 5.f && subj2.getY() == 5.f && subj2.getZ() == 5.f && subj2.getW() == 5.f, "Vector4D scalar constructor");

	Real aux1[4] = { 1.f, 2.f, 3.f, 4.f };
	Vector4D subj3(aux1);
	consistencyCheck(subj3.getX() == 1.f && subj3.getY() == 2.f && subj3.getZ() == 3.f && subj3.getW(), "Vector4D from ptr constructor");

	Vector4D copyFrom1;
	copyFrom1.setX(4.f);
	copyFrom1.setY(2.f);
	copyFrom1.setZ(1.f);
	copyFrom1.setW(3.f);
	Vector4D subj4(copyFrom1);
	consistencyCheck(subj4.getX() == 4.f && subj4.getY() == 2.f && subj4.getZ() == 1.f && subj4.getW() == 3.f, "Vector4D copy constructor");

	printf("Constructor test done.\n");

	/*********************************************************/
	printf("Testing member function behavior...\n");

	Vector4D subj5(3.f, 4.f, 5.f, 6.f);
	Real len5 = subj5.getLength();
	consistencyCheck(Math::equal(len5, Math::sqrt(86.f)), "Vector4D length computation");

	Vector4D subj6(0.f, 0.f, 0.f, 0.f);
	Real len6 = subj6.getLength();
	consistencyCheck(Math::equal(len6, 0.f), "Vector4D zero length");

	subj5.setX(1.f);
	subj5.setY(1.f);
	subj5.setZ(1.f);
	subj5.setW(1.f);
	Real len5_2 = subj5.getLength();
	consistencyCheck(Math::equal(len5_2, 2.f), "Vector4D correct length caching");

	Vector4D subj7(2.f, 5.f, 3.f, 1.f);
	Real lenSq7 = subj7.getLengthSquared();
	consistencyCheck(Math::equal(lenSq7, 39.f), "Vector4D squared length computation");

	Vector4D subj8(0.f, 0.f, 0.f, 0.f);
	Real lenSq8 = subj8.getLengthSquared();
	consistencyCheck(Math::equal(lenSq8, 0.f), "Vector4D zero squared length");

	subj8.setX(9.f);
	subj8.setY(6.f);
	subj8.setZ(2.f);
	subj8.setW(4.f);
	Real lenSq8_2 = subj8.getLengthSquared();
	consistencyCheck(Math::equal(lenSq8_2, 137.f), "Vector4D correct squared length caching");

	Vector4D subj9(1.f, 1.f, 1.f, 1.f);
	subj9.normalize();
	Real len9_man = Math::sqrt(subj9.getX() * subj9.getX() + subj9.getY() * subj9.getY() + subj9.getZ() * subj9.getZ() + subj9.getW() * subj9.getW());
	Real len9 = subj9.getLength();
	consistencyCheck(Math::equal(len9_man, 1.f), "Vector4D normalization length");
	consistencyCheck(Math::equal(len9, 1.f), "Vector4D reported normalization length");

	subj9.setX(4.f);
	Real len9_2 = subj9.getLength();
	consistencyCheck(Math::notEqual(len9_2, len9), "Vector4D reported denormalized length");

	Vector4D subj10(subj9.getNormalized());
	Real len10_man = Math::sqrt(subj10.getX() * subj10.getX() + subj10.getY() * subj10.getY() + subj10.getZ() * subj10.getZ() + subj10.getW() * subj10.getW());
	Real len10 = subj10.getLength();
	consistencyCheck(Math::equal(len10_man, 1.f), "Vector4D getNormalized() length");
	consistencyCheck(Math::equal(len10, 1.f), "Vector4D reported getNormalized() length");

	Vector4D subj11(1.f, 1.f, 1.f, 1.f), subj12(-1.f, -1.f, -1.f, -1.f);
	Real dot11_12 = subj11.dot(subj12), dot12_11 = subj12.dot(subj11);
	consistencyCheck(Math::equal(dot11_12, dot12_11), "Vector4D dot product commutativity");
	Real len = -1.f * subj11.getLength() * subj12.getLength();
	consistencyCheck(Math::equal(dot11_12, -1.f * subj11.getLength() * subj12.getLength()), "Vector4D perpendicular vector dot");

	Vector4D subj13(0.f, 1.f, 0.f, 0.f), subj14(Aurora::Sqr2 / 2.f, Aurora::Sqr2 / 2.f, 0.f, 0.f);
	Angle angle13_14 = subj13.getAngleTo(subj14), angle14_13 = subj14.getAngleTo(subj13);
	consistencyCheck(Math::equal(Degree(angle13_14), Degree(angle14_13)), "Vector4D getAngleTo() identity");
	consistencyCheck(Math::equal(Degree(angle13_14), 45.f), "Vector4D getAngleTo()");

	Vector4D subj15(0.f, 0.f, 0.f, 0.f), subj16(3.f, 4.f, 5.f, 6.f);
	Real distance15_16 = subj15.getDistanceTo(subj16), distance16_15 = subj16.getDistanceTo(subj15);
	consistencyCheck(Math::equal(distance15_16, distance16_15), "Vector4D distance commutativity");
	consistencyCheck(Math::equal(distance15_16, Math::sqrt(3*3+4*4+5*5+6*6)), "Vector4D distance");

	Real distanceSq15_16 = subj15.getSquaredDistanceTo(subj16), distanceSq16_15 = subj16.getSquaredDistanceTo(subj15);
	consistencyCheck(Math::equal(distanceSq15_16, distanceSq16_15), "Vector4D squared distance commutativity");
	consistencyCheck(Math::equal(distanceSq15_16, 3*3+4*4+5*5+6*6), "Vector4D squared distance");

	printf("Member function test done.\n");
	/*********************************************************/
	printf("Testing operators...\n");
	Vector4D subj17(5.f, 6.f, 7.f, 8.f), subj18(2.f, -8.f, -7.f, 3.f);
	Vector4D subj19(subj17+subj18), subj18equiv_1(subj17+subj18-subj17);

	consistencyCheck(Math::equal(subj19.getX(), 7.f) && Math::equal(subj19.getY(), -2.f) && Math::equal(subj19.getZ(), 0.f) && Math::equal(subj19.getW(), 11.f), "Vector4D addition");
	consistencyCheck(Math::equal(subj18equiv_1.getX(), subj18.getX()) && Math::equal(subj18equiv_1.getY(), subj18.getY()) && Math::equal(subj18equiv_1.getZ(), subj18.getZ()) && Math::equal(subj18equiv_1.getW(), subj18.getW()), "Vector4D addition identity");

	Vector4D subj20(subj17-subj18), subj17equiv_1(subj17-subj18+subj18);
	consistencyCheck(Math::equal(subj20.getX(), 3.f) && Math::equal(subj20.getY(), 14.f) && Math::equal(subj20.getZ(), 14.f) && Math::equal(subj20.getW(), 5.f), "Vector4D subtraction");
	consistencyCheck(Math::equal(subj17equiv_1.getX(), subj17.getX()) && Math::equal(subj17equiv_1.getY(), subj17.getY()), "Vector4D subtraction identity");

	Vector4D subj21(-subj17), subj22(-subj21);
	consistencyCheck(Math::equal(subj21.getX(), -5.f) && Math::equal(subj21.getY(), -6.f) && Math::equal(subj21.getZ(), -7.f) && Math::equal(subj21.getW(), -8.f), "Vector4D negation");
	consistencyCheck(Math::equal(subj22.getX(), subj17.getX()) && Math::equal(subj22.getY(), subj17.getY()), "Vector4D negation identity");

	Vector4D subj23(subj17 * subj18), subj24(subj17 * subj18 / subj18);
	consistencyCheck(Math::equal(subj23.getX(), 10.f) && Math::equal(subj23.getY(), -48.f) && Math::equal(subj23.getZ(), -49.f) && Math::equal(subj23.getW(), 24.f), "Vector4D multiplication");
	consistencyCheck(Math::equal(subj24.getX(), subj17.getX()) && Math::equal(subj24.getY(), subj17.getY()), "Vector4D multiplication identity");

	Vector4D subj25(subj17 * 5.f), subj26(subj17 * 5.f / 5.f);
	consistencyCheck(Math::equal(subj25.getX(), 25.f) && Math::equal(subj25.getY(), 30.f) && Math::equal(subj25.getZ(), 35.f) && Math::equal(subj25.getW(), 40.f), "Vector4D scalar multiplication");
	consistencyCheck(Math::equal(subj26.getX(), subj17.getX()) && Math::equal(subj26.getY(), subj17.getY()), "Vector4D scalar multiplication identity");
	
	consistencyCheck(subj25 == subj25, "Vector4D self-equality");
	Vector4D subj27(5.f, 10.f, 2.f, 8.f), subj28(5.f, 10.f, 2.f, 8.f);
	consistencyCheck(subj27 == subj28, "Vector4D equality");


	Vector4D subj29(subj17), subj30(subj17), subj31(subj17), subj32(subj17), subj33(subj17), subj34(subj17);
	subj29 += subj18;
	subj30 -= subj18;
	subj31 *= subj18;
	subj32 *= 5.f;
	subj33 /= subj18;
	subj34 /= 5.f;

	Vector4D subj35(0.f, 0.f, 0.f, 0.f);
	subj35 = subj5;

	consistencyCheck(subj35.getX() == subj5.getX() && subj5.getY() == subj35.getY(), "Vector4D assignment");
	consistencyCheck(subj29 == subj17 + subj18, "Vector4D addition and assignment");
	consistencyCheck(subj30 == subj17 - subj18, "Vector4D subtraction and assignment");
	consistencyCheck(subj31 == subj17 * subj18, "Vector4D multiplication and assignment");
	consistencyCheck(subj32 == subj17 * 5.f, "Vector4D scalar multiplication and assignment");
	consistencyCheck(subj33 == subj17 / subj18, "Vector4D division and assignment");
	consistencyCheck(subj34 == subj17 / 5.f, "Vector4D scalar division and assignment");

	printf("Operator test done.\n");
	/*********************************************************/
	printf("Consistency test for Vector4D completed.\n\n");
}

void testMat3D()
{

}

int main()
{
	testVec2D();
	testVec3D();
	testVec4D();

	system("pause >nul");
}