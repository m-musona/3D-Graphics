#pragma once

#include "GameProgCpp/Math.h"
#include <vector>

struct LineSegment
{
	LineSegment(const Vector3& start, const Vector3& end);
	// Get point along segment where 0 <= t <= 1
	Vector3 PointOnSegment(float t) const;
	// Get minimum distance squared between point and line segment
	float MinDistSq(const Vector3& point) const;
	// Get MinDistSq between two line segments
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	Vector3 mStart;
	Vector3 mEnd;
};

struct Plane
{
	Plane(const Vector3& normal, float d);
	// Construct plane from three points
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);
	// Get the signed distance between the point and the plane
	float SignedDist(const Vector3& point) const;

	Vector3 mNormal;
	float mD;
};

struct SphereColl
{
	SphereColl(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;

	Vector3 mCenter;
	float mRadius;
};

struct AxisAlignedBoundingBox
{
	AxisAlignedBoundingBox(const Vector3& min, const Vector3& max);
	// Update min and max accounting for this point
	// (used when loading a model)
	void UpdateMinMax(const Vector3& point);
	// Rotated by a quaternion
	void Rotate(const Quaternion& q);
	bool Contains(const Vector3& point) const;
	float MinDistSq(const Vector3& point) const;

	Vector3 mMin;
	Vector3 mMax;
};

struct OrientedBoundingBox
{
	Vector3 mCenter;
	Quaternion mRotation;
	Vector3 mExtents;
};

struct Capsule
{
	Capsule(const Vector3& start, const Vector3& end, float radius);
	// Get point along segment where 0 <= t <= 1
	Vector3 PointOnSegment(float t) const;
	bool Contains(const Vector3& point) const;

	LineSegment mSegment;
	float mRadius;
};

struct ConvexPolygon
{
	bool Contains(const Vector2& point) const;
	// Vertices have a clockwise ordering
	std::vector<Vector2> mVertices;
};

// Intersection functions
bool Intersect(const SphereColl& a, const SphereColl& b);
bool Intersect(const AxisAlignedBoundingBox& a, const AxisAlignedBoundingBox& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const SphereColl& s, const AxisAlignedBoundingBox& box);

bool Intersect(const LineSegment& l, const SphereColl& s, float& outT);
bool Intersect(const LineSegment& l, const Plane& p, float& outT);
bool Intersect(const LineSegment& l, const AxisAlignedBoundingBox& b, float& outT, Vector3& outNorm);

bool SweptSphere(const SphereColl& P0, const SphereColl& P1, const SphereColl& Q0, const SphereColl& Q1, float& t);