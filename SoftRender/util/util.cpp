#include "util.h"

MATRIX4X4 lookAt(float eyeX, float eyeY, float eyeZ,
		float centerX, float centerY, float centerZ,
		float upX, float upY, float upZ) {
	VECTOR3D eye;
	eye.x=eyeX;
	eye.y=eyeY;
	eye.z=eyeZ;
	VECTOR3D center;
	center.x=centerX;
	center.y=centerY;
	center.z=centerZ;
	VECTOR3D up;
	up.x=upX;
	up.y=upY;
	up.z=upZ;

	VECTOR3D f=center - eye;
	f.Normalize();
	VECTOR3D s=f.CrossProduct(up);
	s.Normalize();
	VECTOR3D u=s.CrossProduct(f);
	u.Normalize();

	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0]=s.x;
	mat.entries[4]=s.y;
	mat.entries[8]=s.z;
	mat.entries[1]=u.x;
	mat.entries[5]=u.y;
	mat.entries[9]=u.z;
	mat.entries[2]=-f.x;
	mat.entries[6]=-f.y;
	mat.entries[10]=-f.z;
	mat.entries[12]=-(s.DotProduct(eye));
	mat.entries[13]=-(u.DotProduct(eye));
	mat.entries[14]=f.DotProduct(eye);
	return mat;
}

MATRIX4X4 perspective(float fovy,float aspect,float zNear,float zFar) {
	float rFovy=fovy*PI/180;
	float tanHalfFovy=tanf(rFovy/2);

	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0]=1/(aspect * tanHalfFovy);
	mat.entries[5]=1/tanHalfFovy;
	mat.entries[10]=-(zFar + zNear)/(zFar - zNear);
	mat.entries[11]=-1;
	mat.entries[14]=-2*zFar*zNear/(zFar - zNear);
	mat.entries[15]=0;
	return mat;
}

MATRIX4X4 ortho(float left, float right, float bottom, float top, float n, float f) {
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0]=2.0f/(right-left);
	mat.entries[5]=2.0f/(top-bottom);
	mat.entries[10]=-2.0f/(f-n);
	mat.entries[12]=-(right+left)/(right-left);
	mat.entries[13]=-(top+bottom)/(top-bottom);
	mat.entries[14]=-(f+n)/(f-n);
	return mat;
}

int project(float objX, float objY, float objZ,
		const float* model, const float* proj, const int* view,
		float* winX, float* winY, float* winZ) {
	VECTOR4D point;
	point.x=objX;
	point.y=objY;
	point.z=objZ;
	point.w=1.0f;

	MATRIX4X4 modelMat,projMat;
	modelMat = model;
	projMat = proj;
	point=modelMat * point;
	point=projMat * point;
	point /= point.w;

	VECTOR4D bias(0.5,0.5,0.5,0.5);
	point = (point * 0.5);
	point = (point + bias);

	point.x = point.x * view[2] + view[0];
	point.y = point.y * view[3] + view[1];
	*winX=point.x;
	*winY=point.y;
	*winZ=point.z;
	return 1;
}

MATRIX4X4 rotateX(float angle) {
	float radian=angle*A2R;
	float sinA=sinf(radian);
	float cosA=cosf(radian);
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[5]=cosA;
	mat.entries[6]=sinA;
	mat.entries[9]=-sinA;
	mat.entries[10]=cosA;
	return mat;
}

MATRIX4X4 rotateY(float angle) {
	float radian=angle*A2R;
	float sinA=sinf(radian);
	float cosA=cosf(radian);
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0]=cosA;
	mat.entries[2]=-sinA;
	mat.entries[8]=sinA;
	mat.entries[10]=cosA;
	return mat;
}

MATRIX4X4 rotateZ(float angle) {
	float radian=angle*A2R;
	float sinA=sinf(radian);
	float cosA=cosf(radian);
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0]=cosA;
	mat.entries[1]=sinA;
	mat.entries[4]=-sinA;
	mat.entries[5]=cosA;
	return mat;
}

MATRIX4X4 scale(float size) {
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0]=size;
	mat.entries[5]=size;
	mat.entries[10]=size;
	return mat;
}

MATRIX4X4 translate(float tx,float ty,float tz) {
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[12]=tx;
	mat.entries[13]=ty;
	mat.entries[14]=tz;
	return mat;
}

VECTOR4D mul(VECTOR4D a,VECTOR4D b) {
	VECTOR4D vec;
	vec.x=a.x*b.x;
	vec.y=a.y*b.y;
	vec.z=a.z*b.z;
	vec.w=a.w*b.w;
	return vec;
}

void interpolate3f(float pa,float pb,float pc,
		float a,float b,float c,
		float& result) {
	result=pa*a+pb*b+pc*c;
}

void interpolate2f(float pa,float pb,
		float a,float b,
		float& result) {
	result=pa*a+pb*b;
}

float calcZPara(float v1z,float v2z,float z) {
	return (z-v2z)/(v1z-v2z);
}

VECTOR3D calcParaEqu(VECTOR3D vect1,VECTOR3D vect2,float param) {
	VECTOR3D result;
	result=param*(vect1-vect2)+vect2;
	return result;
}
