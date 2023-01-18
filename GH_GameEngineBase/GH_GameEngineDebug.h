#pragma once
//분류 : 디버거
//용도 : 메모리관리 메세지출력 로그
//설명 : 모든 디버깅 관련 여기에 모아 놓는다.

class GH_GameEngineDebug
{
private: //디폴트 접근 지정자
	GH_GameEngineDebug(); //디폴트 생성자
	~GH_GameEngineDebug(); //디폴트 소멸자

private:
	GH_GameEngineDebug& operator=(const GH_GameEngineDebug& _Other) = delete; //디폴트 대입연산자
	GH_GameEngineDebug& operator=(GH_GameEngineDebug&& _Other) = delete; //디폴트 RValue 대입연산자
private:

	GH_GameEngineDebug(const GH_GameEngineDebug& _Other) = delete; //디폴트 복사 생성자
	GH_GameEngineDebug(GH_GameEngineDebug&& _Other) noexcept; //디폴트 RValue 복사생성자

public:
	//모든 함수를 static으로 사용
	static void MessageBoxError(const char* _Ptr);
	static void Assert();
	static void GH_LeakCheckOn();
};