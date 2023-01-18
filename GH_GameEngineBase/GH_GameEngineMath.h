#pragma once
//�з� :
//�뵵 :
//���� :

class GH_float4
{
public:
	static const GH_float4 ZERO;
	static const GH_float4 LEFT;
	static const GH_float4 RIGHT;
	static const GH_float4 UP;
	static const GH_float4 DOWN;
	//�̵��� ���� ����

public:
	//unnamed union�� ����
	//������ �޸𸮱����� struct�� ���°Ͱ� ���� ����ؼ� union ������� �������ش�
	//�Ǽ� �� 4���� �������մ� Ŭ����
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
			//struct�� ���� 16����Ʈ��
			// x/y/z/w 4����Ʈ ������ ����

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

	// ���Կ�����
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
		//float�� int�� ����ȯ ���ִ� �Լ�
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
		//float�� x/y/z�� ������ ���� h�ø�� ����
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

	// ����Ʈ �Ķ����
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
	//unnamed union�� ����
	//������ �޸𸮱����� struct�� ���°Ͱ� ���� ����ؼ� union ������� �������ش�

	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
			//struct�� ���� 16����Ʈ��
			// x/y/z/w 4����Ʈ ������ ����

		};
		struct
		{
			__int64 HighValue;
			__int64 LowValue;
			//struct�� ���� 16����Ʈ��
			//HighValue/LowValue 8����Ʈ ������ ����
		};


	};
};

class GH_Figure
{
	//��ġ, ũ�⸦ ������ �־ ������ �׸��� ����� Ŭ����
public:
	GH_float4 pos_;
	GH_float4 size_;

	//GetPos�� �߰��̹Ƿ�, �¿� ���Ʒ��� ��� ũ���� ���ݸ�ŭ�� ���� �޾ƿ� �� �ִ�.
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

