#include "DirectX12Math.h"
#include "WinApp.h"
#include <cmath>
#include <cassert>
#include <time.h>

 /**
 * @file DirectX12Math.cpp
 * @brief DirectX12Mathの処理について書いてあります
 */

using namespace MyMath;

namespace MyMathUtility
{
	float Vector2Length(const Vector2& v)
	{
		return (float)std::sqrt(v.x * v.x + v.y * v.y);
	}

	float Vector3Length(const Vector3& v)
	{
		return (float)std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	Vector3 MakeNormalize(Vector3 v)
	{
		float len = Vector3Length(v);
		if (len != 0) {
			return v /= len;
		}
		return v;
	}

	MyMath::Vector2 MakeNormalizeVec2(MyMath::Vector2 v)
	{
		float len = Vector2Length(v);
		if (len != 0) {
			return v /= len;
		}
		return v;
	}

	Matrix4 MakeIdentity()
	{
		Matrix4 matIdentity;
		matIdentity.m[0][0] = 1.0f;
		matIdentity.m[1][1] = 1.0f;
		matIdentity.m[2][2] = 1.0f;
		matIdentity.m[3][3] = 1.0f;

		return matIdentity;
	}

	Matrix4 MakeScaling(const Vector3& scale)
	{
		Matrix4 matScale;
		matScale.m[0][0] = scale.x;
		matScale.m[1][1] = scale.y;
		matScale.m[2][2] = scale.z;
		matScale.m[3][3] = 1.0f;

		return matScale;
	}

	Matrix4 MakeRotation(const Vector3& rot)
	{
		Matrix4 matRotX;
		matRotX = MakeIdentity();
		matRotX.m[1][1] = cos(rot.x);
		matRotX.m[1][2] = sin(rot.x);
		matRotX.m[2][1] = -sin(rot.x);
		matRotX.m[2][2] = cos(rot.x);

		Matrix4 matRotY;
		matRotY = MakeIdentity();
		matRotY.m[0][0] = cos(rot.y);
		matRotY.m[0][2] = -sin(rot.y);
		matRotY.m[2][0] = sin(rot.y);
		matRotY.m[2][2] = cos(rot.y);

		Matrix4 matRotZ;
		matRotZ = MakeIdentity();
		matRotZ.m[0][0] = cos(rot.z);
		matRotZ.m[0][1] = sin(rot.z);
		matRotZ.m[1][0] = -sin(rot.z);
		matRotZ.m[1][1] = cos(rot.z);

		Matrix4 matRot;
		matRot = MakeIdentity();

		matRot *= matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;
		return matRot;
	}

	Matrix4 MakeTranslation(const Vector3& trans)
	{
		Matrix4 matTrans;
		matTrans = MakeIdentity();
		matTrans.m[3][0] = trans.x;
		matTrans.m[3][1] = trans.y;
		matTrans.m[3][2] = trans.z;
		return matTrans;
	}

	//sin、cosを両方出す
	void SinCos(float& sin_, float& cos_, float angle)
	{
		sin_ = Sin(angle);
		cos_ = Cos(angle);
	}

	void MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, MyMath::Matrix4& matrix)
	{
		float width = 1.0f / (right - left);
		float height = 1.0f / (top - bottom);
		float range = 1.0f / (far_ - near_);

		matrix.m[0][0] = width * 2;

		matrix.m[1][1] = height * 2;

		matrix.m[2][2] = range;

		matrix.m[3][0] = -(left + right) * width;
		matrix.m[3][1] = -(top + bottom) * height;
		matrix.m[3][2] = range * -near_;
		matrix.m[3][3] = 1.0f;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] = 0.0f;
	}

	void MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix)
	{
		float width = 1.0f / (right - left);
		float height = 1.0f / (top - bottom);
		float range = 1.0f / (far_ - near_);

		matrix.m[0][0] = width * 2;

		matrix.m[1][1] = height * 2;

		matrix.m[2][2] = range;

		matrix.m[3][0] = -(left + right) * width;
		matrix.m[3][1] = -(top + bottom) * height;
		matrix.m[3][2] = range * -near_;
		matrix.m[3][3] = 1.0f;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] = 0.0f;

	}

	Matrix4 MakeLookAtLH(Vector3& eye, Vector3& target, Vector3& up)
	{
		Vector3 zVec = target - eye;
		zVec.normalize();

		Vector3 xVec = up.cross(zVec);
		xVec.normalize();

		Vector3 yVec = zVec.cross(xVec);
		yVec.normalize();

		Matrix4 matrix;
		matrix.m[0][0] = xVec.x;
		matrix.m[0][1] = xVec.y;
		matrix.m[0][2] = xVec.z;

		matrix.m[1][0] = yVec.x;
		matrix.m[1][1] = yVec.y;
		matrix.m[1][2] = yVec.z;

		matrix.m[2][0] = zVec.x;
		matrix.m[2][1] = zVec.y;
		matrix.m[2][2] = zVec.z;

		matrix.m[3][0] = eye.x;
		matrix.m[3][1] = eye.y;
		matrix.m[3][2] = eye.z;

		return matrix;
	}

	Matrix4 MakePerspective(float fogAngleY, float aspectRatio, float nearZ, float farZ)
	{
		// アスペクト比を作成

		Matrix4 matrix;
		float sinFov = 0.0f;
		float cosFov = 0.0f;
		SinCos(sinFov, cosFov, 0.5f * fogAngleY);

		float range = farZ / (farZ - nearZ);
		float height = cosFov / sinFov;

		matrix.m[0][0] = height / aspectRatio;

		matrix.m[1][1] = cosFov / sinFov;

		matrix.m[2][2] = range;
		matrix.m[2][3] = 1.0f;

		matrix.m[3][2] = -range * nearZ;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] =
			matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][3] = 0.0f;

		return matrix;
	}

	Matrix4 MakeInverse(Matrix4& mat)
	{
		//掃き出し法を行う行列
		float sweep[4][8]{};
		//定数倍用
		float constTimes = 0.0f;
		//許容する誤差
		float MAX_ERR = 1e-10f;
		//戻り値用
		Matrix4 retMat;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				sweep[(uint32_t)i][ ( uint32_t ) j] = mat.m[ ( uint32_t ) i][ ( uint32_t ) j];

				//sweepの右側に単位行列をセット
				sweep[ ( uint32_t ) i][4 + ( uint32_t ) j] = MakeIdentity().m[ ( uint32_t ) i][ ( uint32_t ) j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (int i = 0; i < 4; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float max = std::fabs(sweep[i][i]);
			int maxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (int j = i + 1; j < 4; j++)
			{
				if (std::fabs(sweep[j][i]) > max)
				{
					max = std::fabs(sweep[j][i]);
					maxIndex = j;
				}
			}

			if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
			{
				//逆行列は求められない
				return MakeIdentity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != maxIndex)
			{
				for (int j = 0; j < 8; j++)
				{
					float tmp = sweep[maxIndex][j];
					sweep[maxIndex][j] = sweep[i][j];
					sweep[i][j] = tmp;
				}
			}

			//sweep[i][i]に掛けると1になる値を求める
			constTimes = 1 / sweep[i][i];

			//操作(2):p行目をa倍する
			for (int j = 0; j < 8; j++)
			{
				//これによりsweep[i][i]が1になる
				sweep[i][j] *= constTimes;
			}

			//操作(3)によりi行目以外の行のi列目を0にする
			for (int j = 0; j < 4; j++)
			{
				if (j == i)
				{
					//i行目はそのまま
					continue;
				}

				//i行目に掛ける値を求める
				constTimes = -sweep[j][i];

				for (int k = 0; k < 8; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりsweep[j][i]が0になる
					sweep[j][k] += sweep[i][k] * constTimes;
				}
			}
		}

		//sweepの右半分がmatの逆行列
		for (size_t i = 0; i < 4; i++)
		{
			for ( size_t j = 0; j < 4; j++)
			{
				retMat.m[i][j] = sweep[i][4 + j];
			}
		}

		return retMat;
	}

	MyMath::Matrix4 MakeViewport(const MyMath::Matrix4& viewport)
	{
		float width = static_cast<float>(WinApp::GetInstance()->window_width) / 2;
		float height = static_cast<float>(WinApp::GetInstance()->window_height) / 2;

		MyMath::Matrix4 m = viewport;	
		m = { width, 0, 0, 0, 0, -height, 0, 0, 0, 0, 1, 0, width, height, 0, 1 };

		return m;
	}

	MyMath::Vector3 MakeWDivision(MyMath::Vector3& worldPos, MyMath::Matrix4& matrix)
	{
		float w = worldPos.x * matrix.m[0][3] + worldPos.y * matrix.m[1][3] + worldPos.z * matrix.m[2][3] + matrix.m[3][3];

		MyMath::Vector3 result = 
		{
			(worldPos.x * matrix.m[0][0] + worldPos.y * matrix.m[1][0] + worldPos.z * matrix.m[2][0] + matrix.m[3][0]) / w,
			(worldPos.x * matrix.m[0][1] + worldPos.y * matrix.m[1][1] + worldPos.z * matrix.m[2][1] + matrix.m[3][1]) / w,
			(worldPos.x * matrix.m[0][2] + worldPos.y * matrix.m[1][2] + worldPos.z * matrix.m[2][2] + matrix.m[3][2]) / w
		};

		result /= result.z;

		return result;
	}

	
	bool Approximately(float a, float b)
	{
		float tmp = 1e-06f * Max(fabs(a), fabs(b));

		float tmp2 = EPSILON * 8.0f;

		if (fabs(b - a) < Max(tmp, tmp2))
		{
			return true;
		}

		return false;
	}

	float Sin(float sin)
	{
		return std::sinf(sin);
	}

	float Asin(float sin)
	{
		return std::asinf(sin);
	}

	float Cos(float cos)
	{
		return std::cosf(cos);
	}

	float Acos(float cos)
	{
		return std::acosf(cos);
	}

	float Tan(float tan)
	{
		return std::tanf(tan);
	}

	float Atan(float tan)
	{
		return std::atanf(tan);
	}

	float Atan2(float y, float x)
	{
		return std::atan2f(y, x);
	}

	float Max(float a, float b)
	{
		return (((a) > (b)) ? (a) : (b));
	}

	float Min(float a, float b)
	{
		return (((a) < (b)) ? (a) : (b));
	}

	void Complement(float& x1, float x2, float flame)
	{
		//距離を出す
		float distanceX = x2 - x1;
		//距離をflameで割った値
		float dividedDistanceX = distanceX / flame;

		//距離をflameで割った値を足す
		x1 += dividedDistanceX;
	}

	float Clamp(float Value, const float low, const float high)
	{
		if (high < Value)
		{
			Value = high;
		}

		if (Value < low)
		{
			Value = low;
		}

		return Value;
	}

	float Clamp0To1(float val)
	{
		if (val < 0.0f)
		{
			return 0.0f;
		}
		if (val > 1.0f)
		{
			return 1.0f;
		}

		return val;
	}

	// 線形補間(0～1)
	float Lerp(float a, float b, float t)
	{
		return a + (b - a) * Clamp0To1(t);
	}

	Vector3 Lerp(Vector3 a, Vector3 b, float t)
	{
		Vector3 tmp;
		tmp.x = a.x + (b.x - a.x) * Clamp0To1(t);
		tmp.y = a.y + (b.y - a.y) * Clamp0To1(t);
		tmp.z = a.z + (b.z - a.z) * Clamp0To1(t);
		return tmp;
	}

	Vector3 HermiteGetPoint(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, float t)
	{
		Vector3 position = 0.5 * ((2 * p1 + (-p0 + p2) * t) +
			(2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) +
			(-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

		return position;
	}

	float SimpleHarmonicMotion(float time, float amplitude, float period)
	{
		return amplitude * sinf((2 *PI) * time / period);
	}

	float EaseOutCubic(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = startPoint - endPoint;

		return endPoint + len * pow(1 - (nowFlame / endFlame), 3.0f);
	}

	/*float EaseOutCirc(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		return sqrt(1 - pow(nowFlame / endFlame - 1.0f, 2.0f)) + startPoint;
	}*/

	float EaseInSine(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = startPoint - endPoint;

		return endPoint + len * cos(((nowFlame / endFlame) * PI) / 2.0f);
	}

	float EaseOutSine(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = endPoint - startPoint;

		return startPoint + len * sin(((nowFlame / endFlame) * PI) / 2.0f);
	}

	float EaseOutQuint(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = startPoint - endPoint;

		return endPoint + len * pow(1 - (nowFlame / endFlame), 5.0f);
	}

	float EaseInExpo(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = startPoint - endPoint;

		return endPoint + len * (nowFlame / endFlame) == 0.0f ? 0.0f : pow(2.0f, 10.0f * (nowFlame / endFlame) - 10.0f);
	}

	float EaseInOutBack(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		float len = endPoint - startPoint;

		return startPoint + len * (nowFlame / endFlame) < 0.5f
			? len * ((pow(2.0f * (nowFlame / endFlame), 2.0f) * ((c2 + 1.0f) * 2.0f * (nowFlame / endFlame) - c2)) / 2.0f)
			: endPoint + len * ((pow(2.0f * (nowFlame / endFlame) - 2.0f, 2.0f) * ((c2 + 1.0f) * ((nowFlame / endFlame) * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f);
	}

	MyMath::Quaternion EulerToQuaternion(MyMath::Vector3 rot)
	{
		MyMath::Quaternion result;
		float twice = 2.0f;

		//回転軸(x=roll,y=pitch,z=yaw)
		float cosRoll = cosf(rot.x / twice);
		float sinRoll = sin(rot.x / twice);
		float cosPitch = cosf(rot.y / twice);
		float sinPitch = sin(rot.y / twice);
		float cosYaw = cosf(rot.z / twice);
		float sinYaw = sin(rot.z / twice);

		result.w_ = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		result.x_ = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		result.y_ = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		result.z_ = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

		return result;
	}

	/*int GetRandom(int min,int max)
	{
		
		int result;

		if ( randFlag == false )
		{
			srand(( unsigned int ) time(NULL));
			randFlag = true;
		}

		result = min + ( int ) ( rand() * ( max - min + 1.0 ) / ( 1.0 + RAND_MAX ) );

		return result;
	}*/
}

namespace MyMath
{
	Vector3 Vec3Mat4Mul(const Vector3& vec, const Matrix4& mat)
	{
		Vector3 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];
		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];
		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

		return retVec;
	}

	Vector3 CatMullRomSpline(std::vector<Vector3>& points, float t)
	{

		float length = static_cast<float>(points.size());
		float progress = (length - 1) * t;
		float index = std::floor(progress);
		float weight = progress - index;

		if (MyMathUtility::Approximately(weight, 0.0f) && index >= length - 1)
		{
			index = length - 2;
			weight = 1;
		}

		Vector3 p0 = points[static_cast<size_t>(index)];
		Vector3 p1 = points[static_cast<size_t>(index + 1.0f)];
		Vector3 p2;
		Vector3 p3;

		if (index > 0.0f)
		{
			p2 = 0.5f * (points[static_cast<size_t>(index + 1.0f)] - points[static_cast<size_t>(index - 1.0f)]);
		}
		else
		{
			p2 = points[static_cast<size_t>(index + 1.0f)] - points[static_cast<size_t>(index)];
		}

		if (index < length - 2.0f)
		{
			p3 = 0.5f * (points[static_cast<size_t>(index + 2.0f)] - points[static_cast<size_t>(index)]);
		}
		else
		{
			p3 = points[static_cast<size_t>(index + 1.0f)] - points[static_cast<size_t>(index)];
		}

		return MyMathUtility::HermiteGetPoint(p0, p1, p2, p3, weight);
	}

	float LenSegLineOfSeparateAxis(Vector3* sep, Vector3* e1, Vector3* e2, Vector3* e3)
	{
		// 3つの内積の絶対値の和で投影線分長を計算
		float r1 = fabs(sep->dot(*e1));
		float r2 = fabs(sep->dot(*e2));
		float r3 = e3 ? (fabs(sep->dot(*e3))) : 0;
		return r1 + r2 + r3;
	}
}
