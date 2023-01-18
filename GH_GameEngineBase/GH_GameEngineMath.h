#pragma once
//분류 :
//용도 :
//설명 :

class GH_float4
{
public:
	static const GH_float4 ZERO;
	static const GH_float4 LEFT;
	static const GH_float4 RIGHT;
	static const GH_float4 UP;
	static const GH_float4 DOWN;
	//이동을 위한 변수

public:
	//unnamed union을 선언
	//내부의 메모리구조를 struct로 묶는것과 같이 사용해서 union 방식으로 구성해준다
	//실수 값 4개를 가지고잇는 클래스
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
			//struct로 묶인 16바이트를
			// x/y/z/w 4바이트 단위로 나눔

		};

		struct 
		{
			float r;
			float g;
			float b;
			float a;
		};

	};

	GH_float4 operator+(const GH_float4 _other) const
	{
		GH_float4 ReturnValue;

		ReturnValue.x = this->x + _other.x;
		ReturnValue.y = this->y + _other.y;
		ReturnValue.z = this->z + _other.z;
		ReturnValue.w = this->w + _other.w;
		return ReturnValue;
	}

	GH_float4 operator-(const GH_float4 _other) const
	{
		GH_float4 ReturnValue;

		ReturnValue.x = this->x - _other.x;
		ReturnValue.y = this->y - _other.y;
		ReturnValue.z = this->z - _other.z;
		ReturnValue.w = this->w - _other.w;
		return ReturnValue;
	}

	GH_float4 operator*(const float _other) const
	{
		GH_float4 ReturnValue;

		ReturnValue.x = this->x * _other;
		ReturnValue.y = this->y * _other;
		ReturnValue.z = this->z * _other;
		ReturnValue.w = this->w * _other;
		return ReturnValue;
	}



	GH_float4 operator*(const GH_float4 _other) const
	{
		GH_float4 ReturnValue;

		ReturnValue.x = this->x * _other.x;
		ReturnValue.y = this->y * _other.y;
		ReturnValue.z = this->z * _other.z;
		ReturnValue.w = this->w * _other.w;
		return ReturnValue;
	}

	GH_float4 operator/(const GH_float4 _other) const
	{
		GH_float4 ReturnValue;

		ReturnValue.x = this->x / _other.x;
		ReturnValue.y = this->y / _other.y;
		ReturnValue.z = this->z / _other.z;
		ReturnValue.w = this->w / _other.w;
		return ReturnValue;
	}


	GH_float4& operator+=(const GH_float4 _other)
	{
		this->x += _other.x;
		this->y += _other.y;
		this->z += _other.z;
		this->w += _other.w;
		return *this;
	}

	GH_float4& operator-=(const GH_float4 _other)
	{
		this->x -= _other.x;
		this->y -= _other.y;
		this->z -= _other.z;
		this->w -= _other.w;
		return *this;
	}

	GH_float4& operator*=(const GH_float4 _other)
	{
		this->x *= _other.x;
		this->y *= _other.y;
		this->z *= _other.z;
		this->w *= _other.w;
		return *this;
	}

	GH_float4& operator/=(const GH_float4 _other)
	{
		this->x /= _other.x;
		this->y /= _other.y;
		this->z /= _other.z;
		this->w /= _other.w;
		return *this;
	}

	// 대입연산자
	GH_float4& operator=(const GH_float4& _other)
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;
		w = _other.w;

		return *this;
	}

public:
	int ix() const
	{
		return static_cast<int>(x);
		//float을 int로 형변환 해주는 함수
	}

	int iy() const
	{
		return static_cast<int>(y);
	}

	int iz() const
	{
		return static_cast<int>(z);
	}


	float hx() const
	{
		return x * 0.5f;
	}

	float hy() const
	{
		return y * 0.5f;
	}

	float hz() const
	{
		return z * 0.5f;
	}

	GH_float4 GH_halffloat4() const
	{
		return { hx(), hy(), hz() };
		//float의 x/y/z를 반으로 나눈 h시리즈를 리턴
	}


	int ihx() const
	{
		return static_cast<int>(hx());
	}

	int ihy() const
	{
		return static_cast<int>(hy());
	}

	int ihz() const
	{
		return static_cast<int>(hz());
	}

public:
	GH_float4()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	GH_float4(float _x, float _y)
		: x(_x), y(_y), z(0.0f), w(1.0f)
	{
	}

	// 디폴트 파라미터
	GH_float4(float _x, float _y, float _z, float _w = 1.0f)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	~GH_float4() {

	}

public:
	GH_float4(const GH_float4& _other)
		: x(_other.x), y(_other.y), z(_other.z), w(_other.w)
	{

	}

};


class GH_int4
{
public:
	//unnamed union을 선언
	//내부의 메모리구조를 struct로 묶는것과 같이 사용해서 union 방식으로 구성해준다

	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
			//struct로 묶인 16바이트를
			// x/y/z/w 4바이트 단위로 나눔

		};
		struct
		{
			__int64 HighValue;
			__int64 LowValue;
			//struct로 묶인 16바이트를
			//HighValue/LowValue 8바이트 단위로 나눔
		};


	};
};

class GH_Figure
{
	//위치, 크기를 가지고 있어서 도형을 그릴때 사용할 클래스
public:
	GH_float4 pos_;
	GH_float4 size_;

	//GetPos가 중간이므로, 좌우 위아래의 경우 크기의 절반만큼을 빼야 받아올 수 있다.
public:
	int iLeft()
	{
		return pos_.ix() - size_.ihx();
	}

	int iRight()
	{
		return pos_.ix() + size_.ihx();
	}

	int iTop()
	{
		return pos_.iy() - size_.ihy();
	}

	int iBot()
	{
		return pos_.iy() + size_.ihy();
	}

public:
	GH_Figure(GH_float4 _Pos, GH_float4 _Size)
		: pos_(_Pos), size_(_Size)
	{
	}
};

